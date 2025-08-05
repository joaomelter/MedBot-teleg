idf_component_register(
    SRCS 
        "http_client.c"
        "compartimentos.c"
        "time_sinc.c"
        "wifi.c"
        "led.c"
        "buzzer.c"
        "main.c"
    INCLUDE_DIRS 
        "."
    REQUIRES
        lwip
        freertos
        
    PRIV_REQUIRES
        esp_wifi
        esp_netif
        esp_http_client
        nvs_flash
        driver
        esp_event

)