// Luan Banh
// Temperature reading from thermistor and sensor

#include "thermistor.h"
#include "sensor.h"

#define SAMPLE_FREQUENCY 5000   // Sample every 60 seconds

void app_main(void)
{
    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(thermistor_channel, atten);
    adc1_config_channel_atten(sensor_channel, atten);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    //Setup variables
    double Therm_tCelsius    = 0;            // Hold temperature in celsius
    double Sensor_tCelsius   = 0;            // Hold temperature in celsius

    //Continuously sample ADC1
    while (1) {
        // Get the temperature for thermistor
        printf("\n-----\n");
        Therm_tCelsius = thermistor_read_temp();

        // Get the temperature for sensor
        printf("\n-----\n");
        Sensor_tCelsius = sensor_read_temp();

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(SAMPLE_FREQUENCY));
    }
}