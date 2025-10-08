#ifndef KANBAN_MAINWINDOW_H
#define KANBAN_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <memory>
#include <vector>
#include "../include/Board.h"
#include "../include/User.h"
#include "../include/ActivityLog.h"

class BoardView;

/**
 * @class MainWindow
 * @brief Janela principal da aplicação Kanban-Lite GUI
 * 
 * Responsável por:
 * - Menu principal (Arquivo, Ajuda)
 * - Gerenciar visualização de boards
 * - Persistência automática (save/load)
 * - Integração com backend (Board, User, ActivityLog)
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    /**
     * @brief Intercepta evento de fechamento para perguntar sobre salvar
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Menu Arquivo
    void onNewBoard();
    void onSaveState();
    void onLoadState();
    void onExit();
    
    // Menu Ajuda
    void onAbout();
    
    // Gerenciamento de boards
    void onBoardModified();
    
    // Filtros
    void onFilterChanged(const QString &text);
    void onClearFilter();

private:
    void setupUI();
    void createMenus();
    void loadDefaultState();
    void refreshBoardView();
    
    // Widgets principais
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_filterLayout;
    QLineEdit *m_filterInput;
    QPushButton *m_clearFilterButton;
    QScrollArea *m_scrollArea;
    QWidget *m_boardContainer;
    QHBoxLayout *m_boardLayout;
    QLabel *m_statusLabel;
    
    // Menus e ações
    QMenu *m_fileMenu;
    QMenu *m_helpMenu;
    QAction *m_newBoardAction;
    QAction *m_saveAction;
    QAction *m_loadAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    
    // Dados do backend
    std::vector<std::unique_ptr<Board>> m_boards;
    std::vector<std::unique_ptr<User>> m_users;
    std::unique_ptr<ActivityLog> m_activityLog;
    
    // Views
    std::vector<BoardView*> m_boardViews;
    
    // Estado
    bool m_modified;
    QString m_currentFile;
};

#endif // KANBAN_MAINWINDOW_H
