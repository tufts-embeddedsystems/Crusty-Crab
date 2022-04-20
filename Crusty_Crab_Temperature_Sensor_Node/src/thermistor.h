// Luan Banh
// Temperature reading from thermistor

#pragma once

#include "sensors_config.h"

double thermistor_read_temp()
{
    //Setup variables
    double rThermistor = 0;            // Holds thermistor resistance value
    double tKelvin     = 0;            // Holds calculated temperature
    double tCelsius    = 0;            // Hold temperature in celsius
    double adcAverage  = 0;            // Holds the average voltage measurement
    int    adcSamples[SAMPLE_NUMBER];  // Array to hold each voltage measurement
    //uint32_t voltage = 0;
 
    // Sample ADC1 for thermistor for SAMPLE_NUMBER times
    for (int i = 0; i < SAMPLE_NUMBER; i++) 
    {
      adcSamples[i] = adc1_get_raw(thermistor_channel); // read from pin and store
      //voltage = esp_adc_cal_raw_to_voltage(adcSamples[i], adc_chars);
      //PRINTF("Thermistor Sample(%d) Raw: %d\tVoltage: %dmV\n", i, adcSamples[i], voltage);
      vTaskDelay(pdMS_TO_TICKS(10)); // wait 10 milliseconds
    }

    // Calculate the average
    for (int i = 0; i < SAMPLE_NUMBER; i++)
    {
      adcAverage += adcSamples[i]; // add all samples up . . .
    }
    adcAverage /= SAMPLE_NUMBER; // . . . average it w/ divide
    //PRINTF("Average adc: %lf\n", adcAverage);

    // Calculate the thermistor resistance
    rThermistor = BALANCE_RESISTOR * ((MAX_ADC / adcAverage) - 1);
    //PRINTF("Thermistor resistance: %lf kOhm\n", rThermistor/1000.0);

    // Calculate the temperature in Kelvin
    tKelvin = (BETA * ROOM_TEMP) /
              (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));
    //PRINTF("Thermistor Temperature: %lf K\n", tKelvin);

    // Convert Kelvin to Celsius and print
    tCelsius = tKelvin - 273.15 + THERMISTOR_OFFSET; // convert kelvin to celsius
    PRINTF("Thermistor Temperature: %lf C\n", tCelsius);

    return tCelsius;
}