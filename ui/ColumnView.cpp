#include "ColumnView.h"
#include "CardView.h"
#include "CardEditDialog.h"
#include "../include/Card.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QDebug>
#include <QTimer>

ColumnView::ColumnView(Column *column, QWidget *parent)
    : QWidget(parent)
    , m_column(column)
    , m_mainLayout(nullptr)
    , m_headerLabel(nullptr)
    , m_addCardButton(nullptr)
    , m_scrollArea(nullptr)
    , m_cardContainer(nullptr)
    , m_cardLayout(nullptr)
{
    setupUI();
    refreshCards();
}

void ColumnView::setupUI() {
    // Layout principal
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(5);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    
    // Header da coluna
    m_headerLabel = new QLabel(this);
    QFont headerFont = m_headerLabel->font();
    headerFont.setPointSize(12);
    headerFont.setBold(true);
    m_headerLabel->setFont(headerFont);
    m_headerLabel->setAlignment(Qt::AlignCenter);
    m_headerLabel->setStyleSheet(R"(
        QLabel {
            background-color: #34495e;
            color: white;
            padding: 10px;
            border-radius: 4px;
        }
    )");
    updateHeader();
    m_mainLayout->addWidget(m_headerLabel);
    
    // Botão adicionar card
    m_addCardButton = new QPushButton("+ Novo Card", this);
    m_addCardButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 6px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
    )");
    connect(m_addCardButton, &QPushButton::clicked, this, &ColumnView::onAddCard);
    m_mainLayout->addWidget(m_addCardButton);
    
    // Área de scroll para cards
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    m_cardContainer = new QWidget();
    m_cardLayout = new QVBoxLayout(m_cardContainer);
    m_cardLayout->setSpacing(8);
    m_cardLayout->setContentsMargins(5, 5, 5, 5);
    m_cardLayout->setAlignment(Qt::AlignTop);
    
    m_scrollArea->setWidget(m_cardContainer);
    m_mainLayout->addWidget(m_scrollArea);
    
    // Habilita drag & drop
    setAcceptDrops(true);
    
    // Estilo da coluna
    setStyleSheet(R"(
        ColumnView {
            background-color: #95a5a6;
            border: 2px solid #7f8c8d;
            border-radius: 6px;
        }
        QScrollArea {
            background-color: #95a5a6;
            border: none;
        }
    )");
    
    setFixedWidth(280);
    setMinimumHeight(400);
}

void ColumnView::updateHeader() {
    QString headerText = QString::fromStdString(m_column->getName());
    int currentCards = m_column->getCardCount();
    int wipLimit = m_column->getWipLimit();
    
    if (wipLimit > 0) {
        headerText += QString(" (%1/%2)").arg(currentCards).arg(wipLimit);
        
        // Muda cor se estiver próximo/acima do limite
        if (currentCards >= wipLimit) {
            m_headerLabel->setStyleSheet(R"(
                QLabel {
                    background-color: #e74c3c;
                    color: white;
                    padding: 10px;
                    border-radius: 4px;
                }
            )");
        } else if (currentCards >= wipLimit * 0.8) {
            m_headerLabel->setStyleSheet(R"(
                QLabel {
                    background-color: #f39c12;
                    color: white;
                    padding: 10px;
                    border-radius: 4px;
                }
            )");
        } else {
            m_headerLabel->setStyleSheet(R"(
                QLabel {
                    background-color: #34495e;
                    color: white;
                    padding: 10px;
                    border-radius: 4px;
                }
            )");
        }
    } else {
        headerText += QString(" (%1)").arg(currentCards);
    }
    
    m_headerLabel->setText(headerText);
}

void ColumnView::refreshCards() {
    // Remove views antigas
    for (auto *view : m_cardViews) {
        m_cardLayout->removeWidget(view);
        delete view;
    }
    m_cardViews.clear();
    
    // Cria view para cada card usando índices para maior segurança
    auto& cards = m_column->getCards();
    for (size_t i = 0; i < cards.size(); ++i) {
        CardView *cardView = new CardView(&cards[i], m_cardContainer);
        
        // Armazena ponteiro do ColumnView no CardView para facilitar busca
        cardView->setProperty("columnView", QVariant::fromValue<QWidget*>(this));
        
        connect(cardView, &CardView::modified, this, &ColumnView::onCardModified);
        connect(cardView, &CardView::doubleClicked, this, &ColumnView::onCardDoubleClicked);
        connect(cardView, &CardView::dragStarted, this, &ColumnView::onCardDragStarted);
        m_cardLayout->addWidget(cardView);
        m_cardViews.push_back(cardView);
    }
    
    // Removido addStretch() para cards ficarem no topo
    updateHeader();
}

void ColumnView::onAddCard() {
    // Verifica WIP limit
    if (m_column->isFull()) {
        QMessageBox::warning(this, "Limite de Cards Atingido",
                           "Esta coluna atingiu o limite máximo de cards. Não é possível adicionar mais.");
        return;
    }
    
    bool ok;
    QString title = QInputDialog::getText(this, "Novo Card",
                                         "Título do card:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || title.isEmpty()) {
        return;
    }
    
    // Cria card com ID único
    static int cardCounter = 1;
    std::string cardId = "card_" + std::to_string(cardCounter++);
    
    Card card(cardId, title.toStdString());
    
    if (m_column->addCard(card)) {
        refreshCards();
        emit modified();
    } else {
        QMessageBox::warning(this, "Erro",
                           "Não foi possível adicionar o card.");
    }
}

void ColumnView::onCardModified() {
    updateHeader();
    emit modified();
}

void ColumnView::onCardDoubleClicked() {
    // Encontra qual CardView enviou o sinal
    CardView *cardView = qobject_cast<CardView*>(sender());
    if (!cardView || !cardView->getCard()) {
        return;
    }
    
    // Abre diálogo de edição
    CardEditDialog dialog(cardView->getCard(), this);
    connect(&dialog, &CardEditDialog::cardModified, this, [this, cardView]() {
        cardView->updateDisplay();  // Atualiza a visualização
        emit modified();
    });
    
    dialog.exec();
}

void ColumnView::onCardDragStarted(CardView *cardView) {
    // Este slot agora é opcional - o CardView já cria o QDrag
    // Mantido para compatibilidade futura
}

void ColumnView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-cardview")) {
        // Verifica se não está cheio
        if (m_column->isFull()) {
            event->ignore();
        } else {
            event->acceptProposedAction();
            highlightDropZone(true);
        }
    } else {
        event->ignore();
    }
}

void ColumnView::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-cardview")) {
        if (!m_column->isFull()) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void ColumnView::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasFormat("application/x-cardview")) {
        QByteArray dropData = event->mimeData()->data("application/x-cardview");
        CardView *sourceCardView = reinterpret_cast<CardView*>(dropData.toULongLong());
        
        if (sourceCardView && sourceCardView->getCard()) {
            Card *card = sourceCardView->getCard();
            
            // Recupera a coluna de origem da property armazenada
            QVariant columnVariant = sourceCardView->property("columnView");
            ColumnView *sourceColumn = qobject_cast<ColumnView*>(columnVariant.value<QWidget*>());
            
            if (!sourceColumn) {
                qDebug() << "ERRO: Não encontrou coluna de origem!";
                highlightDropZone(false);
                return;
            }
            
            if (sourceColumn == this) {
                qDebug() << "Tentando mover card para a mesma coluna - ignorando";
                highlightDropZone(false);
                return;
            }
            
            qDebug() << "Movendo card de" << QString::fromStdString(sourceColumn->getColumn()->getName()) 
                     << "para" << QString::fromStdString(m_column->getName());
            
            // IMPORTANTE: Fazer CÓPIA do card antes de remover!
            // O ponteiro 'card' fica inválido após removeCard()
            std::string cardId = card->getId();
            Card cardCopy = *card; // Cópia completa antes de remover
            
            // Move o card entre colunas
            if (sourceColumn->getColumn()->removeCard(cardId)) {
                if (m_column->addCard(cardCopy)) {
                    qDebug() << "Card movido com sucesso!";
                    event->acceptProposedAction();
                    emit modified();
                    
                    // CRÍTICO: Usar QTimer para adiar refreshCards()
                    // Isso permite que o drag->exec() termine ANTES de deletar os widgets
                    QTimer::singleShot(0, sourceColumn, [sourceColumn]() {
                        sourceColumn->refreshCards();
                    });
                    QTimer::singleShot(0, this, [this]() {
                        this->refreshCards();
                    });
                } else {
                    qDebug() << "Falha ao adicionar card na coluna destino";
                    // Falha ao adicionar - retorna para coluna original
                    sourceColumn->getColumn()->addCard(cardCopy);
                }
            } else {
                qDebug() << "Falha ao remover card da coluna origem";
            }
        }
        
        highlightDropZone(false);
    }
}

void ColumnView::dragLeaveEvent(QDragLeaveEvent *event) {
    highlightDropZone(false);
    event->accept();
}

void ColumnView::highlightDropZone(bool highlight) {
    if (highlight) {
        // Aplica estilo de highlight em TODOS os widgets
        QString highlightStyle = R"(
            background-color: #a8d5ff;
            border: 3px dashed #3498db;
            border-radius: 6px;
        )";
        
        setStyleSheet("ColumnView { " + highlightStyle + " }");
        m_cardContainer->setStyleSheet("QWidget { background-color: #a8d5ff; }");
        m_scrollArea->setStyleSheet("QScrollArea { background-color: #a8d5ff; border: none; }");
        
        qDebug() << "✨ Coluna destacada:" << QString::fromStdString(m_column->getName());
    } else {
        // Restaura estilo normal
        setStyleSheet(R"(
            ColumnView {
                background-color: #95a5a6;
                border: 2px solid #7f8c8d;
                border-radius: 6px;
            }
        )");
        m_cardContainer->setStyleSheet("QWidget { background-color: #95a5a6; }");
        m_scrollArea->setStyleSheet("QScrollArea { background-color: #95a5a6; border: none; }");
        
        qDebug() << "🔄 Coluna restaurada:" << QString::fromStdString(m_column->getName());
    }
}
