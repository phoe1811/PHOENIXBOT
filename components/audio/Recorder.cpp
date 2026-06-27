#include "Recorder.hpp"

#include <stdio.h>
#include <string.h>

Recorder::Recorder()
{
    index = 0;
    recording = false;
}

esp_err_t Recorder::begin()
{
    printf("RECORDER: Ready\n");

    clear();

    return ESP_OK;
}

void Recorder::clear()
{
    index = 0;

    recording = false;

    memset(buffer,0,sizeof(buffer));
}

void Recorder::startRecording()
{
    if(recording)
        return;

    index = 0;

    recording = true;

    printf("\n");
    printf("=================================\n");
    printf("RECORDING STARTED\n");
    printf("=================================\n");
}

void Recorder::stopRecording()
{
    if(!recording)
        return;

    recording = false;

    printf("\n");
    printf("=================================\n");
    printf("RECORDING STOPPED\n");
    printf("Samples Recorded : %lu\n",
           (unsigned long)index);
    printf("=================================\n");
}

bool Recorder::isRecording()
{
    return recording;
}

void Recorder::addSample(int32_t sample)
{
    if(!recording)
        return;

    if(index >= MAX_SAMPLES)
    {
        stopRecording();
        return;
    }

    buffer[index++] = sample;
}

int32_t* Recorder::getBuffer()
{
    return buffer;
}

uint32_t Recorder::getLength()
{
    return index;
}

uint32_t Recorder::getMaxSamples()
{
    return MAX_SAMPLES;
}