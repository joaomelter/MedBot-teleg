#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdbool.h>
#include "esp_err.h"

/**
 * @file time_sinc.h
 * @brief Módulo de sincronização de tempo via NTP
 * 
 * Fornece sincronização com servidores NTP e funções para manipulação de tempo.
 * Requer conexão WiFi ativa antes de chamar iniciar_sntp().
 */

/* Constantes públicas */
#define TIME_SYNC_TIMEOUT_MS 10000  // Timeout de 10 segundos para sincronização

/**
 * @brief Inicializa o cliente SNTP
 * 
 * Configura múltiplos servidores NTP e inicia a sincronização.
 * Deve ser chamado APÓS a conexão WiFi estar estabelecida.
 */
void iniciar_sntp(void);

/**
 * @brief Verifica se a sincronização foi concluída
 * @return true se o tempo está sincronizado, false caso contrário
 */
bool sincronizacao_concluida(void);

/**
 * @brief Exibe o horário atual formatado
 * 
 * Mostra a data/hora no formato: "YYYY-MM-DD HH:MM:SS"
 * Se não sincronizado, exibe aviso.
 */
void printf_time(void);

/**
 * @brief Obtém o timestamp atual
 * @return Timestamp Unix em segundos, ou -1 se não sincronizado
 */
time_t obter_timestamp(void);

#ifdef __cplusplus
}
#endif