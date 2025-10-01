#include "Board.h"
#include "Column.h"
#include "Card.h"
#include "User.h"
#include "ActivityLog.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <algorithm>

/**
 * @file main.cpp
 * @brief CLI interface for Kanban-Lite system
 * @author Anne Fernandes da Costa Oliveira
 * @date 01/10/2025
 */

class KanbanCLI {
private:
    std::vector<std::unique_ptr<Board>> boards;
    std::vector<std::unique_ptr<User>> users;
    std::unique_ptr<ActivityLog> activityLog;
    static int cardIdCounter; // Contador estático para IDs únicos
    
public:
    KanbanCLI() : activityLog(std::make_unique<ActivityLog>()) {
        // Cria usuário padrão para demonstração
        users.push_back(std::make_unique<User>("default", "Default User", "user@example.com"));
    }
    
    void showUsage() {
        std::cout << "Kanban-Lite CLI - Etapa 2\n";
        std::cout << "Usage: kanban_cli [command] [args...]\n\n";
        std::cout << "Commands:\n";
        std::cout << "  board create <name>                    - Create new board\n";
        std::cout << "  board list                             - List all boards\n";
        std::cout << "  board remove <name>                    - Remove board\n";
        std::cout << "  column add <board> <name> [wip_limit]  - Add column to board\n";
        std::cout << "  column list <board>                    - List columns in board\n";
        std::cout << "  column remove <board> <name>           - Remove column from board\n";
        std::cout << "  card add <board> <column> <title>      - Add card to column\n";
        std::cout << "  card list <board> [column]             - List cards in board/column\n";
        std::cout << "  card move <card_id> <from_col> <to_col> <board> - Move card between columns\n";
        std::cout << "  card remove <board> <column> <card_id> - Remove card\n";
        std::cout << "  history                                - Show activity history\n";
        std::cout << "  save <filename>                        - Save state to file\n";
        std::cout << "  load <filename>                        - Load state from file\n";
        std::cout << "  help                                   - Show this help\n";
        std::cout << "  exit                                   - Exit interactive mode\n\n";
        std::cout << "Interactive mode: Run without arguments\n";
    }
    
    bool processCommand(const std::vector<std::string>& args) {
        if (args.empty()) {
            return false;
        }
        
        const std::string& command = args[0];
        
        if (command == "help") {
            showUsage();
            return true;
        }
        
        if (command == "exit") {
            return false;
        }
        
        if (command == "history") {
            showHistory();
            return true;
        }
        
        if (command == "save" && args.size() == 2) {
            saveState(args[1]);
            return true;
        }
        
        if (command == "load" && args.size() == 2) {
            loadState(args[1]);
            return true;
        }
        
        if (command == "board") {
            return processBoardCommand(args);
        }
        
        if (command == "column") {
            return processColumnCommand(args);
        }
        
        if (command == "card") {
            return processCardCommand(args);
        }
        
        std::cerr << "Error: Unknown command '" << command << "'. Use 'help' for usage.\n";
        return true;
    }
    
private:
    bool processBoardCommand(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            std::cerr << "Error: Board command requires subcommand\n";
            return true;
        }
        
        const std::string& subcommand = args[1];
        
        if (subcommand == "create" && args.size() == 3) {
            const std::string& name = args[2];
            auto board = std::make_unique<Board>(name, name);
            board->attachActivityLog(activityLog.get());
            boards.push_back(std::move(board));
            std::cout << "Board '" << name << "' created successfully.\n";
            return true;
        }
        
        if (subcommand == "list" && args.size() == 2) {
            listBoards();
            return true;
        }
        
        if (subcommand == "remove" && args.size() == 3) {
            const std::string& name = args[2];
            auto it = std::find_if(boards.begin(), boards.end(),
                [&name](const std::unique_ptr<Board>& board) {
                    return board->getName() == name;
                });
            
            if (it != boards.end()) {
                boards.erase(it);
                std::cout << "Board '" << name << "' removed successfully.\n";
            } else {
                std::cerr << "Error: Board '" << name << "' not found.\n";
            }
            return true;
        }
        
        std::cerr << "Error: Invalid board command. Use 'help' for usage.\n";
        return true;
    }
    
    bool processColumnCommand(const std::vector<std::string>& args) {
        if (args.size() < 3) {
            std::cerr << "Error: Column command requires more arguments\n";
            return true;
        }
        
        const std::string& subcommand = args[1];
        
        if (subcommand == "add" && args.size() >= 4) {
            const std::string& boardName = args[2];
            const std::string& columnName = args[3];
            int wipLimit = (args.size() == 5) ? std::stoi(args[4]) : -1;
            
            Board* board = findBoard(boardName);
            if (!board) {
                std::cerr << "Error: Board '" << boardName << "' not found.\n";
                return true;
            }
            
            Column column(columnName, wipLimit);
            if (board->addColumn(column)) {
                std::cout << "Column '" << columnName << "' added to board '" << boardName << "'";
                if (wipLimit != -1) {
                    std::cout << " with WIP limit " << wipLimit;
                }
                std::cout << ".\n";
            } else {
                std::cerr << "Error: Column '" << columnName << "' already exists in board.\n";
            }
            return true;
        }
        
        if (subcommand == "list" && args.size() == 3) {
            const std::string& boardName = args[2];
            listColumns(boardName);
            return true;
        }
        
        if (subcommand == "remove" && args.size() == 4) {
            const std::string& boardName = args[2];
            const std::string& columnName = args[3];
            
            Board* board = findBoard(boardName);
            if (!board) {
                std::cerr << "Error: Board '" << boardName << "' not found.\n";
                return true;
            }
            
            if (board->removeColumn(columnName)) {
                std::cout << "Column '" << columnName << "' removed from board '" << boardName << "'.\n";
            } else {
                std::cerr << "Error: Column '" << columnName << "' not found in board.\n";
            }
            return true;
        }
        
        std::cerr << "Error: Invalid column command. Use 'help' for usage.\n";
        return true;
    }
    
    bool processCardCommand(const std::vector<std::string>& args) {
        if (args.size() < 3) {
            std::cerr << "Error: Card command requires more arguments\n";
            return true;
        }
        
        const std::string& subcommand = args[1];
        
        if (subcommand == "add" && args.size() == 5) {
            const std::string& boardName = args[2];
            const std::string& columnName = args[3];
            const std::string& title = args[4];
            
            Board* board = findBoard(boardName);
            if (!board) {
                std::cerr << "Error: Board '" << boardName << "' not found.\n";
                return true;
            }
            
            // Gera ID único simples para o card
            std::string cardId = "card_" + std::to_string(++cardIdCounter);
            Card card(cardId, title);
            card.setAssignee(users[0].get()); // Atribui ao usuário padrão
            
            if (board->addCard(columnName, card)) {
                std::cout << "Card '" << title << "' (ID: " << cardId << ") added to column '" 
                         << columnName << "' in board '" << boardName << "'.\n";
            } else {
                std::cerr << "Error: Could not add card. Check if column exists and is not full.\n";
            }
            return true;
        }
        
        if (subcommand == "list" && (args.size() == 3 || args.size() == 4)) {
            const std::string& boardName = args[2];
            std::string columnName = (args.size() == 4) ? args[3] : "";
            listCards(boardName, columnName);
            return true;
        }
        
        if (subcommand == "move" && args.size() == 6) {
            const std::string& cardId = args[2];
            const std::string& fromCol = args[3];
            const std::string& toCol = args[4];
            const std::string& boardName = args[5];
            
            Board* board = findBoard(boardName);
            if (!board) {
                std::cerr << "Error: Board '" << boardName << "' not found.\n";
                return true;
            }
            
            if (board->moveCard(cardId, fromCol, toCol)) {
                std::cout << "Card '" << cardId << "' moved from '" << fromCol 
                         << "' to '" << toCol << "'.\n";
            } else {
                std::cerr << "Error: Could not move card. Check card ID and column names.\n";
            }
            return true;
        }
        
        if (subcommand == "remove" && args.size() == 5) {
            const std::string& boardName = args[2];
            const std::string& columnName = args[3];
            const std::string& cardId = args[4];
            
            Board* board = findBoard(boardName);
            if (!board) {
                std::cerr << "Error: Board '" << boardName << "' not found.\n";
                return true;
            }
            
            Column* column = board->findColumn(columnName);
            if (!column) {
                std::cerr << "Error: Column '" << columnName << "' not found.\n";
                return true;
            }
            
            if (column->removeCard(cardId)) {
                std::cout << "Card '" << cardId << "' removed from column '" << columnName << "'.\n";
                if (activityLog) {
                    activityLog->record("Card '" + cardId + "' removed from column '" + columnName + "' in board '" + boardName + "'");
                }
            } else {
                std::cerr << "Error: Card '" << cardId << "' not found in column.\n";
            }
            return true;
        }
        
        std::cerr << "Error: Invalid card command. Use 'help' for usage.\n";
        return true;
    }
    
    Board* findBoard(const std::string& name) {
        auto it = std::find_if(boards.begin(), boards.end(),
            [&name](const std::unique_ptr<Board>& board) {
                return board->getName() == name;
            });
        return (it != boards.end()) ? it->get() : nullptr;
    }
    
    void listBoards() {
        if (boards.empty()) {
            std::cout << "No boards found.\n";
            return;
        }
        
        std::cout << "Boards:\n";
        for (const auto& board : boards) {
            std::cout << "  - " << board->getName() << " (ID: " << board->getId() 
                     << ", Columns: " << board->getColumns().size() << ")\n";
        }
    }
    
    void listColumns(const std::string& boardName) {
        Board* board = findBoard(boardName);
        if (!board) {
            std::cerr << "Error: Board '" << boardName << "' not found.\n";
            return;
        }
        
        const auto& columns = board->getColumns();
        if (columns.empty()) {
            std::cout << "No columns found in board '" << boardName << "'.\n";
            return;
        }
        
        std::cout << "Columns in board '" << boardName << "':\n";
        for (const auto& column : columns) {
            std::cout << "  - " << column.getName() << " (Cards: " << column.getCardCount();
            if (column.getWipLimit() != -1) {
                std::cout << "/" << column.getWipLimit();
            }
            std::cout << ")\n";
        }
    }
    
    void listCards(const std::string& boardName, const std::string& columnName = "") {
        Board* board = findBoard(boardName);
        if (!board) {
            std::cerr << "Error: Board '" << boardName << "' not found.\n";
            return;
        }
        
        const auto& columns = board->getColumns();
        
        if (!columnName.empty()) {
            // Lista cards de uma coluna específica
            const Column* column = board->findColumn(columnName);
            if (!column) {
                std::cerr << "Error: Column '" << columnName << "' not found.\n";
                return;
            }
            
            const auto& cards = column->getCards();
            if (cards.empty()) {
                std::cout << "No cards found in column '" << columnName << "'.\n";
                return;
            }
            
            std::cout << "Cards in column '" << columnName << "':\n";
            for (const auto& card : cards) {
                std::cout << "  - " << card.getTitle() << " (ID: " << card.getId() 
                         << ", Priority: " << card.getPriority() << ")\n";
            }
        } else {
            // Lista cards de todas as colunas
            std::cout << "All cards in board '" << boardName << "':\n";
            for (const auto& column : columns) {
                std::cout << "\n  Column: " << column.getName() << "\n";
                const auto& cards = column.getCards();
                if (cards.empty()) {
                    std::cout << "    (no cards)\n";
                } else {
                    for (const auto& card : cards) {
                        std::cout << "    - " << card.getTitle() << " (ID: " << card.getId() 
                                 << ", Priority: " << card.getPriority() << ")\n";
                    }
                }
            }
        }
    }
    
    void showHistory() {
        auto entries = activityLog->all();
        if (entries.empty()) {
            std::cout << "No activity recorded.\n";
            return;
        }
        
        std::cout << "Activity History:\n";
        for (const auto& entry : entries) {
            auto time = std::chrono::system_clock::to_time_t(entry.timestamp);
            std::cout << "  [" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") 
                     << "] " << entry.message << "\n";
        }
    }
    
    void saveState(const std::string& filename) {
        // Implementação básica de save - formato simples
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file '" << filename << "' for writing.\n";
            return;
        }
        
        // Salva boards em formato simples
        file << "# Kanban-Lite State File\n";
        file << "BOARDS=" << boards.size() << "\n";
        
        for (const auto& board : boards) {
            file << "BOARD_NAME=" << board->getName() << "\n";
            file << "BOARD_ID=" << board->getId() << "\n";
            file << "COLUMNS=" << board->getColumns().size() << "\n";
            
            for (const auto& column : board->getColumns()) {
                file << "COLUMN_NAME=" << column.getName() << "\n";
                file << "COLUMN_WIP=" << column.getWipLimit() << "\n";
                file << "CARDS=" << column.getCardCount() << "\n";
                
                for (const auto& card : column.getCards()) {
                    file << "CARD_ID=" << card.getId() << "\n";
                    file << "CARD_TITLE=" << card.getTitle() << "\n";
                    file << "CARD_DESC=" << card.getDescription() << "\n";
                    file << "CARD_PRIORITY=" << card.getPriority() << "\n";
                }
            }
        }
        
        file.close();
        std::cout << "State saved to '" << filename << "'.\n";
        
        if (activityLog) {
            activityLog->record("State saved to file '" + filename + "'");
        }
    }
    
    void loadState(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file '" << filename << "' for reading.\n";
            return;
        }
        
        // Limpa estado atual
        boards.clear();
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            // Implementação básica de parsing
            if (line.find("BOARD_NAME=") == 0) {
                std::string boardName = line.substr(11);
                auto board = std::make_unique<Board>(boardName, boardName);
                board->attachActivityLog(activityLog.get());
                boards.push_back(std::move(board));
            }
            // Adicionar mais parsing conforme necessário
        }
        
        file.close();
        std::cout << "State loaded from '" << filename << "'.\n";
        
        if (activityLog) {
            activityLog->record("State loaded from file '" + filename + "'");
        }
    }
    
public:
    void runInteractive() {
        std::cout << "Kanban-Lite CLI - Interactive Mode\n";
        std::cout << "Type 'help' for commands or 'exit' to quit.\n\n";
        
        std::string line;
        while (true) {
            std::cout << "kanban> ";
            if (!std::getline(std::cin, line)) {
                break; // EOF
            }
            
            if (line.empty()) continue;
            
            // Parse linha em argumentos
            std::vector<std::string> args;
            std::istringstream iss(line);
            std::string arg;
            while (iss >> arg) {
                args.push_back(arg);
            }
            
            if (!processCommand(args)) {
                break; // exit command
            }
        }
        
        std::cout << "Goodbye!\n";
    }
    
    int runCommand(int argc, char* argv[]) {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i) {
            args.push_back(argv[i]);
        }
        
        if (args.empty()) {
            runInteractive();
            return 0;
        }
        
        if (!processCommand(args)) {
            return 1;
        }
        
        return 0;
    }
};

// Definição do contador estático
int KanbanCLI::cardIdCounter = 0;

int main(int argc, char* argv[]) {
    try {
        KanbanCLI cli;
        return cli.runCommand(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
}