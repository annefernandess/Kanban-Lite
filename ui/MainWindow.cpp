#include "MainWindow.h"
#include "BoardView.h"
#include "../include/Board.h"
#include "../include/Column.h"
#include "../include/Card.h"
#include "../include/User.h"
#include "../include/ActivityLog.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QStatusBar>
#include <QCloseEvent>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <fstream>

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_scrollArea(nullptr)
    , m_boardContainer(nullptr)
    , m_boardLayout(nullptr)
    , m_statusLabel(nullptr)
    , m_fileMenu(nullptr)
    , m_helpMenu(nullptr)
    , m_modified(false)
    , m_currentFile("")
{
    // Define o caminho do arquivo baseado no diretório do executável
    // Se executado de build/ui, vai para ../../data/kanban_data.json (raiz do projeto)
    QString appDir = QCoreApplication::applicationDirPath();
    m_currentFile = QDir(appDir).filePath("../../data/kanban_data.json");
    m_currentFile = QDir::cleanPath(m_currentFile); // Normaliza o caminho
    
    qDebug() << "Diretório do executável:" << appDir;
    qDebug() << "Arquivo de dados:" << m_currentFile;
    
    // Inicializa ActivityLog
    m_activityLog = std::make_unique<ActivityLog>();
    
    // Cria usuário padrão
    m_users.push_back(std::make_unique<User>("default", "Default User", "user@example.com"));
    
    setupUI();
    createMenus();
    loadDefaultState();
    
    setWindowTitle("Kanban-Lite - Trabalho Final POO");
    resize(1200, 800);
}

MainWindow::~MainWindow() {
    // Smart pointers farão a limpeza automática
}

void MainWindow::setupUI() {
    // Widget central
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // Layout principal
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Label de título
    QLabel *titleLabel = new QLabel("📋 Kanban-Lite - Gerenciamento de Tarefas", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("padding: 10px; background-color: #2c3e50; color: white; border-radius: 5px;");
    m_mainLayout->addWidget(titleLabel);
    
    // Campo de filtro por tag
    m_filterLayout = new QHBoxLayout();
    
    QLabel *filterLabel = new QLabel("🔍 Filtrar por tag:", this);
    filterLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    m_filterLayout->addWidget(filterLabel);
    
    m_filterInput = new QLineEdit(this);
    m_filterInput->setPlaceholderText("Digite uma tag para filtrar os cards (ex: urgente, bug, frontend)");
    m_filterInput->setClearButtonEnabled(true);
    connect(m_filterInput, &QLineEdit::textChanged, this, &MainWindow::onFilterChanged);
    m_filterLayout->addWidget(m_filterInput, 1);
    
    m_clearFilterButton = new QPushButton("✖ Limpar Filtro", this);
    m_clearFilterButton->setEnabled(false);
    connect(m_clearFilterButton, &QPushButton::clicked, this, &MainWindow::onClearFilter);
    m_filterLayout->addWidget(m_clearFilterButton);
    
    m_mainLayout->addLayout(m_filterLayout);
    
    // Área de scroll para boards
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Container de boards
    m_boardContainer = new QWidget();
    m_boardLayout = new QHBoxLayout(m_boardContainer);
    m_boardLayout->setSpacing(20);
    m_boardLayout->setContentsMargins(20, 20, 20, 20);
    
    m_scrollArea->setWidget(m_boardContainer);
    m_mainLayout->addWidget(m_scrollArea);
    
    // Status bar
    m_statusLabel = new QLabel("Pronto", this);
    statusBar()->addWidget(m_statusLabel);
    
    // Estilo geral
    setStyleSheet(R"(
        QMainWindow {
            background-color: #ecf0f1;
        }
        QScrollArea {
            border: none;
            background-color: #ecf0f1;
        }
    )");
}

void MainWindow::createMenus() {
    // Menu Arquivo
    m_fileMenu = menuBar()->addMenu("&Arquivo");
    
    m_newBoardAction = new QAction("&Novo Board", this);
    m_newBoardAction->setShortcut(QKeySequence::New);
    connect(m_newBoardAction, &QAction::triggered, this, &MainWindow::onNewBoard);
    m_fileMenu->addAction(m_newBoardAction);
    
    m_fileMenu->addSeparator();
    
    m_saveAction = new QAction("&Salvar", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveState);
    m_fileMenu->addAction(m_saveAction);
    
    m_loadAction = new QAction("&Carregar", this);
    m_loadAction->setShortcut(QKeySequence::Open);
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::onLoadState);
    m_fileMenu->addAction(m_loadAction);
    
    m_fileMenu->addSeparator();
    
    m_exitAction = new QAction("&Sair", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
    m_fileMenu->addAction(m_exitAction);
    
    // Menu Ajuda
    m_helpMenu = menuBar()->addMenu("A&juda");
    
    m_aboutAction = new QAction("&Sobre", this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::loadDefaultState() {
    // Tenta carregar estado padrão
    std::ifstream file(m_currentFile.toStdString());
    if (file.good()) {
        file.close();
        onLoadState();
    } else {
        // Sem board salvo - mostra tela de boas-vindas
        m_statusLabel->setText("Nenhum board criado. Crie seu primeiro board!");
        refreshBoardView();
    }
}

void MainWindow::refreshBoardView() {
    // Remove todos os widgets do layout (incluindo welcome widget)
    QLayoutItem *item;
    while ((item = m_boardLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    
    // Limpa vector de board views
    m_boardViews.clear();
    
    // Se não há boards, mostra tela de boas-vindas
    if (m_boards.empty()) {
        QWidget *welcomeWidget = new QWidget(this);
        QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeWidget);
        welcomeLayout->setAlignment(Qt::AlignCenter);
        
        QLabel *welcomeIcon = new QLabel("📋", welcomeWidget);
        QFont iconFont = welcomeIcon->font();
        iconFont.setPointSize(48);
        welcomeIcon->setFont(iconFont);
        welcomeIcon->setAlignment(Qt::AlignCenter);
        
        QLabel *welcomeTitle = new QLabel("Bem-vindo ao Kanban-Lite!", welcomeWidget);
        QFont titleFont = welcomeTitle->font();
        titleFont.setPointSize(18);
        titleFont.setBold(true);
        welcomeTitle->setFont(titleFont);
        welcomeTitle->setAlignment(Qt::AlignCenter);
        
        QLabel *welcomeText = new QLabel(
            "Você ainda não tem nenhum board.\n\n"
            "Crie seu primeiro board para começar a organizar suas tarefas!",
            welcomeWidget
        );
        welcomeText->setAlignment(Qt::AlignCenter);
        welcomeText->setWordWrap(true);
        
        QPushButton *createFirstButton = new QPushButton("🎯 Criar Primeiro Board", welcomeWidget);
        QFont buttonFont = createFirstButton->font();
        buttonFont.setPointSize(12);
        buttonFont.setBold(true);
        createFirstButton->setFont(buttonFont);
        createFirstButton->setStyleSheet(R"(
            QPushButton {
                background-color: #27ae60;
                color: white;
                border: none;
                padding: 15px 30px;
                border-radius: 8px;
                min-width: 250px;
            }
            QPushButton:hover {
                background-color: #229954;
            }
            QPushButton:pressed {
                background-color: #1e8449;
            }
        )");
        connect(createFirstButton, &QPushButton::clicked, this, &MainWindow::onNewBoard);
        
        welcomeLayout->addWidget(welcomeIcon);
        welcomeLayout->addSpacing(20);
        welcomeLayout->addWidget(welcomeTitle);
        welcomeLayout->addSpacing(10);
        welcomeLayout->addWidget(welcomeText);
        welcomeLayout->addSpacing(30);
        welcomeLayout->addWidget(createFirstButton, 0, Qt::AlignCenter);
        
        welcomeWidget->setStyleSheet(R"(
            QWidget {
                background-color: white;
                border-radius: 12px;
                padding: 40px;
            }
        )");
        
        m_boardLayout->addWidget(welcomeWidget);
        // Não adiciona ao m_boardViews pois é QWidget, não BoardView
        // Será removido na próxima chamada de refreshBoardView
        return;
    }
    
    // Cria views para cada board
    for (auto& board : m_boards) {
        BoardView *boardView = new BoardView(board.get(), this);
        connect(boardView, &BoardView::modified, this, &MainWindow::onBoardModified);
        
        // Se há apenas 1 board, ele deve ocupar TODO o espaço (stretch = 1)
        // Se há múltiplos boards, usa tamanho fixo (stretch = 0)
        int stretch = (m_boards.size() == 1) ? 1 : 0;
        m_boardLayout->addWidget(boardView, stretch);
        
        m_boardViews.push_back(boardView);
    }
    
    // Só adiciona stretch se houver múltiplos boards
    if (m_boards.size() > 1) {
        m_boardLayout->addStretch();
    }
}

void MainWindow::onNewBoard() {
    bool ok;
    QString name = QInputDialog::getText(this, "Novo Board",
                                         "Nome do board:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !name.isEmpty()) {
        auto board = std::make_unique<Board>(name.toStdString(), name.toStdString());
        board->attachActivityLog(m_activityLog.get());
        m_boards.push_back(std::move(board));
        
        refreshBoardView();
        m_modified = true;
        m_statusLabel->setText(QString("Board '%1' criado").arg(name));
    }
}

void MainWindow::onSaveState() {
    try {
        json state;
        
        // Serializa boards
        json boardsArray = json::array();
        for (const auto& board : m_boards) {
            boardsArray.push_back(board->toJson());
        }
        state["boards"] = boardsArray;
        
        // Serializa users
        json usersArray = json::array();
        for (const auto& user : m_users) {
            usersArray.push_back(user->toJson());
        }
        state["users"] = usersArray;
        
        // Serializa activity log
        if (m_activityLog) {
            state["activityLog"] = m_activityLog->toJson();
        }
        
        // Metadata
        state["metadata"] = {
            {"version", "1.0"},
            {"appVersion", "GUI-v4"}
        };
        
        // Cria diretório se não existir
        QFileInfo fileInfo(m_currentFile);
        QDir dir = fileInfo.absoluteDir();
        if (!dir.exists()) {
            qDebug() << "Criando diretório:" << dir.absolutePath();
            if (!dir.mkpath(".")) {
                throw std::runtime_error("Não foi possível criar diretório: " + dir.absolutePath().toStdString());
            }
        }
        
        // Salva arquivo
        std::ofstream file(m_currentFile.toStdString());
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir arquivo para escrita: " + m_currentFile.toStdString());
        }
        
        file << state.dump(2);
        file.close();
        
        m_modified = false;
        m_statusLabel->setText(QString("✓ Estado salvo em '%1'").arg(m_currentFile));
        
        if (m_activityLog) {
            m_activityLog->record("Estado salvo via GUI");
        }
        
        QMessageBox::information(this, "Salvar",
                               QString("Estado salvo com sucesso!\n\nArquivo: %1\nBoards: %2")
                               .arg(m_currentFile)
                               .arg(m_boards.size()));
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erro ao Salvar",
                            QString("Erro: %1").arg(e.what()));
    }
}

void MainWindow::onLoadState() {
    try {
        std::ifstream file(m_currentFile.toStdString());
        if (!file.is_open()) {
            throw std::runtime_error("Arquivo não encontrado");
        }
        
        json state;
        file >> state;
        file.close();
        
        // Limpa estado atual
        m_boards.clear();
        m_users.clear();
        
        // Carrega users
        if (state.contains("users") && state["users"].is_array()) {
            for (const auto& userJson : state["users"]) {
                m_users.push_back(std::make_unique<User>(User::fromJson(userJson)));
            }
        }
        
        // Carrega boards
        if (state.contains("boards") && state["boards"].is_array()) {
            for (const auto& boardJson : state["boards"]) {
                auto board = std::make_unique<Board>(Board::fromJson(boardJson));
                board->attachActivityLog(m_activityLog.get());
                m_boards.push_back(std::move(board));
            }
        }
        
        // Carrega activity log
        if (state.contains("activityLog")) {
            m_activityLog = std::make_unique<ActivityLog>(
                ActivityLog::fromJson(state["activityLog"])
            );
        }
        
        // Atualiza UI
        refreshBoardView();
        
        m_modified = false;
        m_statusLabel->setText(QString("✓ Carregado %1 board(s)").arg(m_boards.size()));
        
        QMessageBox::information(this, "Carregar",
                               QString("Estado carregado com sucesso!\n\nBoards: %1\nUsuários: %2")
                               .arg(m_boards.size())
                               .arg(m_users.size()));
        
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erro ao Carregar",
                           QString("Não foi possível carregar o estado:\n%1").arg(e.what()));
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "Sobre Kanban-Lite",
        "<h2>Kanban-Lite GUI</h2>"
        "<p><b>Trabalho Final - Programação Orientada a Objetos (C++)</b></p>"
        "<p>Aluna: Anne Fernandes da Costa Oliveira</p>"
        "<p>Matrícula: 20240010789</p>"
        "<hr>"
        "<p><b>Etapas concluídas:</b></p>"
        "<ul>"
        "<li>Etapa 1: Design e diagramas UML</li>"
        "<li>Etapa 2: CLI funcional</li>"
        "<li>Etapa 3: Interface Gráfica (Qt)</li>"
        "</ul>"
        "<hr>"
        "<p>Tecnologias: C++17, Qt6, CMake, nlohmann/json, CTest</p>"
    );
}

void MainWindow::onBoardModified() {
    m_modified = true;
    m_statusLabel->setText("Modificado (não salvo)");
}

void MainWindow::onFilterChanged(const QString &text) {
    QString filterTag = text.trimmed();
    m_clearFilterButton->setEnabled(!filterTag.isEmpty());
    
    if (filterTag.isEmpty()) {
        // Sem filtro - mostra todos os cards
        for (auto *boardView : m_boardViews) {
            boardView->setVisible(true);
        }
        m_statusLabel->setText("Pronto");
        return;
    }
    
    // Aplica filtro em todos os boards
    int totalCardsFound = 0;
    for (size_t i = 0; i < m_boards.size(); ++i) {
        auto cards = m_boards[i]->findCardsByTag(filterTag.toStdString());
        totalCardsFound += cards.size();
    }
    
    // Atualiza status
    if (totalCardsFound == 0) {
        m_statusLabel->setText(QString("🔍 Nenhum card encontrado com a tag '%1'").arg(filterTag));
    } else {
        m_statusLabel->setText(QString("🔍 Filtro ativo: '%1' - %2 card(s) encontrado(s)")
                              .arg(filterTag)
                              .arg(totalCardsFound));
    }
    
    // TODO: Implementar highlight visual nos cards filtrados
    // Por enquanto, apenas mostra a contagem
}

void MainWindow::onClearFilter() {
    m_filterInput->clear();
    m_statusLabel->setText("Pronto");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_modified) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Salvar alterações?",
            "O estado foi modificado. Deseja salvar antes de sair?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );
        
        if (reply == QMessageBox::Save) {
            onSaveState();
            event->accept();
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}
