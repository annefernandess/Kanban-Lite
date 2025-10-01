#include "Card.h"
#include "User.h"

/**
 * @file Card.cpp
 * @brief Implementação da classe Card para representar tarefas no sistema Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 01/10/2025
 */

/**
 * @brief Construtor da classe Card.
 * @param id Identificador único do cartão
 * @param title Título do cartão
 */
Card::Card(std::string id, std::string title)
    : m_id(std::move(id)), m_title(std::move(title))
{
    auto now = std::chrono::system_clock::now();
    m_createdAt = now;
    m_updatedAt = now;
}

/**
 * @brief Define o título do cartão e atualiza timestamp.
 * @param t Novo título do cartão
 */
void Card::setTitle(const std::string& t) {
    m_title = t;
    m_updatedAt = std::chrono::system_clock::now();
}

/**
 * @brief Define a descrição do cartão e atualiza timestamp.
 * @param d Nova descrição do cartão
 */
void Card::setDescription(const std::string& d) {
    m_description = d;
    m_updatedAt = std::chrono::system_clock::now();
}

/**
 * @brief Define o usuário responsável pelo cartão.
 * @param u Ponteiro para o usuário assignee (pode ser nullptr)
 */
void Card::setAssignee(User* u) {
    m_assignee = u;
    m_updatedAt = std::chrono::system_clock::now();
}

/**
 * @brief Define a prioridade do cartão.
 * @param p Valor da prioridade (normalmente 1-5)
 */
void Card::setPriority(int p) {
    m_priority = p;
    m_updatedAt = std::chrono::system_clock::now();
}

/**
 * @brief Operador de igualdade para comparar cartões.
 * @param other Outro cartão para comparação
 * @return true se os IDs são iguais, false caso contrário
 */
bool Card::operator==(const Card& other) const {
    return m_id == other.m_id;
}

/**
 * @brief Retorna o ID único do cartão.
 * @return Referência constante ao ID do cartão
 */
const std::string& Card::getId() const {
    return m_id;
}

/**
 * @brief Retorna o título do cartão.
 * @return Referência constante ao título do cartão
 */
const std::string& Card::getTitle() const {
    return m_title;
}

/**
 * @brief Retorna a descrição do cartão.
 * @return Referência constante à descrição do cartão
 */
const std::string& Card::getDescription() const {
    return m_description;
}

/**
 * @brief Retorna o usuário responsável pelo cartão.
 * @return Ponteiro para o usuário assignee (pode ser nullptr)
 */
User* Card::getAssignee() const {
    return m_assignee;
}

/**
 * @brief Retorna a prioridade do cartão.
 * @return Valor da prioridade
 */
int Card::getPriority() const {
    return m_priority;
}

/**
 * @brief Retorna o timestamp de criação do cartão.
 * @return Referência constante ao timestamp de criação
 */
const std::chrono::system_clock::time_point& Card::getCreatedAt() const {
    return m_createdAt;
}

/**
 * @brief Retorna o timestamp da última atualização.
 * @return Referência constante ao timestamp de atualização
 */
const std::chrono::system_clock::time_point& Card::getUpdatedAt() const {
    return m_updatedAt;
}