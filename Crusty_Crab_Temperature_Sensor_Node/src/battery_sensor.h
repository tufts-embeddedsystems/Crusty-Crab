// Luan Banh
// Battery ADC reading
// BATTERY_SENSOR_EN_PIN is 7

#pragma once

#include "sensors_config.h"

uint8_t battery_read(int adcSamples[SAMPLE_NUMBER])
{
    double      voltageAverage = 0;         // Holds the average voltage measurement
    double      adcAverage = 0;             // Holds the average voltage measurement
    uint8_t     percentage = 0;             // Holds the batterry percentage

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

    if (voltageAverage < 1400) // 0% if less than 1.4V . . .
            percentage = 0;
    else if (voltageAverage > 2000) // 100% if more than 2V . . .
        percentage = 100;
    else
        percentage = (voltageAverage - DEAD_BATTERY_VOLT) / (MAX_BATTERY_VOLT*30) - 10000;

    PRINTF("Battery voltage: %lf V, Percentage: %d %%\n", voltageAverage / 1000.0, percentage);

    return percentage;
}