#ifndef CARDEDITDIALOG_H
#define CARDEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

class Card;

/**
 * @brief Diálogo para editar propriedades de um Card
 * 
 * Permite editar título, descrição, prioridade e tags
 */
class CardEditDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit CardEditDialog(Card *card, QWidget *parent = nullptr);
    
signals:
    void cardModified();
    
private slots:
    void onSave();
    void onAddTag();
    void onRemoveTag();
    
private:
    void setupUI();
    void loadCardData();
    void updateTagsList();
    
    Card *m_card;
    QLineEdit *m_titleEdit;
    QTextEdit *m_descriptionEdit;
    QSpinBox *m_prioritySpinBox;
    QLineEdit *m_tagInput;
    QLabel *m_tagsLabel;
    QPushButton *m_addTagButton;
    QPushButton *m_removeTagButton;
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
};

#endif // CARDEDITDIALOG_H
