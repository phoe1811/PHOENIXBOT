#ifndef AUDIO_ENGINE_HPP
#define AUDIO_ENGINE_HPP

#include "esp_err.h"
#include <stdint.h>

#include "Microphone.hpp"
#include "Recorder.hpp"

class AudioEngine
{
public:

    AudioEngine();

    esp_err_t begin();

    esp_err_t getRMS(int32_t *level);

    bool voiceDetected();

private:

    //-------------------------
    // Hardware
    //-------------------------

    Microphone microphone;

    Recorder recorder;

    //-------------------------
    // Audio Config
    //-------------------------

    static constexpr int SAMPLE_COUNT = 128;

    static constexpr int CALIBRATION_SAMPLES = 100;

    static constexpr int DISCARD_SAMPLES = 40;

    //-------------------------
    // Adaptive Threshold
    //-------------------------

    int32_t noiseFloor;

    int32_t voiceThreshold;

    //-------------------------
    // Voice State
    //-------------------------

    bool speaking;

    int silenceCounter;

    int printCounter;

    //-------------------------
    // Internal
    //-------------------------

    esp_err_t calibrateMicrophone();
};

#endif