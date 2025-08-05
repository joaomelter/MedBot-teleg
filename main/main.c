#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "time_sinc.h"

void app_main(void)
{
    // Configura o horário manualmente (ano, mês, dia, hora, minuto, segundo)
    set_time_manually(2025, 8, 6, 14, 30, 0); // Exemplo: 6/8/2025, 14:30:00

    while (1) {
        print_local_time(); // Mostra o horário atual
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Atualiza a cada 1 segundo
    }
}