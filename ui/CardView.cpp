#include "CardView.h"
#include "../include/Card.h"
#include "../include/User.h"
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

CardView::CardView(Card *card, QWidget *parent)
    : QWidget(parent)
    , m_card(card)
    , m_mainLayout(nullptr)
    , m_titleLabel(nullptr)
    , m_descriptionLabel(nullptr)
    , m_tagsLabel(nullptr)
    , m_infoLabel(nullptr)
{
    setupUI();
    updateDisplay();
}

void CardView::setupUI() {
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(3);
    m_mainLayout->setContentsMargins(8, 8, 8, 8);
    
    // Título do card
    m_titleLabel = new QLabel(this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(10);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setWordWrap(true);
    m_mainLayout->addWidget(m_titleLabel);
    
    // Descrição do card
    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setMaximumHeight(60);
    QFont descFont = m_descriptionLabel->font();
    descFont.setPointSize(8);
    m_descriptionLabel->setFont(descFont);
    m_mainLayout->addWidget(m_descriptionLabel);
    
    // Tags do card
    m_tagsLabel = new QLabel(this);
    m_tagsLabel->setWordWrap(true);
    QFont tagsFont = m_tagsLabel->font();
    tagsFont.setPointSize(7);
    m_tagsLabel->setFont(tagsFont);
    m_mainLayout->addWidget(m_tagsLabel);
    
    // Info do card (assignee, prioridade)
    m_infoLabel = new QLabel(this);
    QFont infoFont = m_infoLabel->font();
    infoFont.setPointSize(7);
    infoFont.setItalic(true);
    m_infoLabel->setFont(infoFont);
    m_mainLayout->addWidget(m_infoLabel);
    
    // Cursor indica que é clicável
    setCursor(Qt::PointingHandCursor);
    
    setMinimumWidth(240);
    setMaximumWidth(260);
}

void CardView::updateDisplay() {
    if (!m_card) {
        return; // Segurança: evita crash se card é nulo
    }
    
    // Título
    m_titleLabel->setText(QString::fromStdString(m_card->getTitle()));
    
    // Descrição
    std::string desc = m_card->getDescription();
    if (desc.empty()) {
        m_descriptionLabel->hide();
    } else {
        m_descriptionLabel->show();
        QString descText = QString::fromStdString(desc);
        if (descText.length() > 80) {
            descText = descText.left(77) + "...";
        }
        m_descriptionLabel->setText(descText);
    }
    
    // Tags - copia o vector para evitar problemas com referências
    if (m_tagsLabel) {
        try {
            // Copia as tags em vez de usar referência (mais seguro)
            std::vector<std::string> tags = m_card->getTags();
            if (tags.empty()) {
                m_tagsLabel->hide();
            } else {
                m_tagsLabel->show();
                QString tagsText;
                for (size_t i = 0; i < tags.size() && i < 5; ++i) { // Limita a 5 tags
                    if (i > 0) tagsText += " ";
                    // Usa texto simples em vez de HTML para evitar problemas
                    tagsText += QString("🏷️%1").arg(QString::fromStdString(tags[i]));
                }
                m_tagsLabel->setText(tagsText);
                m_tagsLabel->setStyleSheet("QLabel { color: #3498db; font-weight: bold; }");
            }
        } catch (...) {
            // Se houver erro ao obter tags, apenas esconde o label
            if (m_tagsLabel) m_tagsLabel->hide();
        }
    }
    
    // Info
    QString info;
    User* assignee = m_card->getAssignee();
    if (assignee != nullptr) {
        info += QString("👤 %1").arg(QString::fromStdString(assignee->getName()));
    }
    
    // Prioridade
    int priority = m_card->getPriority();
    if (priority > 0) {
        if (!info.isEmpty()) info += " | ";
        
        QString priorityIcon;
        if (priority >= 4) {
            priorityIcon = "🔴 Alta";
        } else if (priority >= 2) {
            priorityIcon = "🟡 Média";
        } else {
            priorityIcon = "🟢 Baixa";
        }
        info += priorityIcon;
    }
    
    if (!info.isEmpty()) {
        m_infoLabel->show();
        m_infoLabel->setText(info);
    } else {
        m_infoLabel->hide();
    }
    
    // Cor de borda baseada na prioridade
    QString borderColor = getPriorityColor();
    setStyleSheet(QString(R"(
        CardView {
            background-color: white;
            border-left: 4px solid %1;
            border-radius: 4px;
        }
        CardView:hover {
            background-color: #f0f0f0;
        }
    )").arg(borderColor));
}

QString CardView::getPriorityColor() const {
    int priority = m_card->getPriority();
    
    if (priority >= 4) {
        return "#e74c3c"; // Vermelho (alta)
    } else if (priority >= 2) {
        return "#f39c12"; // Laranja (média)
    } else if (priority > 0) {
        return "#27ae60"; // Verde (baixa)
    }
    
    return "#95a5a6"; // Cinza padrão (sem prioridade)
}

void CardView::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    emit doubleClicked();
}

void CardView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void CardView::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    
    // Verifica se moveu pixels suficientes para iniciar drag
    if ((event->pos() - m_dragStartPos).manhattanLength() < 10) {
        return;
    }
    
    QString cardTitle = QString::fromStdString(m_card->getTitle());
    qDebug() << "Iniciando drag do card:" << cardTitle;
    
    // Cria e executa operação de drag
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData();
    
    // Armazena ponteiro do CardView no MIME data
    mimeData->setData("application/x-cardview", 
                      QByteArray::number(reinterpret_cast<qulonglong>(this)));
    
    drag->setMimeData(mimeData);
    
    // Emite signal antes de iniciar (para feedback visual se necessário)
    emit dragStarted(this);
    
    // Executa drag (bloqueia até drop)
    // ATENÇÃO: Após exec(), este CardView pode ter sido DELETADO!
    Qt::DropAction result = drag->exec(Qt::MoveAction);
    qDebug() << "Drag finalizado:" << cardTitle << "resultado:" << result;
    
    // NÃO acessa membros após drag->exec() - o widget pode ter sido deletado!
}
