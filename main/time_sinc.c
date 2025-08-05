#include "time_sinc.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "lwip/apps/sntp.h"
#include <string.h>


static const char *TAG = "TimeModule";

// Configuração melhorada
static const char *ntp_servers[] = {
    "pool.ntp.org",         // Servidor público principal
    "br.pool.ntp.org",      // Servidor regional (Brasil)
    "time.google.com"       // Backup adicional
};

// Callback de sincronização
static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Sincronização NTP concluída!");
}

void iniciar_sntp(void)
{
    ESP_LOGI(TAG, "Inicializando SNTP...");
    
    // Configura timezone (BRT/BRST - América/São Paulo)
    setenv("TZ", "BRT3BRST,M10.3.0/0,M2.3.0/0", 1);
    tzset();

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_set_sync_interval(3600000); // Sincroniza a cada 1h

    // Configura servidores
    for (int i = 0; i < sizeof(ntp_servers)/sizeof(ntp_servers[0]); i++) {
        sntp_setservername(i, ntp_servers[i]);
    }
    
    sntp_init();
}

bool sincronizacao_concluida(void)
{
    time_t agora;
    time(&agora);
    
    // Verifica se o tempo é posterior a 01/01/2022
    return (agora > 1640995200); // Timestamp para 01/01/2022
}

void printf_time(void)
{
    time_t agora;
    struct tm info_tempo;
    char strftime_buf[64];

    time(&agora);
    localtime_r(&agora, &info_tempo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &info_tempo);

    if (!sincronizacao_concluida()) {
        ESP_LOGW(TAG, "Aguardando sincronização NTP...");
        return;
    }

    ESP_LOGI(TAG, "Data/Hora atual: %s", strftime_buf);
    ESP_LOGI(TAG, "Timestamp: %lld", (long long)agora);
}