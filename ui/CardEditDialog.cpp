#include "CardEditDialog.h"
#include "../include/Card.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QGroupBox>

CardEditDialog::CardEditDialog(Card *card, QWidget *parent)
    : QDialog(parent)
    , m_card(card)
{
    setWindowTitle("Editar Card");
    setMinimumWidth(500);
    setupUI();
    loadCardData();
}

void CardEditDialog::setupUI() {
    auto *mainLayout = new QVBoxLayout(this);
    
    // Form para campos principais
    auto *formLayout = new QFormLayout();
    
    // Título
    m_titleEdit = new QLineEdit(this);
    formLayout->addRow("Título:", m_titleEdit);
    
    // Descrição
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setMaximumHeight(100);
    formLayout->addRow("Descrição:", m_descriptionEdit);
    
    // Prioridade
    m_prioritySpinBox = new QSpinBox(this);
    m_prioritySpinBox->setRange(0, 10);
    m_prioritySpinBox->setToolTip("0 = Baixa, 1-3 = Baixa, 4-6 = Média, 7-10 = Alta");
    formLayout->addRow("Prioridade:", m_prioritySpinBox);
    
    mainLayout->addLayout(formLayout);
    
    // Seção de Tags
    auto *tagsGroup = new QGroupBox("Tags (Etiquetas)", this);
    auto *tagsLayout = new QVBoxLayout(tagsGroup);
    
    // Tags atuais
    m_tagsLabel = new QLabel("Nenhuma tag", tagsGroup);
    m_tagsLabel->setWordWrap(true);
    m_tagsLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 8px; border-radius: 4px; }");
    tagsLayout->addWidget(m_tagsLabel);
    
    // Input para nova tag
    auto *tagInputLayout = new QHBoxLayout();
    m_tagInput = new QLineEdit(tagsGroup);
    m_tagInput->setPlaceholderText("Digite uma tag (ex: urgente, bug, frontend)");
    tagInputLayout->addWidget(m_tagInput);
    
    m_addTagButton = new QPushButton("➕ Adicionar", tagsGroup);
    connect(m_addTagButton, &QPushButton::clicked, this, &CardEditDialog::onAddTag);
    tagInputLayout->addWidget(m_addTagButton);
    
    tagsLayout->addLayout(tagInputLayout);
    
    // Botão remover tag
    m_removeTagButton = new QPushButton("🗑️ Remover última tag", tagsGroup);
    connect(m_removeTagButton, &QPushButton::clicked, this, &CardEditDialog::onRemoveTag);
    tagsLayout->addWidget(m_removeTagButton);
    
    mainLayout->addWidget(tagsGroup);
    
    // Botões de ação
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_cancelButton = new QPushButton("Cancelar", this);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(m_cancelButton);
    
    m_saveButton = new QPushButton("💾 Salvar", this);
    m_saveButton->setDefault(true);
    connect(m_saveButton, &QPushButton::clicked, this, &CardEditDialog::onSave);
    buttonLayout->addWidget(m_saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Enter no campo de tag adiciona a tag
    connect(m_tagInput, &QLineEdit::returnPressed, this, &CardEditDialog::onAddTag);
}

void CardEditDialog::loadCardData() {
    if (!m_card) return;
    
    m_titleEdit->setText(QString::fromStdString(m_card->getTitle()));
    m_descriptionEdit->setPlainText(QString::fromStdString(m_card->getDescription()));
    m_prioritySpinBox->setValue(m_card->getPriority());
    
    updateTagsList();
}

void CardEditDialog::updateTagsList() {
    if (!m_card) return;
    
    auto tags = m_card->getTags();
    if (tags.empty()) {
        m_tagsLabel->setText("🏷️ Nenhuma tag");
        m_removeTagButton->setEnabled(false);
    } else {
        QString tagsText = "🏷️ Tags: ";
        for (size_t i = 0; i < tags.size(); ++i) {
            if (i > 0) tagsText += ", ";
            tagsText += QString::fromStdString(tags[i]);
        }
        m_tagsLabel->setText(tagsText);
        m_removeTagButton->setEnabled(true);
    }
}

void CardEditDialog::onAddTag() {
    QString tag = m_tagInput->text().trimmed();
    
    if (tag.isEmpty()) {
        QMessageBox::warning(this, "Tag vazia", "Digite uma tag antes de adicionar.");
        return;
    }
    
    if (tag.contains(" ")) {
        QMessageBox::warning(this, "Tag inválida", "Tags não podem conter espaços. Use hífen ou underline.");
        return;
    }
    
    m_card->addTag(tag.toStdString());
    m_tagInput->clear();
    updateTagsList();
}

void CardEditDialog::onRemoveTag() {
    auto tags = m_card->getTags();
    if (!tags.empty()) {
        m_card->removeTag(tags.back());
        updateTagsList();
    }
}

void CardEditDialog::onSave() {
    QString title = m_titleEdit->text().trimmed();
    
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Título vazio", "O card deve ter um título.");
        return;
    }
    
    m_card->setTitle(title.toStdString());
    m_card->setDescription(m_descriptionEdit->toPlainText().toStdString());
    m_card->setPriority(m_prioritySpinBox->value());
    
    emit cardModified();
    accept();
}
