#pragma once

#include "driver/i2s_std.h"
#include "esp_err.h"
#include <stdint.h>

class Microphone
{
public:

    // Initialize INMP441
    esp_err_t begin();

    // Read one audio sample
    esp_err_t read(int32_t *sample);

private:

    // I2S RX channel
    i2s_chan_handle_t rx_channel = nullptr;

    // GPIO configuration
    static constexpr gpio_num_t PIN_BCLK = GPIO_NUM_5;
    static constexpr gpio_num_t PIN_WS   = GPIO_NUM_6;
    static constexpr gpio_num_t PIN_DIN  = GPIO_NUM_4;

    // Audio configuration
    static constexpr uint32_t SAMPLE_RATE = 16000;
};