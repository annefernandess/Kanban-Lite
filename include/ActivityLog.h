#ifndef KANBAN_LITE_ACTIVITY_LOG_H
#define KANBAN_LITE_ACTIVITY_LOG_H

#include <string>
#include <vector>
#include <chrono>
#include "external/json.hpp"

/**
 * @file ActivityLog.h
 * @brief Sistema de auditoria e logging para operações do Kanban.
 * @author Anne Fernandes da Costa Oliveira
 * @date 25/09/2025
 */

/**
 * @struct ActivityEntry
 * @brief Representa uma entrada individual no log de atividades.
 * 
 * Estrutura simples que armazena um evento com timestamp preciso.
 * Utiliza std::chrono para garantir precisão temporal e permitir
 * operações matemáticas com datas.
 */
struct ActivityEntry {
    std::chrono::system_clock::time_point timestamp;  /**< @brief Momento exato do evento */
    std::string message;                               /**< @brief Descrição do evento ocorrido */
};

/**
 * @class ActivityLog
 * @brief Sistema centralizado de auditoria para eventos do sistema Kanban.
 * 
 * Responsável por registrar e armazenar eventos do sistema para fins de
 * auditoria, debug e rastreabilidade. Implementa padrão de logging simples
 * com timestamps automáticos e armazenamento em memória.
 * 
 * @details
 * - Responsabilidade única: logging e auditoria de eventos
 * - Thread-safety: não implementada (monothreaded na Etapa 1)
 * - Persistência: planejada para etapas futuras
 * - Armazenamento: em memória (std::vector)
 * 
 * Conceitos POO aplicados:
 * - Encapsulamento: dados privados com interface controlada
 * - Abstração: interface simples record() e all()
 * - Coesão: foca apenas em logging
 */
class ActivityLog {
public:
    /**
     * @brief Registra um novo evento no log com timestamp automático.
     * 
     * Cria uma entrada de log com o evento especificado e timestamp
     * do momento atual. O timestamp é gerado automaticamente usando
     * std::chrono::system_clock::now().
     * 
     * @param event Descrição do evento a ser registrado
     * @pre event não deve ser vazio (recomendação)
     * @post Nova entrada adicionada ao final do vetor de entries
     * @post Timestamp definido como momento atual da chamada
     * 
     * @note Thread-safety não garantida na versão atual
     */
    void record(const std::string& event);

    /**
     * @brief Retorna cópia de todas as entradas do log.
     * 
     * Fornece acesso completo ao histórico de eventos registrados.
     * Retorna cópia para evitar modificações acidentais e garantir
     * thread-safety básica.
     * 
     * @return std::vector<ActivityEntry> Cópia completa do histórico
     * @post Estado do objeto não é alterado (método const)
     * 
     * @note Retorna cópia (não referência) para simplicidade na Etapa 1
     * @note Para listas grandes, considerar iteradores ou paginação em versões futuras
     */
    std::vector<ActivityEntry> all() const;

    /**
     * @brief Serializa o log de atividades para JSON.
     * 
     * Converte todas as entradas do log para formato JSON.
     * 
     * @return Objeto JSON com array de entradas
     */
    nlohmann::json toJson() const;

    /**
     * @brief Desserializa log de atividades a partir de JSON.
     * 
     * Reconstrói log com todas as entradas.
     * 
     * @param j Objeto JSON com dados do log
     * @return ActivityLog reconstruído
     * @throws json::exception se estrutura inválida
     */
    static ActivityLog fromJson(const nlohmann::json& j);

private:
    std::vector<ActivityEntry> m_entries;         /**< @brief Container com histórico completo de eventos */
};

#endif // KANBAN_LITE_ACTIVITY_LOG_H
