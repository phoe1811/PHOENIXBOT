#include "audio.hpp"
#include "Microphone.hpp"

#include "esp_log.h"
#include <stdlib.h>
#include <math.h>

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
    esp_err_t err = microphone.read();

    if(err != ESP_OK)
        return err;

    int32_t *buffer = microphone.getBuffer();

    size_t count = microphone.getSampleCount();

    if(count == 0)
    {
        *sample = 0;
        return ESP_OK;
    }

    double sum = 0;

    for(size_t i = 0; i < count; i++)
    {
        double s = (double)(buffer[i] >> 8);

        sum += s * s;
    }

    *sample = (int32_t)sqrt(sum / count);

    return ESP_OK;
}

extern "C"
uint32_t audio_get_level(void)
{
    int32_t sample = 0;

    if(audio_read_sample(&sample) != ESP_OK)
        return 0;

    return (uint32_t)sample;
}