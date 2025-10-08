#include "BoardView.h"
#include "ColumnView.h"
#include "../include/Board.h"
#include "../include/Column.h"
#include "../include/Card.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QFrame>
#include <QTimer>

BoardView::BoardView(Board *board, QWidget *parent)
    : QWidget(parent)
    , m_board(board)
    , m_mainLayout(nullptr)
    , m_titleLabel(nullptr)
    , m_scrollArea(nullptr)
    , m_columnContainer(nullptr)
    , m_columnLayout(nullptr)
    , m_addColumnButton(nullptr)
{
    setupUI();
    refreshColumns();
}

void BoardView::setupUI() {
    // Layout principal
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);
    
    // Header do board
    QWidget *headerWidget = new QWidget(this);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    
    m_titleLabel = new QLabel(QString::fromStdString(m_board->getName()), this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setStyleSheet("color: #2c3e50; padding: 10px;");
    headerLayout->addWidget(m_titleLabel);
    
    m_addColumnButton = new QPushButton("+ Nova Coluna", this);
    m_addColumnButton->setStyleSheet(R"(
        QPushButton {
            background-color: #27ae60;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #229954;
        }
    )");
    connect(m_addColumnButton, &QPushButton::clicked, this, &BoardView::onAddColumn);
    headerLayout->addWidget(m_addColumnButton);
    
    m_mainLayout->addWidget(headerWidget);
    
    // Área de scroll para colunas
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setMinimumHeight(500);
    
    m_columnContainer = new QWidget();
    m_columnLayout = new QHBoxLayout(m_columnContainer);
    m_columnLayout->setSpacing(15);
    m_columnLayout->setContentsMargins(10, 10, 10, 10);
    m_columnLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    m_scrollArea->setWidget(m_columnContainer);
    m_mainLayout->addWidget(m_scrollArea);
    
    // Estilo do board
    setStyleSheet(R"(
        BoardView {
            background-color: white;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            padding: 10px;
        }
        QScrollArea {
            background-color: #ecf0f1;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
        }
    )");
    
    setMinimumWidth(400);
    setMaximumWidth(1000);
}

void BoardView::refreshColumns() {
    // Remove views antigas
    for (auto *view : m_columnViews) {
        m_columnLayout->removeWidget(view);
        delete view;
    }
    m_columnViews.clear();
    
    // Cria view para cada coluna usando índices para maior segurança
    auto& columns = m_board->getColumns();
    for (size_t i = 0; i < columns.size(); ++i) {
        ColumnView *columnView = new ColumnView(&columns[i], this);
        connect(columnView, &ColumnView::modified, this, &BoardView::onColumnModified);
        m_columnLayout->addWidget(columnView);
        m_columnViews.push_back(columnView);
    }
    
    m_columnLayout->addStretch();
}

void BoardView::onAddColumn() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nova Coluna",
                                         "Nome da coluna:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || name.isEmpty()) {
        return;
    }
    
    int wipLimit = QInputDialog::getInt(this, "Limite de Cards",
                                       "Quantos cards no máximo? (0 = sem limite):",
                                       0, 0, 100, 1, &ok);
    if (!ok) {
        return;
    }
    
    // Converte 0 para -1 (convenção do sistema: -1 = sem limite)
    if (wipLimit == 0) {
        wipLimit = -1;
    }
    
    try {
        // Cria coluna
        Column column(name.toStdString(), wipLimit);
        
        if (m_board->addColumn(column)) {
            // IMPORTANTE: Usar QTimer para fazer refresh após event loop processar
            QTimer::singleShot(0, this, [this, name]() {
                refreshColumns();
                emit modified();
                QMessageBox::information(this, "Sucesso",
                                       QString("Coluna '%1' adicionada!").arg(name));
            });
        } else {
            QMessageBox::warning(this, "Erro",
                               "Não foi possível adicionar a coluna. Nome já existe?");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erro Fatal",
                            QString("Erro ao adicionar coluna: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erro Fatal",
                            "Erro desconhecido ao adicionar coluna.");
    }
}

void BoardView::onColumnModified() {
    emit modified();
}
