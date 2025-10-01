#include "User.h"

/**
 * @file User.cpp
 * @brief Implementação da classe User para representar participantes do sistema.
 * @author Anne Fernandes da Costa Oliveira
 * @date 01/10/2025
 */

/**
 * @brief Construtor da classe User com move semantics.
 * @param id Identificador único do usuário
 * @param name Nome completo do usuário
 * @param email Endereço de email do usuário
 */
User::User(std::string id, std::string name, std::string email)
    : m_id(std::move(id)), m_name(std::move(name)), m_email(std::move(email))
{
    // Construtor move-enabled para eficiência
}

/**
 * @brief Retorna o nome do usuário.
 * @return Referência constante ao nome do usuário
 */
const std::string& User::getName() const {
    return m_name;
}

/**
 * @brief Retorna o email do usuário.
 * @return Referência constante ao email do usuário
 */
const std::string& User::getEmail() const {
    return m_email;
}

/**
 * @brief Retorna o ID único do usuário.
 * @return Referência constante ao ID do usuário
 */
const std::string& User::getId() const {
    return m_id;
}

/**
 * @brief Operador de igualdade para comparar usuários.
 * @param other Outro usuário para comparação
 * @return true se os IDs são iguais, false caso contrário
 */
bool User::operator==(const User& other) const {
    return m_id == other.m_id;
}