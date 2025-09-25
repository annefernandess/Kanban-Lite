#ifndef KANBAN_LITE_USER_H
#define KANBAN_LITE_USER_H

#include <string>

/**
 * @file User.h
 * @brief Definição da classe User para representar participantes do sistema.
 * @author Anne Fernandes da Costa Oliveira
 * @date 25/09/2025
 */

/**
 * @class User
 * @brief Representa um usuário/participante do sistema Kanban.
 * 
 * A classe User encapsula informações de identificação de usuários
 * que podem ser criadores ou responsáveis (assignees) por cards.
 * Implementa conceitos de encapsulamento através de getters const.
 * 
 * @details
 * - Responsabilidade única: manter dados de identificação do usuário
 * - Encapsulamento: atributos privados com acesso somente leitura
 * - Imutabilidade: após criação, dados não podem ser alterados
 * - Associação: é referenciado por Card sem gerenciar ciclo de vida
 */
class User {
public:
    /**
     * @brief Construtor da classe User.
     * 
     * Cria novo usuário com identificador, nome e email obrigatórios.
     * Todos os parâmetros são copiados e armazenados internamente.
     * 
     * @param id Identificador único do usuário
     * @param name Nome completo do usuário
     * @param email Endereço de email do usuário
     * @pre id não deve ser string vazia
     * @pre name não deve ser string vazia
     * @pre email não deve ser string vazia
     * @post User criado com dados imutáveis
     */
    User(std::string id, std::string name, std::string email);

    /**
     * @brief Obtém nome do usuário.
     * 
     * Fornece acesso somente leitura ao nome do usuário.
     * 
     * @return Referência const para o nome
     * @note Método const - não modifica o objeto
     */
    const std::string& getName() const;

    /**
     * @brief Obtém email do usuário.
     * 
     * Fornece acesso somente leitura ao email do usuário.
     * 
     * @return Referência const para o email
     * @note Método const - não modifica o objeto
     */
    const std::string& getEmail() const;

    /**
     * @brief Obtém ID do usuário.
     * 
     * Fornece acesso somente leitura ao identificador único.
     * 
     * @return Referência const para o ID
     * @note Método const - não modifica o objeto
     */
    const std::string& getId() const;

    /**
     * @brief Operador de igualdade para comparação de usuários.
     * 
     * Compara dois usuários baseado em seus IDs únicos.
     * 
     * @param other Usuário a ser comparado
     * @return true se os IDs são iguais, false caso contrário
     * @note Comparação baseada apenas no ID (chave primária)
     */
    bool operator==(const User& other) const;

private:
    std::string m_id;                              /**< @brief Identificador único do usuário */
    std::string m_name;                            /**< @brief Nome completo do usuário */
    std::string m_email;                           /**< @brief Endereço de email do usuário */
};

#endif // KANBAN_LITE_USER_H
