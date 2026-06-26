#include "Microphone.hpp"

#include "esp_log.h"
#include <cstring>

static const char *TAG = "MIC";

esp_err_t Microphone::begin()
{
    esp_err_t err;

    //------------------------------------------
    // Create RX Channel
    //------------------------------------------

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(
        I2S_NUM_AUTO,
        I2S_ROLE_MASTER
    );

    err = i2s_new_channel(
        &chan_cfg,
        NULL,
        &rx_channel
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create RX channel");
        return err;
    }

    //------------------------------------------
    // Standard Mode Configuration
    //------------------------------------------

    i2s_std_config_t std_cfg = {

        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000),

        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(
            I2S_DATA_BIT_WIDTH_32BIT,
            I2S_SLOT_MODE_MONO
        ),

        .gpio_cfg = {

            .mclk = I2S_GPIO_UNUSED,

            .bclk = PIN_BCLK,

            .ws = PIN_WS,

            .dout = I2S_GPIO_UNUSED,

            .din = PIN_DIN,

            .invert_flags = {

                .mclk_inv = false,

                .bclk_inv = false,

                .ws_inv = false,
            },
        },
    };

    err = i2s_channel_init_std_mode(
        rx_channel,
        &std_cfg
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "STD Mode Init Failed");
        return err;
    }

    //------------------------------------------
    // Enable Channel
    //------------------------------------------

    err = i2s_channel_enable(rx_channel);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Enable Failed");
        return err;
    }

    ESP_LOGI(TAG, "INMP441 Ready");

    return ESP_OK;
}

esp_err_t Microphone::read(int32_t *sample)
{
    size_t bytes_read = 0;

    esp_err_t err = i2s_channel_read(
        rx_channel,
        sample,
        sizeof(int32_t),
        &bytes_read,
        100
    );

    if (err != ESP_OK)
    {
        return err;
    }

    if (bytes_read != sizeof(int32_t))
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}