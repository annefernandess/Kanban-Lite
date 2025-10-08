#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>

class Card;

/**
 * @brief Widget que exibe um Card individual
 * 
 * Mostra título, descrição, prioridade (com código de cores)
 * e informações de atribuição. Suporta double-click para edição.
 */
class CardView : public QWidget {
    Q_OBJECT
    
public:
    explicit CardView(Card *card, QWidget *parent = nullptr);
    
    Card* getCard() const { return m_card; }
    void updateDisplay();  // Tornado público para permitir refresh após edição
    
signals:
    void modified();
    void doubleClicked();
    void dragStarted(CardView *card);
    
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    
private:
    void setupUI();
    QString getPriorityColor() const;
    
    Card *m_card;
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QLabel *m_tagsLabel;
    QLabel *m_infoLabel;
    
    // Drag & Drop
    QPoint m_dragStartPos;
};

#endif // CARDVIEW_H
