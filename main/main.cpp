#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "AudioEngine.hpp"

extern "C" void app_main(void)
{
    printf("\n");
    printf("=========================================\n");
    printf("      PHOENIXBOT V1.1 STARTING\n");
    printf("=========================================\n");

    static AudioEngine audio;

    esp_err_t err = audio.begin();

    if(err != ESP_OK)
    {
        printf("\n");
        printf("=========================================\n");
        printf(" AUDIO INITIALIZATION FAILED\n");
        printf("=========================================\n");

        while(true)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    printf("\n");
    printf("=========================================\n");
    printf("      PHOENIXBOT READY\n");
    printf("=========================================\n");

    while(true)
    {
        audio.voiceDetected();

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}