// Luan Banh
// Temperature reading from thermistor
// B57350V2104F460
// TEMP_SENSOR_EN_PIN is 10

#pragma once

#include "sensors_config.h"

int32_t thermistor_read_temp(int adcSamples[SAMPLE_NUMBER])
{
    //Setup variables
    double rThermistor = 0;            // Holds thermistor resistance value
    double tKelvin     = 0;            // Holds calculated temperature
    double tCelsius    = 0;            // Hold temperature in celsius
    double adcAverage  = 0;            // Holds the average voltage measurement

    // Calculate the average
    for (int i = 0; i < SAMPLE_NUMBER; i++)
    {
      adcAverage += adcSamples[i]; // add all samples up . . .
    }
    adcAverage /= SAMPLE_NUMBER; // . . . average it w/ divide
    //PRINTF("Average adc: %lf\n", adcAverage);

    // Calculate the thermistor resistance
    rThermistor = BALANCE_RESISTOR * ((MAX_ADC / adcAverage) - 1);

    // Calculate the temperature in Kelvin
    tKelvin = (BETA * ROOM_TEMP) /
              (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));

    // Convert Kelvin to Celsius and print
    tCelsius = tKelvin - 273.15 + THERMISTOR_OFFSET; // convert kelvin to celsius
    PRINTF("Thermistor Temperature: %lf C\n", tCelsius);

    return tCelsius * 1000;
}