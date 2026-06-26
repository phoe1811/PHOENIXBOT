#pragma once

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t audio_init(void);

esp_err_t audio_read_sample(int32_t *sample);

// NEW
uint32_t audio_get_level(void);

#ifdef __cplusplus
}
#endif