#include "User.h"
#include <stdexcept>

using json = nlohmann::json;

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

/**
 * @brief Serializa o usuário para JSON.
 * @return Objeto JSON contendo dados do usuário
 */
json User::toJson() const {
    return json{
        {"id", m_id},
        {"name", m_name},
        {"email", m_email}
    };
}

/**
 * @brief Desserializa usuário a partir de JSON.
 * @param j Objeto JSON com dados do usuário
 * @return User reconstruído
 * @throws json::exception se campos obrigatórios ausentes
 * @throws std::invalid_argument se dados inválidos
 */
User User::fromJson(const json& j) {
    // Validação de campos obrigatórios
    if (!j.contains("id") || !j.contains("name") || !j.contains("email")) {
        throw std::invalid_argument("User JSON must contain id, name, and email fields");
    }

    // Validação de tipos
    if (!j["id"].is_string() || !j["name"].is_string() || !j["email"].is_string()) {
        throw std::invalid_argument("User fields must be strings");
    }

    // Extração com validação adicional
    std::string id = j["id"].get<std::string>();
    std::string name = j["name"].get<std::string>();
    std::string email = j["email"].get<std::string>();

    if (id.empty() || name.empty() || email.empty()) {
        throw std::invalid_argument("User fields cannot be empty");
    }

    // RAII: construção do objeto com move semantics
    return User(std::move(id), std::move(name), std::move(email));
}