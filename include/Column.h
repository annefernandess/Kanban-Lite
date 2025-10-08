#ifndef KANBAN_LITE_COLUMN_H
#define KANBAN_LITE_COLUMN_H

#include <string>
#include <vector>
#include "external/json.hpp"

class Card; // forward declaration

/**
 * @file Column.h
 * @brief Definição da classe Column para organização de cards no Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 25/09/2025
 */

/**
 * @class Column
 * @brief Representa uma coluna do quadro Kanban que organiza cards.
 * 
 * A classe Column mantém e organiza um conjunto de cards, implementando
 * conceitos de Work In Progress (WIP) limits para controle de fluxo.
 * Utiliza composição para gerenciar o ciclo de vida dos cards contidos.
 * 
 * @details
 * - Responsabilidade única: manter e organizar conjunto de cards
 * - Composição: Column possui e gerencia cards
 * - WIP Limits: controle de limite de trabalho em progresso
 * - Encapsulamento: acesso controlado aos cards via métodos públicos
 */
class Column {
public:
    /**
     * @brief Construtor da classe Column.
     * 
     * Cria uma nova coluna com nome obrigatório e limite WIP opcional.
     * Valor -1 para wipLimit indica ausência de limite.
     * 
     * @param name Nome identificador da coluna
     * @param wipLimit Limite máximo de cards (-1 = sem limite)
     * @pre name não deve ser string vazia
     * @post Column criada vazia (sem cards)
     */
    Column(std::string name, int wipLimit = -1);

    /**
     * @brief Adiciona card à coluna.
     * 
     * Insere novo card na coluna respeitando limite WIP quando ativo.
     * O card é copiado para o container interno (composição).
     * 
     * @param card Card a ser adicionado
     * @return true se adicionado com sucesso, false se limite WIP atingido
     * @post Se sucesso, card adicionado ao final do vetor
     * @todo Implementar verificação de limite WIP
     */
    bool addCard(const Card& card);

    /**
     * @brief Remove card da coluna pelo ID.
     * 
     * Localiza e remove o card especificado da coluna.
     * 
     * @param cardId Identificador único do card a ser removido
     * @return true se removido com sucesso, false se não encontrado
     * @post Card removido do container se encontrado
     */
    bool removeCard(const std::string& cardId);

    /**
     * @brief Busca card pelo ID (versão não-const).
     * 
     * Localiza card por identificador permitindo modificação.
     * 
     * @param cardId Identificador do card procurado
     * @return Ponteiro para o card encontrado ou nullptr se não existir
     * @note Permite modificação do card retornado
     */
    Card* findCard(const std::string& cardId);

    /**
     * @brief Busca card pelo ID (versão const).
     * 
     * Localiza card por identificador para acesso somente leitura.
     * 
     * @param cardId Identificador do card procurado
     * @return Ponteiro const para o card encontrado ou nullptr se não existir
     * @note Acesso somente leitura ao card retornado
     */
    const Card* findCard(const std::string& cardId) const;

    /**
     * @brief Verifica se coluna atingiu limite WIP.
     * 
     * Determina se a coluna está cheia baseado no limite WIP configurado.
     * Sempre retorna false se wipLimit = -1 (sem limite).
     * 
     * @return true se limite atingido, false caso contrário
     * @note Sempre false quando wipLimit = -1
     * @todo Implementar lógica quando WIP limits estiverem ativos
     */
    bool isFull() const;

        /**
     * @brief Obtém lista de todos os cards da coluna (read-only).
     * 
     * @return Referência const para std::vector<Card> interno
     * @note Permite iteração segura sem modificação dos dados
     */
    const std::vector<Card>& getCards() const;

    /**
     * @brief Obtém lista de todos os cards da coluna (mutable).
     * 
     * @return Referência para std::vector<Card> interno
     * @note Permite modificação dos cards
     */
    std::vector<Card>& getCards();

    /**
     * @brief Obtém nome da coluna.
     * 
     * @return Nome identificador da coluna
     */
    const std::string& getName() const;

    /**
     * @brief Obtém limite WIP configurado.
     * 
     * @return Valor do limite WIP (-1 indica sem limite)
     */
    int getWipLimit() const;

    /**
     * @brief Obtém quantidade atual de cards.
     * 
     * @return Número de cards presentes na coluna
     */
    size_t getCardCount() const;

    /**
     * @brief Operador de igualdade para comparação de colunas.
     * 
     * Compara duas colunas baseado no nome identificador.
     * 
     * @param other Coluna a ser comparada
     * @return true se nomes forem iguais, false caso contrário
     */
    bool operator==(const Column& other) const;

    /**
     * @brief Serializa a coluna para JSON.
     * 
     * Converte coluna e todos seus cards para formato JSON.
     * 
     * @return Objeto JSON com nome, WIP limit e array de cards
     */
    nlohmann::json toJson() const;

    /**
     * @brief Desserializa coluna a partir de JSON.
     * 
     * Reconstrói coluna com todos os cards contidos.
     * 
     * @param j Objeto JSON com dados da coluna
     * @return Column reconstruída com cards
     * @throws json::exception se campos obrigatórios ausentes
     * @throws std::invalid_argument se dados inválidos
     */
    static Column fromJson(const nlohmann::json& j);

private:
    std::string m_name;                            /**< @brief Nome identificador da coluna */
    int m_wipLimit;                                /**< @brief Limite WIP (-1 = sem limite) */
    std::vector<Card> m_cards;                     /**< @brief Container de cards (composição) */
};

#endif // KANBAN_LITE_COLUMN_H
