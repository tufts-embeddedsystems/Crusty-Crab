// Luan Banh
// Main configuration for the project

#pragma once
// #pragma pack(1)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "math.h"

#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#include "nvs_flash.h"
#include "esp_netif.h"
#include "mqtt_client.h"
#include "minimal_wifi.h"

// VERBOSE 1 means print, 0 means don't print
#define VERBOSE 1

#if VERBOSE
  #define PRINTF(f_, ...) printf((f_), ##__VA_ARGS__)
#else
  #define PRINTF(f_, ...)
#endif

#define TIME_TO_SLEEP  1800        /* Time ESP32 will go to sleep (in seconds), half an hour */

#define BROKER_URI "mqtt://en1-pi.eecs.tufts.edu"

#define CONFIG_EXAMPLE_WIFI_SSID "Tufts_Wireless"
#define CONFIG_EXAMPLE_WIFI_PASSWORD ""

#define ON_LED  6