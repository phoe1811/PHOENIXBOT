#include "AudioEngine.hpp"

#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

AudioEngine::AudioEngine()
{
    speaking = false;
    silenceCounter = 0;
    printCounter = 0;
    noiseFloor = 0;
    voiceThreshold = 0;
}

esp_err_t AudioEngine::begin()
{
    printf("\n");
    printf("=========================================\n");
    printf(" AUDIO ENGINE INITIALIZING\n");
    printf("=========================================\n");

    printf("[STEP 1] Microphone.begin()\n");
    ESP_ERROR_CHECK(microphone.begin());
    printf("[OK]\n");

    printf("[STEP 2] Recorder.begin()\n");
    ESP_ERROR_CHECK(recorder.begin());
    printf("[OK]\n");

    printf("[STEP 3] Calibration\n");
    esp_err_t err = calibrateMicrophone();

if(err != ESP_OK)
{
    printf("Calibration Failed\n");
    return err;
}
    printf("[OK]\n");

    printf("=========================================\n");
    printf(" AUDIO ENGINE READY\n");
    printf("=========================================\n");

    return ESP_OK;
}

esp_err_t AudioEngine::calibrateMicrophone()
{
    printf("Calibrating Microphone...\n");

    int64_t total = 0;
    int32_t rms = 0;

    for(int i=0;i<DISCARD_SAMPLES;i++)
    {
        microphone.read();
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    for(int i=0;i<CALIBRATION_SAMPLES;i++)
    {
        if(getRMS(&rms) != ESP_OK)
            return ESP_FAIL;

        total += rms;
        printf(".");
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    printf("\n");

    noiseFloor = total / CALIBRATION_SAMPLES;
    voiceThreshold = noiseFloor + 60000;

    if(voiceThreshold < 90000)
        voiceThreshold = 90000;

    printf("Ambient Noise   : %ld\n", noiseFloor);
    printf("Voice Threshold : %ld\n", voiceThreshold);

    return ESP_OK;
}

esp_err_t AudioEngine::getRMS(int32_t *level)
{
    esp_err_t ret = microphone.read();

    if(ret != ESP_OK)
        return ret;

    int32_t *buffer = microphone.getBuffer();
    size_t count = microphone.getSampleCount();

    if(count == 0)
    {
        *level = 0;
        return ESP_OK;
    }

    if(recorder.isRecording())
    {
        for(size_t i=0;i<count;i++)
            recorder.addSample(buffer[i]);
    }

    double sum = 0;

    for(size_t i=0;i<count;i++)
    {
        double sample = (double)(buffer[i] >> 8);
        sum += sample * sample;
    }

    *level = (int32_t)sqrt(sum / count);

    return ESP_OK;
}

bool AudioEngine::voiceDetected()
{
    int32_t level = 0;

    if(getRMS(&level) != ESP_OK)
        return false;

    printCounter++;

    if(printCounter >= 10)
    {
        printCounter = 0;
        printf("LEVEL = %ld\n", level);
    }

    if(level > voiceThreshold)
    {
        silenceCounter = 0;

        if(!speaking)
        {
            speaking = true;
            recorder.startRecording();

            printf("\n=================================\n");
            printf("VOICE START\n");
            printf("Threshold : %ld\n", voiceThreshold);
            printf("Level     : %ld\n", level);
            printf("=================================\n");
        }

        return true;
    }

    if(speaking)
    {
        silenceCounter++;

        if(silenceCounter >= 20)
        {
            speaking = false;
            recorder.stopRecording();

            printf("\n=================================\n");
            printf("VOICE END\n");
            printf("Recorded Samples : %lu\n",
                   (unsigned long)recorder.getLength());
            printf("=================================\n");
        }
    }

    return false;
}
