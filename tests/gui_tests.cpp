/**
 * @file gui_tests.cpp
 * @brief Testes básicos para widgets da GUI Qt
 * 
 * Testes mínimos que verificam se os widgets podem ser instanciados
 * sem crashes. Não requerem display X11 ativo.
 */

#include "../ui/MainWindow.h"
#include "../ui/BoardView.h"
#include "../ui/ColumnView.h"
#include "../ui/CardView.h"
#include "../include/Board.h"
#include "../include/Column.h"
#include "../include/Card.h"
#include <QApplication>
#include <cassert>
#include <iostream>

void test_board_view_creation() {
    std::cout << "Test: BoardView creation... ";
    Board board("test_board");
    BoardView view(&board);
    std::cout << "✓ OK\n";
}

void test_column_view_creation() {
    std::cout << "Test: ColumnView creation... ";
    Board board("test_board");
    board.addColumn(Column("Test Column", 5));
    auto& columns = board.getColumns();
    if (!columns.empty()) {
        Column* col = const_cast<Column*>(&columns[0]);
        ColumnView view(col);
        std::cout << "✓ OK\n";
    } else {
        std::cout << "✗ FAILED (no columns)\n";
        exit(1);
    }
}

void test_card_view_creation() {
    std::cout << "Test: CardView creation... ";
    Card card("card_1", "Test Card");
    card.setPriority("high");
    card.setAssignee("TestUser");
    CardView view(&card);
    std::cout << "✓ OK\n";
}

void test_mainwindow_creation() {
    std::cout << "Test: MainWindow creation... ";
    // MainWindow cria estado interno automaticamente
    MainWindow* window = new MainWindow();
    assert(window != nullptr);
    delete window;
    std::cout << "✓ OK\n";
}

int main(int argc, char *argv[]) {
    // QApplication necessário para widgets Qt
    QApplication app(argc, argv);
    
    std::cout << "\n=== GUI Tests ===\n\n";
    
    try {
        test_board_view_creation();
        test_column_view_creation();
        test_card_view_creation();
        test_mainwindow_creation();
        
        std::cout << "\n✓ All GUI tests passed!\n\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed with exception: " << e.what() << "\n\n";
        return 1;
    }
}
