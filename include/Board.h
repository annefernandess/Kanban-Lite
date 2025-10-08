#ifndef KANBAN_LITE_BOARD_H
#define KANBAN_LITE_BOARD_H

#include <string>
#include <vector>
#include <memory>
#include "external/json.hpp"

// Forward declarations
class Column;
class Card;
class ActivityLog;
class User;

/**
 * @file Board.h
 * @brief Definição da classe Board para gerenciamento de quadros Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 25/09/2025
 */

/**
 * @class Board
 * @brief Representa um quadro Kanban que agrega e gerencia colunas.
 * 
 * A classe Board é o agregador principal do sistema, responsável por
 * coordenar operações de alto nível sobre colunas e cards. Implementa
 * padrão de composição para gerenciar colunas e delegação para operações
 * específicas de cards.
 * 
 * @details
 * - Responsabilidade única: agregar colunas e coordenar operações de alto nível
 * - Encapsulamento: atributos privados com acesso controlado via métodos
 * - Coesão: foca apenas em gerenciar colunas e delegar operações de cartão
 * - Baixo acoplamento: interage com Column através de interface pública
 * - Composição: Board possui e gerencia o ciclo de vida das Columns
 * - Dependency Injection: ActivityLog injetado externamente
 */
class Board {
public:
    /**
     * @brief Construtor da classe Board.
     * 
     * Cria uma nova instância de Board com identificador e nome obrigatórios.
     * O board é inicializado sem colunas e sem log de atividades.
     * 
     * @param id Identificador único do board
     * @param name Nome descritivo do board
     * @pre id não deve ser string vazia
     * @pre name não deve ser string vazia
     * @post Board criado sem colunas e sem ActivityLog
     */
    Board(std::string id, std::string name);

    /**
     * @brief Adiciona uma nova coluna ao board.
     * 
     * Insere uma coluna no board garantindo unicidade de nome.
     * A coluna é copiada para o container interno (composição).
     * 
     * @param column Coluna a ser adicionada
     * @return true se adicionada com sucesso, false se nome já existe
     * @post Se sucesso, coluna adicionada ao final do vetor de colunas
     * @todo Implementar verificação de unicidade de nome
     */
    bool addColumn(const Column& column);

    /**
     * @brief Remove coluna do board pelo nome.
     * 
     * Localiza e remove a coluna especificada. Todos os cards
     * contidos na coluna são perdidos (comportamento de composição).
     * 
     * @param name Nome da coluna a ser removida
     * @return true se removida com sucesso, false se não encontrada
     * @todo Implementar validação de existência antes da remoção
     */
    bool removeColumn(const std::string& name);

    /**
     * @brief Busca coluna pelo nome (versão não-const).
     * 
     * Localiza coluna por nome permitindo modificação.
     * 
     * @param name Nome da coluna procurada
     * @return Ponteiro para a coluna encontrada ou nullptr se não existir
     * @note Permite modificação da coluna retornada
     */
    Column* findColumn(const std::string& name);

    /**
     * @brief Busca coluna pelo nome (versão const).
     * 
     * Localiza coluna por nome para acesso somente leitura.
     * 
     * @param name Nome da coluna procurada
     * @return Ponteiro const para a coluna encontrada ou nullptr se não existir
     * @note Acesso somente leitura à coluna retornada
     */
    const Column* findColumn(const std::string& name) const;

    /**
     * @brief Adiciona card a uma coluna específica.
     * 
     * Operação de conveniência que localiza a coluna e delega
     * a operação de adição do card.
     * 
     * @param columnName Nome da coluna de destino
     * @param card Card a ser adicionado
     * @return true se adicionado com sucesso, false se coluna não existe
     * @todo Implementar delegação para Column::addCard()
     */
    bool addCard(const std::string& columnName, const Card& card);

    /**
     * @brief Move card entre colunas.
     * 
     * Remove card da coluna origem e adiciona na coluna destino.
     * Verifica regras de WIP (Work In Progress) antes da movimentação.
     * 
     * @param cardId Identificador do card a ser movido
     * @param fromCol Nome da coluna origem
     * @param toCol Nome da coluna destino
     * @return true se movido com sucesso, false caso contrário
     * @todo Implementar verificação de regras de WIP
     */
    bool moveCard(const std::string& cardId, const std::string& fromCol, const std::string& toCol);

    /**
     * @brief Injeta dependência do sistema de log.
     * 
     * Implementa padrão Dependency Injection para o sistema de auditoria.
     * Board não gerencia o ciclo de vida do ActivityLog.
     * 
     * @param log Ponteiro para o sistema de log (pode ser nullptr)
     * @post ActivityLog configurado para uso (se não nullptr)
     * @note Board não possui o ActivityLog - gerenciamento externo
     */
    void attachActivityLog(ActivityLog* log);

    // Getters para acesso aos dados
    const std::string& getId() const;
    const std::string& getName() const;
    const std::vector<Column>& getColumns() const;
    std::vector<Column>& getColumns();  // Versão não-const para permitir modificação

    // Métodos de filtro e busca
    /**
     * @brief Busca todos os cards que possuem uma etiqueta específica.
     * @param tag Etiqueta a ser buscada
     * @return Vector de ponteiros para cards que possuem a etiqueta
     */
    std::vector<Card*> findCardsByTag(const std::string& tag);

    /**
     * @brief Filtra cards por prioridade mínima.
     * @param minPriority Prioridade mínima (inclusive)
     * @return Vector de ponteiros para cards com prioridade >= minPriority
     */
    std::vector<Card*> filterByPriority(int minPriority);

    /**
     * @brief Filtra cards por usuário responsável.
     * @param user Ponteiro para o usuário
     * @return Vector de ponteiros para cards atribuídos ao usuário
     */
    std::vector<Card*> filterByAssignee(User* user);

    /**
     * @brief Coleta todas as etiquetas únicas de todos os cards do board.
     * @return Vector de strings com todas as etiquetas em uso
     */
    std::vector<std::string> getAllTags() const;

    /**
     * @brief Serializa o board para JSON.
     * @return Objeto JSON com id, name e array de colunas
     */
    nlohmann::json toJson() const;

    /**
     * @brief Desserializa board a partir de JSON.
     * @param j Objeto JSON com dados do board
     * @return Board reconstruído
     * @throws std::invalid_argument se campos obrigatórios ausentes
     * @throws json::exception se estrutura inválida
     */
    static Board fromJson(const nlohmann::json& j);

private:
    std::string m_id;                              /**< @brief Identificador único do board */
    std::string m_name;                            /**< @brief Nome descritivo do board */
    std::vector<Column> m_columns;                 /**< @brief Container de colunas (composição) */
    ActivityLog* m_activityLog { nullptr };       /**< @brief Ponteiro para sistema de log (não owned) */
};

#endif // KANBAN_LITE_BOARD_H
