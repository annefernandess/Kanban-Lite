#include "MainWindow.h"
#include <QApplication>

/**
 * @brief Ponto de entrada da aplicação GUI Kanban-Lite
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Configura informações da aplicação
    app.setApplicationName("Kanban-Lite");
    app.setApplicationVersion("3.0");
    app.setOrganizationName("UFPB");
    
    // Cria e exibe janela principal
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
