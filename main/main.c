#include <stdio.h>
#include "time_sinc.h"  // Inclua o header

void app_main() 
{
    printf("Iniciando sincronização de tempo...\n");
    
    // Chame as funções do time_sinc.c
    iniciar_sntp();
    
    while(1) {
        printf_time();
        vTaskDelay(5000 / portTICK_PERIOD_MS);  // A cada 5 segundos
    }
}