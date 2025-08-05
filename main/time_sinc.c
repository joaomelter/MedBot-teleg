#include "time_sinc.h"
#include <time.h>
#include "esp_log.h"
#include "sys/time.h" // Para settimeofday()

static const char *TAG = "RTC";

void set_time_manually(int year, int month, int day, int hour, int min, int sec)
{
    struct tm tm_time = {
        .tm_year = year - 1900, // Ano desde 1900
        .tm_mon = month - 1,    // Mês 0-11
        .tm_mday = day,         // Dia 1-31
        .tm_hour = hour,        // Hora 0-23
        .tm_min = min,          // Minuto 0-59
        .tm_sec = sec           // Segundo 0-59
    };

    time_t t = mktime(&tm_time);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);

    ESP_LOGI(TAG, "Horário configurado: %04d-%02d-%02d %02d:%02d:%02d",
             year, month, day, hour, min, sec);
}

void print_local_time(void)
{
    time_t now;
    struct tm timeinfo;
    char buffer[50];

    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

    ESP_LOGI(TAG, "Horário atual: %s", buffer);
}