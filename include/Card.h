#ifndef KANBAN_LITE_CARD_H
#define KANBAN_LITE_CARD_H

#include <string>
#include <memory>
#include <chrono>

class User; // forward declaration

/**
 * @file Card.h
 * @brief Definição da classe Card para representar tarefas no sistema Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 25/09/2025
 */

/**
 * @class Card
 * @brief Representa uma tarefa/unidade de trabalho no sistema Kanban.
 * 
 * A classe Card encapsula todas as informações relacionadas a uma tarefa,
 * incluindo identificação, conteúdo, responsável, prioridade e timestamps
 * de auditoria. Implementa conceitos de POO como encapsulamento através
 * de métodos setters com validação futura.
 * 
 * @details
 * - Responsabilidade única: gerenciar dados e comportamento de uma tarefa
 * - Encapsulamento: atributos privados com acesso controlado via métodos
 * - Associação: referencia User sem gerenciar seu ciclo de vida
 * - Timestamps automáticos para auditoria (criação/modificação)
 */
class Card {
public:
    /**
     * @brief Construtor da classe Card.
     * 
     * Cria uma nova instância de Card com ID e título obrigatórios.
     * Os timestamps de criação e atualização são definidos automaticamente
     * para o momento atual.
     * 
     * @param id Identificador único do card (não pode ser vazio)
     * @param title Título da tarefa (não pode ser vazio)
     * 
     * @pre id não deve ser string vazia
     * @pre title não deve ser string vazia
     * @post Card criado com priority = 0, assignee = nullptr
     * @post createdAt e updatedAt definidos para momento atual
     */
    Card(std::string id, std::string title);

    /**
     * @brief Define o título do card.
     * 
     * Atualiza o título da tarefa e o timestamp de modificação.
     * Validação será implementada para garantir título não vazio.
     * 
     * @param t Novo título do card
     * @pre t não deve ser string vazia (validação futura)
     * @post updatedAt atualizado para momento atual
     * 
     * @todo Implementar validação para título não vazio
     */
    void setTitle(const std::string& t);

    /**
     * @brief Define a descrição detalhada do card.
     * 
     * Atualiza a descrição da tarefa. Descrição pode ser vazia.
     * 
     * @param d Nova descrição (pode ser vazia)
     * @post updatedAt atualizado para momento atual
     */
    void setDescription(const std::string& d);

    /**
     * @brief Atribui um responsável ao card.
     * 
     * Define o usuário responsável pela execução da tarefa.
     * O Card não gerencia o ciclo de vida do User (associação fraca).
     * 
     * @param u Ponteiro para o User responsável (pode ser nullptr)
     * @post assignee definido como u
     * @post updatedAt atualizado para momento atual
     * 
     * @note Card não possui o User - gerenciamento de memória externo
     */
    void setAssignee(User* u);

    /**
     * @brief Define a prioridade numérica do card.
     * 
     * Estabelece a prioridade da tarefa usando escala numérica.
     * Validação de faixa será implementada (ex: 0-5).
     * 
     * @param p Valor da prioridade
     * @pre p deve estar em faixa válida (validação futura: 0-5)
     * @post priority definido como p
     * @post updatedAt atualizado para momento atual
     * 
     * @todo Implementar validação de faixa (ex: 0..5)
     */
    void setPriority(int p);

    /**
     * @brief Operador de igualdade para comparação de cards.
     * 
     * Compara dois cards baseado em seus IDs únicos.
     * Permite uso em containers STL e algoritmos.
     * 
     * @param other Card a ser comparado
     * @return true se os IDs são iguais, false caso contrário
     * 
     * @note Comparação baseada apenas no ID (chave primária)
     * @note Método const - não modifica o objeto
     */
    bool operator==(const Card& other) const;

    // Getters para acesso aos dados
    const std::string& getId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    User* getAssignee() const;
    int getPriority() const;
    const std::chrono::system_clock::time_point& getCreatedAt() const;
    const std::chrono::system_clock::time_point& getUpdatedAt() const;

private:
    std::string m_id;                              /**< @brief Identificador único do card */
    std::string m_title;                           /**< @brief Título da tarefa */
    std::string m_description;                     /**< @brief Descrição detalhada (opcional) */
    User* m_assignee { nullptr };                 /**< @brief Ponteiro para usuário responsável (não owned) */
    int m_priority { 0 };                          /**< @brief Prioridade numérica (0=baixa, valores maiores=alta) */
    std::chrono::system_clock::time_point m_createdAt;   /**< @brief Timestamp de criação */
    std::chrono::system_clock::time_point m_updatedAt;   /**< @brief Timestamp da última modificação */
};

#endif // KANBAN_LITE_CARD_H
