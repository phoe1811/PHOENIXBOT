#pragma once

#include "driver/gpio.h"

/*
 * ==========================================================
 *                PHOENIXBOT CONFIGURATION
 * ==========================================================
 */

/***********************
 *  Audio Configuration
 ***********************/
#define AUDIO_SAMPLE_RATE     16000
#define AUDIO_BUFFER_SAMPLES  512

/***********************
 *  Microphone (INMP441)
 ***********************/
#define MIC_BCLK_PIN   GPIO_NUM_5
#define MIC_WS_PIN     GPIO_NUM_6
#define MIC_DOUT_PIN   GPIO_NUM_4

/***********************
 *  Speaker (MAX98357A)
 ***********************/
#define SPK_BCLK_PIN   GPIO_NUM_5
#define SPK_WS_PIN     GPIO_NUM_6
#define SPK_DIN_PIN    GPIO_NUM_15

/***********************
 *  Future Display
 ***********************/
#define TFT_CS_PIN     GPIO_NUM_32
#define TFT_DC_PIN     GPIO_NUM_46
#define TFT_RST_PIN    GPIO_NUM_33

/***********************
 *  WiFi
 ***********************/
#define WIFI_SSID      "Airtel_Suman"
#define WIFI_PASSWORD  "Tikhampur"