#include "ActivityLog.h"

using json = nlohmann::json;

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
    m_entries.push_back(entry);  // Removido std::move - deixa o compilador otimizar
}

/**
 * @brief Retorna todas as entradas do log de atividades.
 * @return Cópia do vetor contendo todas as entradas de atividade
 */
std::vector<ActivityEntry> ActivityLog::all() const {
    return m_entries; // Retorna cópia como especificado no header
}

/**
 * @brief Serializa o log de atividades para JSON.
 * @return Objeto JSON com array de entradas
 */
json ActivityLog::toJson() const {
    json entriesArray = json::array();
    
    for (const auto& entry : m_entries) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            entry.timestamp.time_since_epoch()
        ).count();
        
        entriesArray.push_back({
            {"timestamp", ms},
            {"message", entry.message}
        });
    }
    
    return json{
        {"entries", entriesArray}
    };
}

/**
 * @brief Desserializa log de atividades a partir de JSON.
 * @param j Objeto JSON com dados do log
 * @return ActivityLog reconstruído
 * @throws json::exception se estrutura inválida
 */
ActivityLog ActivityLog::fromJson(const json& j) {
    ActivityLog log;
    
    if (j.contains("entries") && j["entries"].is_array()) {
        for (const auto& entryJson : j["entries"]) {
            if (entryJson.contains("timestamp") && entryJson.contains("message")) {
                ActivityEntry entry;
                
                // Reconstrução do timestamp
                auto ms = std::chrono::milliseconds(
                    entryJson["timestamp"].get<long long>()
                );
                entry.timestamp = std::chrono::system_clock::time_point(ms);
                entry.message = entryJson["message"].get<std::string>();
                
                log.m_entries.push_back(std::move(entry));
            }
        }
    }
    
    return log;
}