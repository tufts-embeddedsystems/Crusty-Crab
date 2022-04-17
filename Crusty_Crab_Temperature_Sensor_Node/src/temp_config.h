// Luan Banh
// configuration file

#pragma once

#include "main_config.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate

#define VDD             3.3
#define MAX_ADC         511.0
#define SAMPLE_NUMBER   10

#define ROOM_TEMP          298.15   // room temperature in Kelvin
#define BALANCE_RESISTOR   10000.0  // the resistance value for the balance resistor
#define RESISTOR_ROOM_TEMP 10000.0  // thermistor resistance at room temperature
#define THERMISTOR_OFFSET  0.0      // Celsius offset for thermistor
#define SENSOR_OFFSET      -5.0     // Celsius offset for sensor

// B constant 25C/85C = 3435 K
// B constant 25C/50C = 3380 K
#define BETA               3380     // B constant from datasheet

static esp_adc_cal_characteristics_t *adc_chars;
static const adc1_channel_t thermistor_channel = ADC1_CHANNEL_0;     //GPIO34 if ADC1
static const adc1_channel_t sensor_channel = ADC1_CHANNEL_1;         //GPIO35 if ADC1
static const adc_bits_width_t width = ADC_WIDTH_BIT_DEFAULT;
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;