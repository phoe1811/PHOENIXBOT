#ifndef RECORDER_HPP
#define RECORDER_HPP

#include "esp_err.h"
#include <stdint.h>

class Recorder
{
public:

    Recorder();

    esp_err_t begin();

    void startRecording();

    void stopRecording();

    bool isRecording();

    void addSample(int32_t sample);

    int32_t* getBuffer();

    uint32_t getLength();

    uint32_t getMaxSamples();

    void clear();

private:

    static constexpr uint32_t MAX_SAMPLES = 16000 * 5;

    int32_t buffer[MAX_SAMPLES];

    uint32_t index;

    bool recording;
};

#endif