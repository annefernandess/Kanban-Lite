/**
 * @file persistence_tests.cpp
 * @brief Testes automatizados para serialização/desserialização JSON
 * @author Anne Fernandes da Costa Oliveira
 * @date 08/10/2025
 */

#include "User.h"
#include "Card.h"
#include "Column.h"
#include "Board.h"
#include "ActivityLog.h"
#include "external/json.hpp"
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>

using json = nlohmann::json;

// Contadores de testes
int tests_run = 0;
int tests_passed = 0;

#define TEST(name) \
    void name(); \
    struct name##_runner { \
        name##_runner() { \
            tests_run++; \
            std::cout << "Running " #name "... "; \
            try { \
                name(); \
                tests_passed++; \
                std::cout << "PASSED\n"; \
            } catch (const std::exception& e) { \
                std::cout << "FAILED: " << e.what() << "\n"; \
            } \
        } \
    } name##_instance; \
    void name()

// ============================================================================
// TESTES DE USER
// ============================================================================

TEST(test_user_serialization) {
    User user("user1", "John Doe", "john@example.com");
    json j = user.toJson();
    
    assert(j["id"] == "user1");
    assert(j["name"] == "John Doe");
    assert(j["email"] == "john@example.com");
}

TEST(test_user_deserialization) {
    json j = {
        {"id", "user2"},
        {"name", "Jane Smith"},
        {"email", "jane@example.com"}
    };
    
    User user = User::fromJson(j);
    assert(user.getId() == "user2");
    assert(user.getName() == "Jane Smith");
    assert(user.getEmail() == "jane@example.com");
}

TEST(test_user_roundtrip) {
    User original("user3", "Bob Johnson", "bob@test.com");
    json j = original.toJson();
    User restored = User::fromJson(j);
    
    assert(original.getId() == restored.getId());
    assert(original.getName() == restored.getName());
    assert(original.getEmail() == restored.getEmail());
}

TEST(test_user_validation_missing_id) {
    json j = {
        {"name", "Test User"},
        {"email", "test@example.com"}
    };
    
    try {
        User::fromJson(j);
        assert(false); // Não deveria chegar aqui
    } catch (const std::invalid_argument& e) {
        // Esperado
    }
}

TEST(test_user_validation_empty_name) {
    json j = {
        {"id", "user4"},
        {"name", ""},
        {"email", "test@example.com"}
    };
    
    try {
        User::fromJson(j);
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Esperado
    }
}

// ============================================================================
// TESTES DE CARD
// ============================================================================

TEST(test_card_serialization) {
    Card card("card1", "Implement feature X");
    card.setDescription("Detailed description");
    card.setPriority(1);
    
    json j = card.toJson();
    
    assert(j["id"] == "card1");
    assert(j["title"] == "Implement feature X");
    assert(j["description"] == "Detailed description");
    assert(j["priority"] == 1);
}

TEST(test_card_deserialization) {
    json j = {
        {"id", "card2"},
        {"title", "Fix bug Y"},
        {"description", "Bug details"},
        {"priority", 2}
    };
    
    Card card = Card::fromJson(j);
    assert(card.getId() == "card2");
    assert(card.getTitle() == "Fix bug Y");
    assert(card.getDescription() == "Bug details");
    assert(card.getPriority() == 2);
}

TEST(test_card_timestamp_serialization) {
    Card card("card3", "Task with timestamp");
    
    // Aguarda 10ms para garantir diferença
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    json j = card.toJson();
    
    // Verifica que timestamps existem e são números
    assert(j.contains("createdAt"));
    assert(j.contains("updatedAt"));
    assert(j["createdAt"].is_number());
    assert(j["updatedAt"].is_number());
}

TEST(test_card_roundtrip) {
    Card original("card4", "Test Card");
    original.setDescription("Test description");
    original.setPriority(3);
    
    json j = original.toJson();
    Card restored = Card::fromJson(j);
    
    assert(original.getId() == restored.getId());
    assert(original.getTitle() == restored.getTitle());
    assert(original.getDescription() == restored.getDescription());
    assert(original.getPriority() == restored.getPriority());
}

TEST(test_card_validation_missing_title) {
    json j = {
        {"id", "card5"}
    };
    
    try {
        Card::fromJson(j);
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Esperado
    }
}

// ============================================================================
// TESTES DE COLUMN
// ============================================================================

TEST(test_column_serialization) {
    Column column("To Do", 5);
    Card card1("c1", "Task 1");
    Card card2("c2", "Task 2");
    
    column.addCard(card1);
    column.addCard(card2);
    
    json j = column.toJson();
    
    assert(j["name"] == "To Do");
    assert(j["wipLimit"] == 5);
    assert(j["cards"].is_array());
    assert(j["cards"].size() == 2);
}

TEST(test_column_deserialization) {
    json j = {
        {"name", "In Progress"},
        {"wipLimit", 3},
        {"cards", json::array({
            {{"id", "c3"}, {"title", "Task 3"}},
            {{"id", "c4"}, {"title", "Task 4"}}
        })}
    };
    
    Column column = Column::fromJson(j);
    assert(column.getName() == "In Progress");
    assert(column.getWipLimit() == 3);
    assert(column.getCardCount() == 2);
}

TEST(test_column_roundtrip) {
    Column original("Done", 10);
    Card card1("c5", "Completed Task 1");
    Card card2("c6", "Completed Task 2");
    
    original.addCard(card1);
    original.addCard(card2);
    
    json j = original.toJson();
    Column restored = Column::fromJson(j);
    
    assert(original.getName() == restored.getName());
    assert(original.getWipLimit() == restored.getWipLimit());
    assert(original.getCardCount() == restored.getCardCount());
}

TEST(test_column_empty_cards) {
    Column column("Empty Column", 5);
    
    json j = column.toJson();
    Column restored = Column::fromJson(j);
    
    assert(restored.getCardCount() == 0);
}

TEST(test_column_validation_missing_name) {
    json j = {
        {"wipLimit", 5}
    };
    
    try {
        Column::fromJson(j);
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Esperado
    }
}

// ============================================================================
// TESTES DE BOARD
// ============================================================================

TEST(test_board_serialization) {
    Board board("board1", "Project Board");
    Column col1("To Do", 5);
    Column col2("Done", 10);
    
    board.addColumn(col1);
    board.addColumn(col2);
    
    json j = board.toJson();
    
    assert(j["id"] == "board1");
    assert(j["name"] == "Project Board");
    assert(j["columns"].is_array());
    assert(j["columns"].size() == 2);
}

TEST(test_board_deserialization) {
    json j = {
        {"id", "board2"},
        {"name", "Sprint Board"},
        {"columns", json::array({
            {{"name", "Backlog"}, {"wipLimit", 0}},
            {{"name", "In Progress"}, {"wipLimit", 3}}
        })}
    };
    
    Board board = Board::fromJson(j);
    assert(board.getId() == "board2");
    assert(board.getName() == "Sprint Board");
    assert(board.getColumns().size() == 2);
}

TEST(test_board_complete_hierarchy) {
    // Cria board completo
    Board original("board3", "Full Test Board");
    
    Column col1("To Do", 5);
    Card card1("c7", "Task A");
    card1.setDescription("Description A");
    col1.addCard(card1);
    
    Column col2("In Progress", 3);
    Card card2("c8", "Task B");
    card2.setPriority(1);
    col2.addCard(card2);
    
    original.addColumn(col1);
    original.addColumn(col2);
    
    // Serializa e desserializa
    json j = original.toJson();
    Board restored = Board::fromJson(j);
    
    // Verifica estrutura
    assert(restored.getId() == "board3");
    assert(restored.getName() == "Full Test Board");
    assert(restored.getColumns().size() == 2);
    
    // Verifica primeira coluna
    const Column* restoredCol1 = restored.findColumn("To Do");
    assert(restoredCol1 != nullptr);
    assert(restoredCol1->getCardCount() == 1);
    
    // Verifica segunda coluna
    const Column* restoredCol2 = restored.findColumn("In Progress");
    assert(restoredCol2 != nullptr);
    assert(restoredCol2->getCardCount() == 1);
}

TEST(test_board_validation_empty_id) {
    json j = {
        {"id", ""},
        {"name", "Test Board"}
    };
    
    try {
        Board::fromJson(j);
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Esperado
    }
}

// ============================================================================
// TESTES DE ACTIVITY LOG
// ============================================================================

TEST(test_activitylog_serialization) {
    ActivityLog log;
    log.record("Event 1");
    log.record("Event 2");
    
    json j = log.toJson();
    
    assert(j.contains("entries"));
    assert(j["entries"].is_array());
    assert(j["entries"].size() == 2);
}

TEST(test_activitylog_deserialization) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
    
    json j = {
        {"entries", json::array({
            {{"timestamp", ms}, {"message", "Log entry 1"}},
            {{"timestamp", ms + 1000}, {"message", "Log entry 2"}}
        })}
    };
    
    ActivityLog log = ActivityLog::fromJson(j);
    auto entries = log.all();
    
    assert(entries.size() == 2);
    assert(entries[0].message == "Log entry 1");
    assert(entries[1].message == "Log entry 2");
}

TEST(test_activitylog_roundtrip) {
    ActivityLog original;
    original.record("First event");
    original.record("Second event");
    original.record("Third event");
    
    json j = original.toJson();
    ActivityLog restored = ActivityLog::fromJson(j);
    
    auto originalEntries = original.all();
    auto restoredEntries = restored.all();
    
    assert(originalEntries.size() == restoredEntries.size());
    for (size_t i = 0; i < originalEntries.size(); ++i) {
        assert(originalEntries[i].message == restoredEntries[i].message);
    }
}

TEST(test_activitylog_empty) {
    ActivityLog log;
    
    json j = log.toJson();
    ActivityLog restored = ActivityLog::fromJson(j);
    
    assert(restored.all().empty());
}

// ============================================================================
// TESTES DE INTEGRAÇÃO
// ============================================================================

TEST(test_complete_system_persistence) {
    // Cria sistema completo
    User user1("u1", "Alice", "alice@test.com");
    User user2("u2", "Bob", "bob@test.com");
    
    Board board("main", "Main Board");
    ActivityLog log;
    
    Column todo("To Do", 10);
    Card task1("t1", "Implement feature");
    task1.setDescription("Feature details");
    task1.setPriority(1);
    todo.addCard(task1);
    
    Column inProgress("In Progress", 5);
    Card task2("t2", "Fix critical bug");
    task2.setPriority(0);
    inProgress.addCard(task2);
    
    board.addColumn(todo);
    board.addColumn(inProgress);
    
    log.record("System initialized");
    log.record("Board created");
    
    // Serializa tudo
    json state = {
        {"users", json::array({user1.toJson(), user2.toJson()})},
        {"board", board.toJson()},
        {"log", log.toJson()}
    };
    
    // Desserializa
    User restoredUser1 = User::fromJson(state["users"][0]);
    User restoredUser2 = User::fromJson(state["users"][1]);
    Board restoredBoard = Board::fromJson(state["board"]);
    ActivityLog restoredLog = ActivityLog::fromJson(state["log"]);
    
    // Verifica integridade
    assert(restoredUser1.getName() == "Alice");
    assert(restoredUser2.getName() == "Bob");
    assert(restoredBoard.getColumns().size() == 2);
    assert(restoredLog.all().size() == 2);
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n=== KANBAN-LITE PERSISTENCE TESTS ===\n\n";
    
    // Testes são executados automaticamente via construtores estáticos
    
    std::cout << "\n=== RESULTS ===\n";
    std::cout << "Tests run: " << tests_run << "\n";
    std::cout << "Tests passed: " << tests_passed << "\n";
    std::cout << "Tests failed: " << (tests_run - tests_passed) << "\n";
    
    if (tests_passed == tests_run) {
        std::cout << "\n✓ ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "\n✗ SOME TESTS FAILED\n";
        return 1;
    }
}
