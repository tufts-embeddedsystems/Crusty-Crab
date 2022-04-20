// Luan Banh
// Temperature reading from sensor
// G-NIMO-005

#pragma once

#include "sensors_config.h"

double sensor_read_temp()
{
    double tCelsius    = 0;            // Hold temperature in celsius
    double voltageAverage  = 0;        // Holds the average voltage measurement
    double adcAverage  = 0;            // Holds the average voltage measurement
    int    adcSamples[SAMPLE_NUMBER];  // Array to hold each voltage measurement
    //uint32_t voltage = 0;
 
    // Sample ADC1 for thermistor for SAMPLE_NUMBER times
    for (int i = 0; i < SAMPLE_NUMBER; i++) 
    {
      adcSamples[i] = adc1_get_raw(sensor_channel); // read from pin and store
      //voltage = esp_adc_cal_raw_to_voltage(adcSamples[i], adc_chars);
      //PRINTF("Sensor Sample(%d) Raw: %d\tVoltage: %dmV\n", i, adcSamples[i], voltage);
      vTaskDelay(pdMS_TO_TICKS(10)); // wait 10 milliseconds
    }

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
    //PRINTF("Average voltage: %lf V\n", voltageAverage/1000);

    // Convert voltage into temperature for sensor
    // T / °C = VSDM / VDD x 175.72 – 46.85
    tCelsius = ((voltageAverage / 1000.0) / VDD) * 175.72 - 46.85 + SENSOR_OFFSET;
    PRINTF("Sensor temperature: %lf C\n", tCelsius);

    return tCelsius;
}