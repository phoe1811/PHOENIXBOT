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

    ESP_ERROR_CHECK(calibrateMicrophone());

    printf("[OK]\n");

    printf("[STEP 4] Ready\n");

    printf("\n");
    printf("=========================================\n");
    printf(" AUDIO ENGINE READY\n");
    printf("=========================================\n");

    return ESP_OK;
}
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

    //------------------------------------------
    // Store raw PCM while recording
    //------------------------------------------

    if(recorder.isRecording())
    {
        for(size_t i = 0; i < count; i++)
        {
            recorder.addSample(buffer[i]);
        }
    }

    //------------------------------------------
    // RMS Calculation
    //------------------------------------------

    double sum = 0;

    for(size_t i = 0; i < count; i++)
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

    //------------------------------------------
    // Print every 10th frame
    //------------------------------------------

    printCounter++;

    if(printCounter >= 10)
    {
        printCounter = 0;

        printf("LEVEL = %ld\n", level);
    }

    //------------------------------------------
    // Voice Start
    //------------------------------------------

    if(level > voiceThreshold)
    {
        silenceCounter = 0;

        if(!speaking)
        {
            speaking = true;

            recorder.startRecording();

            printf("\n");
            printf("=================================\n");
            printf("VOICE START\n");
            printf("Threshold : %ld\n", voiceThreshold);
            printf("Level     : %ld\n", level);
            printf("=================================\n");
        }

        return true;
    }

    //------------------------------------------
    // Voice End
    //------------------------------------------

    if(speaking)
    {
        silenceCounter++;

        if(silenceCounter >= 20)
        {
            speaking = false;

            recorder.stopRecording();

            printf("\n");
            printf("=================================\n");
            printf("VOICE END\n");
            printf("Recorded Samples : %lu\n",
                   (unsigned long)recorder.getLength());
            printf("=================================\n");
        }
    }

    return false;
}
