#ifndef KANBAN_COLUMNVIEW_H
#define KANBAN_COLUMNVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <vector>
#include "../include/Column.h"

class CardView;

/**
 * @class ColumnView
 * @brief Widget que visualiza uma Column com seus cards
 * 
 * Exibe:
 * - Nome da coluna e limite WIP
 * - Cards empilhados verticalmente
 * - Botão para adicionar novo card
 * 
 * Funcionalidades:
 * - Adicionar cards
 * - Drag & drop (futuro)
 */
class ColumnView : public QWidget {
    Q_OBJECT

public:
    explicit ColumnView(Column *column, QWidget *parent = nullptr);
    ~ColumnView() override = default;
    
    Column* getColumn() const { return m_column; }

signals:
    void modified();

private slots:
    void onAddCard();
    void onCardModified();
    void onCardDoubleClicked();
    void onCardDragStarted(CardView *cardView);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;

private:
    void setupUI();
    void refreshCards();
    void updateHeader();
    void highlightDropZone(bool highlight);
    
    Column *m_column; // Não owned
    
    // Widgets
    QVBoxLayout *m_mainLayout;
    QLabel *m_headerLabel;
    QPushButton *m_addCardButton;
    QScrollArea *m_scrollArea;
    QWidget *m_cardContainer;
    QVBoxLayout *m_cardLayout;
    
    // Views dos cards
    std::vector<CardView*> m_cardViews;
};

#endif // KANBAN_COLUMNVIEW_H
