#include "Card.h"
#include "User.h"
#include <stdexcept>
#include <algorithm>

using json = nlohmann::json;

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
    : m_id(std::move(id))
    , m_title(std::move(title))
    , m_tags() // Inicializa explicitamente como vector vazio
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

/**
 * @brief Adiciona uma tag ao card.
 * @param tag Nome da tag a adicionar
 */
void Card::addTag(const std::string& tag) {
    // Adiciona apenas se não existir
    if (std::find(m_tags.begin(), m_tags.end(), tag) == m_tags.end()) {
        m_tags.push_back(tag);
        m_updatedAt = std::chrono::system_clock::now();
    }
}

/**
 * @brief Remove uma tag do card.
 * @param tag Nome da tag a remover
 */
void Card::removeTag(const std::string& tag) {
    auto it = std::find(m_tags.begin(), m_tags.end(), tag);
    if (it != m_tags.end()) {
        m_tags.erase(it);
        m_updatedAt = std::chrono::system_clock::now();
    }
}

/**
 * @brief Verifica se o card possui determinada tag.
 * @param tag Nome da tag a verificar
 * @return true se possui a tag, false caso contrário
 */
bool Card::hasTag(const std::string& tag) const {
    return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
}

/**
 * @brief Retorna todas as tags do card.
 * @return Vector com todas as tags
 */
const std::vector<std::string>& Card::getTags() const {
    return m_tags;
}

/**
 * @brief Serializa o card para JSON.
 * @return Objeto JSON com todos os dados do card
 */
json Card::toJson() const {
    // Converte timestamps para epoch milliseconds
    auto createdMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_createdAt.time_since_epoch()
    ).count();
    
    auto updatedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_updatedAt.time_since_epoch()
    ).count();

    json j = {
        {"id", m_id},
        {"title", m_title},
        {"description", m_description},
        {"priority", m_priority},
        {"createdAt", createdMs},
        {"updatedAt", updatedMs},
        {"tags", m_tags}  // Serializa tags como array JSON
    };

    // Adiciona assignee ID se existir
    if (m_assignee != nullptr) {
        j["assigneeId"] = m_assignee->getId();
    } else {
        j["assigneeId"] = nullptr;
    }

    return j;
}

/**
 * @brief Desserializa card a partir de JSON.
 * @param j Objeto JSON com dados do card
 * @return Card reconstruído
 * @throws json::exception se campos obrigatórios ausentes
 * @throws std::invalid_argument se dados inválidos
 */
Card Card::fromJson(const json& j) {
    // Validação de campos obrigatórios
    if (!j.contains("id") || !j.contains("title")) {
        throw std::invalid_argument("Card JSON must contain id and title fields");
    }

    // Extração de campos obrigatórios
    std::string id = j["id"].get<std::string>();
    std::string title = j["title"].get<std::string>();

    if (id.empty() || title.empty()) {
        throw std::invalid_argument("Card id and title cannot be empty");
    }

    // RAII: construção do card com move semantics
    Card card(std::move(id), std::move(title));

    // Campos opcionais
    if (j.contains("description") && j["description"].is_string()) {
        card.setDescription(j["description"].get<std::string>());
    }

    if (j.contains("priority") && j["priority"].is_number_integer()) {
        card.setPriority(j["priority"].get<int>());
    }

    // Reconstrução de timestamps
    if (j.contains("createdAt") && j["createdAt"].is_number()) {
        auto ms = std::chrono::milliseconds(j["createdAt"].get<long long>());
        card.m_createdAt = std::chrono::system_clock::time_point(ms);
    }

    if (j.contains("updatedAt") && j["updatedAt"].is_number()) {
        auto ms = std::chrono::milliseconds(j["updatedAt"].get<long long>());
        card.m_updatedAt = std::chrono::system_clock::time_point(ms);
    }

    // Carrega tags se existirem
    if (j.contains("tags") && j["tags"].is_array()) {
        for (const auto& tag : j["tags"]) {
            if (tag.is_string()) {
                card.m_tags.push_back(tag.get<std::string>());
            }
        }
    }

    // Nota: assignee deve ser resolvido pela aplicação após carregamento
    // pois precisamos do mapa de users completo

    return card;
}