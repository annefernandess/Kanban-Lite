#include "ActivityLog.h"

/**
 * @file ActivityLog.cpp
 * @brief Implementação da classe ActivityLog para registro de atividades do sistema.
 * @author Anne Fernandes da Costa Oliveira
 * @date 01/10/2025
 */

/**
 * @brief Registra uma nova atividade no log.
 * @param event Descrição do evento a ser registrado
 */
void ActivityLog::record(const std::string& event) {
    ActivityEntry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.message = event;
    m_entries.push_back(std::move(entry));
}

/**
 * @brief Retorna todas as entradas do log de atividades.
 * @return Cópia do vetor contendo todas as entradas de atividade
 */
std::vector<ActivityEntry> ActivityLog::all() const {
    return m_entries; // Retorna cópia como especificado no header
}