#include "Microphone.hpp"

#include <stdio.h>

#include "driver/i2s_std.h"

#define I2S_BCLK GPIO_NUM_5
#define I2S_WS   GPIO_NUM_6
#define I2S_DIN  GPIO_NUM_4

static i2s_chan_handle_t rx_handle = NULL;

Microphone::Microphone()
{
    sampleCount = 0;
}

esp_err_t Microphone::begin()
{
    printf("MIC: Initializing...\n");

    i2s_chan_config_t chan_cfg =
        I2S_CHANNEL_DEFAULT_CONFIG(
            I2S_NUM_AUTO,
            I2S_ROLE_MASTER);

    ESP_ERROR_CHECK(
        i2s_new_channel(
            &chan_cfg,
            NULL,
            &rx_handle));

    i2s_std_config_t std_cfg =
    {
        .clk_cfg =
            I2S_STD_CLK_DEFAULT_CONFIG(16000),

        .slot_cfg =
            I2S_STD_MSB_SLOT_DEFAULT_CONFIG(
                I2S_DATA_BIT_WIDTH_32BIT,
                I2S_SLOT_MODE_MONO),

        .gpio_cfg =
        {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCLK,
            .ws = I2S_WS,
            .dout = I2S_GPIO_UNUSED,
            .din = I2S_DIN,

            .invert_flags =
            {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    ESP_ERROR_CHECK(
        i2s_channel_init_std_mode(
            rx_handle,
            &std_cfg));

    ESP_ERROR_CHECK(
        i2s_channel_enable(
            rx_handle));

    printf("MIC: Ready\n");

    return ESP_OK;
}

esp_err_t Microphone::read()
{
    size_t bytesRead = 0;

    esp_err_t ret =
        i2s_channel_read(
            rx_handle,
            samples,
            sizeof(samples),
            &bytesRead,
            100);

    if (ret != ESP_OK)
        return ret;

    sampleCount = bytesRead / sizeof(int32_t);

    return ESP_OK;
}

int32_t* Microphone::getBuffer()
{
    return samples;
}

size_t Microphone::getSampleCount()
{
    return sampleCount;
}