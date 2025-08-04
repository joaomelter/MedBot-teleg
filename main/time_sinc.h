#pragma once // Evita múltiplas inclusões desse arquivo

// C standard libs
#include <stdio.h>    // Funções básicas de entrada e saída
#include <time.h>     // Biblioteca para manipular o tempo

// ESP-IDF libs
#include "esp_log.h"      // Logs no terminal
#include "esp_sntp.h"     // Sincronização via NTP
#include "freertos/FreeRTOS.h" // Gerenciamento de tarefas
#include "freertos/task.h"

// Protótipos das funções
void iniciar_sntp(void);   // Inicializa a sincronização NTP (Network Time Protocol)
void printf_time(void);    // Mostra o horário atual

/*
NTP é um protocolo de rede utilizado para sincronizar o relógio do ESP32 ou qualquer computador com os servidores de hora oficial da internet.
SNTP é uma versão simplificada desses protocolos, utilizada em sistemas embarcados.
Log é o registro de informações.
*/
