#ifndef KANBAN_BOARDVIEW_H
#define KANBAN_BOARDVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <vector>
#include "../include/Board.h"

class ColumnView;

/**
 * @class BoardView
 * @brief Widget que visualiza um Board completo com suas colunas
 * 
 * Exibe:
 * - Nome do board
 * - Colunas horizontalmente
 * - Botão para adicionar novas colunas
 */
class BoardView : public QWidget {
    Q_OBJECT

public:
    explicit BoardView(Board *board, QWidget *parent = nullptr);
    ~BoardView() override = default;
    
    Board* getBoard() const { return m_board; }

signals:
    void modified();

private slots:
    void onAddColumn();
    void onColumnModified();

private:
    void setupUI();
    void refreshColumns();
    
    Board *m_board; // Não owned (gerenciado por MainWindow)
    
    // Widgets
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QScrollArea *m_scrollArea;
    QWidget *m_columnContainer;
    QHBoxLayout *m_columnLayout;
    QPushButton *m_addColumnButton;
    
    // Views das colunas
    std::vector<ColumnView*> m_columnViews;
};

#endif // KANBAN_BOARDVIEW_H
