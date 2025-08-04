#include "time_sinc.h"     // Inclui os includes do arquivo time_sinc.h
#include "lwip/apps/sntp.h"  // Substitui esp_sntp.h
#include "esp_log.h"       // Permite imprimir mensagens coloridas no terminal
#include "esp_system.h"    // Funções extras do sistema

static const char *TAG = "TimeModule";

// Função responsável por sincronizar com servidor NTP para sincronizar com a hora atual
void iniciar_sntp(void)
{
    ESP_LOGI(TAG, "Inicializando SNTP...");

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org"); // Servidor público mundial de NTP    
    sntp_init(); // Inicializa o serviço SNTP
}

void printf_time(void)
{
    time_t agora;
    struct tm info_tempo;

    time(&agora);
    localtime_r(&agora, &info_tempo); // Converte para struct tm para data e hora legível

    if (info_tempo.tm_year < (2016 - 1900))
    {
        ESP_LOGI(TAG, "Esperando Sincronizacao NTP...");
        while(info_tempo.tm_year < (2016 - 1900))
        {
            vTaskDelay(2000 / portTICK_PERIOD_MS); // Espera 2 segundos
            time(&agora);
            localtime_r(&agora, &info_tempo);
        }
    }

    // Cria string formatada para exibir a hora atual
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &info_tempo);

    // Mostra a hora atual no console
    ESP_LOGI(TAG, "Data/Hora atual: %s", strftime_buf);
}
