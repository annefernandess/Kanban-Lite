#include "Board.h"
#include "Column.h"
#include "Card.h"
#include "ActivityLog.h"
#include "external/json.hpp"
#include <algorithm>
#include <stdexcept>

using json = nlohmann::json;

/**
 * @file Board.cpp
 * @brief Implementação da classe Board para gerenciar quadros Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 01/10/2025
 */

/**
 * @brief Construtor da classe Board.
 * @param id Identificador único do quadro
 * @param name Nome do quadro
 */
Board::Board(std::string id, std::string name)
    : m_id(std::move(id)), m_name(std::move(name))
{
}

/**
 * @brief Adiciona uma nova coluna ao quadro.
 * @param column Coluna a ser adicionada
 * @return true se adicionada com sucesso, false se nome já existe
 */
bool Board::addColumn(const Column& column) {
    // Verifica se já existe coluna com o mesmo nome
    auto it = std::find_if(m_columns.begin(), m_columns.end(),
        [&column](const Column& existing) { return existing.getName() == column.getName(); });
    
    if (it != m_columns.end()) {
        return false; // Nome já existe
    }
    
    m_columns.push_back(column);
    
    // Log da operação se ActivityLog disponível
    if (m_activityLog) {
        m_activityLog->record("Column '" + column.getName() + "' added to board '" + m_name + "'");
    }
    
    return true;
}

/**
 * @brief Remove uma coluna do quadro pelo nome.
 * @param name Nome da coluna a ser removida
 * @return true se removida com sucesso, false se não encontrada
 */
bool Board::removeColumn(const std::string& name) {
    auto it = std::find_if(m_columns.begin(), m_columns.end(),
        [&name](const Column& column) { return column.getName() == name; });
    
    if (it != m_columns.end()) {
        if (m_activityLog) {
            m_activityLog->record("Column '" + name + "' removed from board '" + m_name + "'");
        }
        m_columns.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief Busca uma coluna pelo nome (versão não-const).
 * @param name Nome da coluna a ser encontrada
 * @return Ponteiro para a coluna ou nullptr se não encontrada
 */
Column* Board::findColumn(const std::string& name) {
    auto it = std::find_if(m_columns.begin(), m_columns.end(),
        [&name](const Column& column) { return column.getName() == name; });
    
    return (it != m_columns.end()) ? &(*it) : nullptr;
}

/**
 * @brief Busca uma coluna pelo nome (versão const).
 * @param name Nome da coluna a ser encontrada
 * @return Ponteiro constante para a coluna ou nullptr se não encontrada
 */
const Column* Board::findColumn(const std::string& name) const {
    auto it = std::find_if(m_columns.begin(), m_columns.end(),
        [&name](const Column& column) { return column.getName() == name; });
    
    return (it != m_columns.end()) ? &(*it) : nullptr;
}

/**
 * @brief Adiciona um cartão a uma coluna específica do quadro.
 * @param columnName Nome da coluna onde adicionar o cartão
 * @param card Cartão a ser adicionado
 * @return true se adicionado com sucesso, false caso contrário
 */
bool Board::addCard(const std::string& columnName, const Card& card) {
    Column* column = findColumn(columnName);
    if (!column) {
        return false; // Coluna não encontrada
    }
    
    bool result = column->addCard(card);
    if (result && m_activityLog) {
        m_activityLog->record("Card '" + card.getId() + "' added to column '" + columnName + "' in board '" + m_name + "'");
    }
    
    return result;
}

/**
 * @brief Move um cartão entre colunas respeitando limites WIP.
 * @param cardId ID do cartão a ser movido
 * @param fromCol Nome da coluna origem
 * @param toCol Nome da coluna destino
 * @return true se movido com sucesso, false caso contrário
 */
bool Board::moveCard(const std::string& cardId, const std::string& fromCol, const std::string& toCol) {
    Column* fromColumn = findColumn(fromCol);
    Column* toColumn = findColumn(toCol);
    
    if (!fromColumn || !toColumn) {
        return false; // Uma das colunas não existe
    }
    
    // Procura o card na coluna origem
    const Card* card = fromColumn->findCard(cardId);
    if (!card) {
        return false; // Card não encontrado na coluna origem
    }
    
    // Verifica se a coluna destino pode receber o card (WIP limit)
    if (toColumn->isFull()) {
        return false; // Coluna destino cheia
    }
    
    // Faz uma cópia do card antes de remover
    Card cardCopy = *card;
    
    // Remove da origem e adiciona ao destino
    if (fromColumn->removeCard(cardId) && toColumn->addCard(cardCopy)) {
        if (m_activityLog) {
            m_activityLog->record("Card '" + cardId + "' moved from '" + fromCol + "' to '" + toCol + "' in board '" + m_name + "'");
        }
        return true;
    }
    
    return false;
}

/**
 * @brief Anexa um ActivityLog ao quadro para registro de eventos.
 * @param log Ponteiro para o ActivityLog (dependency injection)
 */
void Board::attachActivityLog(ActivityLog* log) {
    m_activityLog = log;
}

/**
 * @brief Retorna o ID único do quadro.
 * @return Referência constante ao ID do quadro
 */
const std::string& Board::getId() const {
    return m_id;
}

/**
 * @brief Retorna o nome do quadro.
 * @return Referência constante ao nome do quadro
 */
const std::string& Board::getName() const {
    return m_name;
}

/**
 * @brief Retorna todas as colunas do quadro.
 * @return Referência constante ao vetor de colunas
 */
const std::vector<Column>& Board::getColumns() const {
    return m_columns;
}

/**
 * @brief Retorna todas as colunas do quadro (versão não-const).
 * @return Referência ao vetor de colunas
 */
std::vector<Column>& Board::getColumns() {
    return m_columns;
}

/**
 * @brief Busca todos os cards que possuem uma etiqueta específica.
 * @param tag Etiqueta a ser buscada
 * @return Vector de ponteiros para cards que possuem a etiqueta
 */
std::vector<Card*> Board::findCardsByTag(const std::string& tag) {
    std::vector<Card*> result;
    
    for (auto& column : m_columns) {
        for (auto& card : column.getCards()) {
            if (card.hasTag(tag)) {
                result.push_back(&card);
            }
        }
    }
    
    return result;
}

/**
 * @brief Filtra cards por prioridade mínima.
 * @param minPriority Prioridade mínima (inclusive)
 * @return Vector de ponteiros para cards com prioridade >= minPriority
 */
std::vector<Card*> Board::filterByPriority(int minPriority) {
    std::vector<Card*> result;
    
    for (auto& column : m_columns) {
        for (auto& card : column.getCards()) {
            if (card.getPriority() >= minPriority) {
                result.push_back(&card);
            }
        }
    }
    
    return result;
}

/**
 * @brief Filtra cards por usuário responsável.
 * @param user Ponteiro para o usuário
 * @return Vector de ponteiros para cards atribuídos ao usuário
 */
std::vector<Card*> Board::filterByAssignee(User* user) {
    std::vector<Card*> result;
    
    if (!user) {
        return result; // Se user é nullptr, retorna vazio
    }
    
    for (auto& column : m_columns) {
        for (auto& card : column.getCards()) {
            if (card.getAssignee() == user) {
                result.push_back(&card);
            }
        }
    }
    
    return result;
}

/**
 * @brief Coleta todas as etiquetas únicas de todos os cards do board.
 * @return Vector de strings com todas as etiquetas em uso
 */
std::vector<std::string> Board::getAllTags() const {
    std::vector<std::string> allTags;
    
    // Coleta todas as tags de todos os cards
    for (const auto& column : m_columns) {
        for (const auto& card : column.getCards()) {
            const auto& cardTags = card.getTags();
            allTags.insert(allTags.end(), cardTags.begin(), cardTags.end());
        }
    }
    
    // Remove duplicatas
    std::sort(allTags.begin(), allTags.end());
    auto last = std::unique(allTags.begin(), allTags.end());
    allTags.erase(last, allTags.end());
    
    return allTags;
}

/**
 * @brief Serializa o board para JSON.
 * @return Objeto JSON contendo id, name e array de colunas
 */
json Board::toJson() const {
    json columnsArray = json::array();
    
    for (const auto& column : m_columns) {
        columnsArray.push_back(column.toJson());
    }
    
    return json{
        {"id", m_id},
        {"name", m_name},
        {"columns", columnsArray}
    };
}

/**
 * @brief Desserializa board a partir de JSON.
 * @param j Objeto JSON com dados do board
 * @return Board reconstruído com todas as colunas e cards
 * @throws std::invalid_argument se campos obrigatórios ausentes ou inválidos
 * @throws json::exception se estrutura JSON malformada
 */
Board Board::fromJson(const json& j) {
    // Validação dos campos obrigatórios
    if (!j.contains("id")) {
        throw std::invalid_argument("Board JSON missing required field: id");
    }
    if (!j.contains("name")) {
        throw std::invalid_argument("Board JSON missing required field: name");
    }
    
    std::string id = j["id"].get<std::string>();
    std::string name = j["name"].get<std::string>();
    
    if (id.empty()) {
        throw std::invalid_argument("Board id cannot be empty");
    }
    if (name.empty()) {
        throw std::invalid_argument("Board name cannot be empty");
    }
    
    // Construção do board com RAII (move semantics)
    Board board(std::move(id), std::move(name));
    
    // Desserialização das colunas (se presente)
    if (j.contains("columns") && j["columns"].is_array()) {
        for (const auto& columnJson : j["columns"]) {
            try {
                Column column = Column::fromJson(columnJson);
                board.m_columns.push_back(std::move(column));
            } catch (const std::exception& e) {
                // Robustez: continua carregando outras colunas mesmo se uma falhar
                // Em produção, poderia logar erro mas não falha completamente
            }
        }
    }
    
    return board;
}