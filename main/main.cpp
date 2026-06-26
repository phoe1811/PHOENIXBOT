#include <stdio.h>

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "audio.hpp"
}

extern "C"
void app_main(void)
{
    printf("\n");
    printf("=========================================\n");
    printf("      PHOENIXBOT V1.0 STARTING\n");
    printf("=========================================\n");

    if(audio_init()!=ESP_OK)
    {
        printf("Audio Init Failed\n");

        while(true)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    printf("PHOENIXBOT READY\n");

    while(true)
    {
        printf("LEVEL = %lu\n",
               (unsigned long)audio_get_level());

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}