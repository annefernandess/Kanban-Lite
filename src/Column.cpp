#include "Column.h"
#include "Card.h"
#include <algorithm>

/**
 * @file Column.cpp
 * @brief Implementação da classe Column para organizar cartões no sistema Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 01/10/2025
 */

/**
 * @brief Construtor da classe Column.
 * @param name Nome da coluna
 * @param wipLimit Limite WIP (Work In Progress), -1 para sem limite
 */
Column::Column(std::string name, int wipLimit)
    : m_name(std::move(name)), m_wipLimit(wipLimit)
{
}

/**
 * @brief Adiciona um cartão à coluna respeitando limite WIP.
 * @param card Cartão a ser adicionado
 * @return true se adicionado com sucesso, false se limite WIP atingido
 */
bool Column::addCard(const Card& card) {
    // Verifica limite WIP se ativo (-1 significa sem limite)
    if (m_wipLimit != -1 && static_cast<int>(m_cards.size()) >= m_wipLimit) {
        return false; // Limite atingido
    }
    
    m_cards.push_back(card);
    return true;
}

/**
 * @brief Remove um cartão da coluna pelo ID.
 * @param cardId ID do cartão a ser removido
 * @return true se removido com sucesso, false se não encontrado
 */
bool Column::removeCard(const std::string& cardId) {
    auto it = std::find_if(m_cards.begin(), m_cards.end(),
        [&cardId](const Card& card) { return card.getId() == cardId; });
    
    if (it != m_cards.end()) {
        m_cards.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief Busca um cartão pelo ID (versão não-const).
 * @param cardId ID do cartão a ser encontrado
 * @return Ponteiro para o cartão ou nullptr se não encontrado
 */
Card* Column::findCard(const std::string& cardId) {
    auto it = std::find_if(m_cards.begin(), m_cards.end(),
        [&cardId](const Card& card) { return card.getId() == cardId; });
    
    return (it != m_cards.end()) ? &(*it) : nullptr;
}

/**
 * @brief Busca um cartão pelo ID (versão const).
 * @param cardId ID do cartão a ser encontrado
 * @return Ponteiro constante para o cartão ou nullptr se não encontrado
 */
const Card* Column::findCard(const std::string& cardId) const {
    auto it = std::find_if(m_cards.begin(), m_cards.end(),
        [&cardId](const Card& card) { return card.getId() == cardId; });
    
    return (it != m_cards.end()) ? &(*it) : nullptr;
}

/**
 * @brief Verifica se a coluna atingiu o limite WIP.
 * @return true se a coluna está cheia, false caso contrário
 */
bool Column::isFull() const {
    if (m_wipLimit == -1) {
        return false; // Sem limite
    }
    return static_cast<int>(m_cards.size()) >= m_wipLimit;
}

/**
 * @brief Retorna todos os cartões da coluna.
 * @return Referência constante ao vetor de cartões
 */
const std::vector<Card>& Column::getCards() const {
    return m_cards;
}

/**
 * @brief Retorna o nome da coluna.
 * @return Referência constante ao nome da coluna
 */
const std::string& Column::getName() const {
    return m_name;
}

/**
 * @brief Retorna o limite WIP da coluna.
 * @return Limite WIP (-1 para sem limite)
 */
int Column::getWipLimit() const {
    return m_wipLimit;
}

size_t Column::getCardCount() const {
    return m_cards.size();
}

bool Column::operator==(const Column& other) const {
    return m_name == other.m_name;
}