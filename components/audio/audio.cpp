#include "audio.hpp"
#include "Microphone.hpp"

#include "esp_log.h"
#include <stdlib.h>

static const char *TAG = "AUDIO";

static Microphone microphone;

extern "C"
esp_err_t audio_init(void)
{
    ESP_LOGI(TAG,"=================================");
    ESP_LOGI(TAG,"Initializing Audio...");
    ESP_LOGI(TAG,"=================================");

    esp_err_t err = microphone.begin();

    if(err != ESP_OK)
    {
        ESP_LOGE(TAG,"Microphone Initialization Failed");
        return err;
    }

    ESP_LOGI(TAG,"Audio Ready");

    return ESP_OK;
}

extern "C"
esp_err_t audio_read_sample(int32_t *sample)
{
    esp_err_t err = microphone.read(sample);

    if(err != ESP_OK)
        return err;

    // INMP441 24-bit sample normalize
    *sample >>= 8;

    return ESP_OK;
}

extern "C"
uint32_t audio_get_level(void)
{
    int32_t sample;

    if(audio_read_sample(&sample)!=ESP_OK)
        return 0;

    return abs(sample);
}