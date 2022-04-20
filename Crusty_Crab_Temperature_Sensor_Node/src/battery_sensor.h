// Luan Banh
// Battery ADC reading

#pragma once

#include "sensors_config.h"

RTC_DATA_ATTR int battery_percentage = 100;

int battery_read()
{
    double voltageAverage = 0;         // Holds the average voltage measurement
    double adcAverage = 0;             // Holds the average voltage measurement
    int    adcSamples[SAMPLE_NUMBER];  // Array to hold each voltage measurement

    // Set BATTERY_SENSOR_EN_PIN to act as GND to get current flowing
    gpio_set_direction(BATTERY_SENSOR_EN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BATTERY_SENSOR_EN_PIN, 0);

    //Wait a bit before taking samples
    vTaskDelay(BATTERY_WAIT / portTICK_PERIOD_MS);

    // Sample ADC1 for battery for SAMPLE_NUMBER times
    for (int i = 0; i < SAMPLE_NUMBER; i++)
    {
        adcSamples[i] = adc1_get_raw(battery_sensor_channel); // read from pin and store
        vTaskDelay(pdMS_TO_TICKS(10));                        // wait 10 milliseconds
    }

    // Set BATTERY_SENSOR_EN_PIN as floating pin to stop current from flowing
    gpio_set_direction(BATTERY_SENSOR_EN_PIN, GPIO_MODE_INPUT);

    // Calculate the average voltage
    for (int i = 0; i < SAMPLE_NUMBER; i++)
    {
        voltageAverage += esp_adc_cal_raw_to_voltage(adcSamples[i], adc_chars); // add all samples up . . .
        adcAverage += adcSamples[i];                                            // add all samples up . . .
    }
    voltageAverage /= SAMPLE_NUMBER; // . . . average it w/ divide
    adcAverage /= SAMPLE_NUMBER;
    if (adcAverage == 0)
        voltageAverage = 0;

    PRINTF("Battery voltage: %lf V\n", adcAverage, voltageAverage / 1000.0);

    return (voltageAverage / 1000.0) / MAX_BATTERY_VOLTAGE * 100.0;
}