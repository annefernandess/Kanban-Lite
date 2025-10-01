#include "Board.h"
#include "Column.h"
#include "Card.h"
#include "ActivityLog.h"
#include <algorithm>

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