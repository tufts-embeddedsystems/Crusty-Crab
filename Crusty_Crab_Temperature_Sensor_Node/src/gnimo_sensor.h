// Luan Banh
// Temperature reading from sensor
// G-NIMO-005
// TEMP_SENSOR_EN_PIN is 10

#pragma once

#include "sensors_config.h"

int32_t sensor_read_temp(int adcSamples[SAMPLE_NUMBER])
{
    double tCelsius    = 0;            // Hold temperature in celsius
    double voltageAverage  = 0;        // Holds the average voltage measurement
    double adcAverage  = 0;            // Holds the average voltage measurement

    // Calculate the average voltage
    for (int i = 0; i < SAMPLE_NUMBER; i++)
    {
      voltageAverage += esp_adc_cal_raw_to_voltage(adcSamples[i], adc_chars); // add all samples up . . .
      adcAverage += adcSamples[i]; // add all samples up . . .
    }
    voltageAverage /=  SAMPLE_NUMBER; // . . . average it w/ divide
    adcAverage /= SAMPLE_NUMBER;
    if (adcAverage == 0)
      voltageAverage = 0;
    // PRINTF("Average sensor voltage: %lf mV\n", voltageAverage);

    // Convert voltage into temperature for sensor
    // T / °C = VSDM / VDD x 175.72 – 46.85
    tCelsius = ((voltageAverage / 1000.0) / VDD) * 175.72 - 46.85 + SENSOR_OFFSET;
    PRINTF("Sensor temperature: %lf C\n", tCelsius);

    return tCelsius * 1000;
}