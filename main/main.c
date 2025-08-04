#include "esp_wifi.h"  // Controla a conexao do Wi-Fi do ESP32
#include "esp_event.h" // Sistema que gerencia os eventos do ESP-IDF
#include "nvs_flash.h" // Acesso a memoria flash persistente (NVS = Non-Volatile Storage)

#include "esp_http_client.h" // Cliente HTTP para fazer as requisições, enviar os dados para o server python

#include "driver/gpio.h" // Controla os pinos GPIO saidas e entradas de energia 
#include "driver/ledc.h"  // Controla as saidas esp32 pwm

#include "esp_sntp.h" //Sincroniza a hora com o servidor NTP pela internet
#include <time.h> // Bibliioteca padrão da linguagem C para manipular o tempo

