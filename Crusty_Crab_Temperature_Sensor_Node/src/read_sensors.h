// Luan Banh
// Temperature reading from sensor, G-NIMO-005
// Temperature reading from thermistor, B57350V2104F460
// Battery ADC reading
// BATTERY_SENSOR_EN_PIN is 7
// TEMP_SENSOR_EN_PIN is 10

#pragma once

#include "sensors_config.h"

struct ADC_DATA {
    int    adcSamplesTherm[SAMPLE_NUMBER];  // Array to hold therm voltage measurement
    int    adcSamplesSensor[SAMPLE_NUMBER];  // Array to hold sensor voltage measurement
    int    adcSamplesBattery[SAMPLE_NUMBER];  // Array to hold battery voltage measurement
};

#define ADC_DATA struct ADC_DATA

void read_sensors(ADC_DATA *data)
{
    // Set TEMP_SENSOR_EN_PIN
    gpio_set_direction(TEMP_SENSOR_EN_PIN, GPIO_MODE_OUTPUT);
    REG_WRITE(GPIO_OUT_W1TS_REG, BIT10);

    // Clear BATTERY_SENSOR_EN_PIN to act as GND to get current flowing
    gpio_set_direction(BATTERY_SENSOR_EN_PIN, GPIO_MODE_OUTPUT);
    REG_WRITE(GPIO_OUT_W1TC_REG, BIT7);

    //Wait a bit before taking samples
    vTaskDelay(SENSORS_WAIT / portTICK_PERIOD_MS);

    // Sample ADC1 for SAMPLE_NUMBER times
    for (int i = 0; i < SAMPLE_NUMBER; i++) 
    {
      data->adcSamplesTherm[i] = adc1_get_raw(thermistor_channel); // read from pin and store
      data->adcSamplesSensor[i] = adc1_get_raw(sensor_channel); // read from pin and store
      data->adcSamplesBattery[i] = adc1_get_raw(battery_sensor_channel); // read from pin and store
      vTaskDelay(pdMS_TO_TICKS(10)); // wait 10 milliseconds
    }

    // Clear TEMP_SENSOR_EN_PIN
    REG_WRITE(GPIO_OUT_W1TC_REG, BIT10);

    // Set BATTERY_SENSOR_EN_PIN as floating pin to stop current from flowing
    gpio_set_direction(BATTERY_SENSOR_EN_PIN, GPIO_MODE_INPUT);
}