#ifndef MICROPHONE_HPP
#define MICROPHONE_HPP

#include "esp_err.h"
#include <stdint.h>
#include <stddef.h>

class Microphone
{
public:

    Microphone();

    esp_err_t begin();

    esp_err_t read();

    int32_t* getBuffer();

    size_t getSampleCount();

private:

    static constexpr int SAMPLE_COUNT = 128;

    int32_t samples[SAMPLE_COUNT];

    size_t sampleCount;
};

#endif