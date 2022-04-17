// Luan Banh
// Temperature reading from thermistor and sensor

#include "thermistor.h"
#include "sensor.h"
#include "sleep.h"

void app_main(void)
{
    // Initial setup
    ++bootCount;
    printf("Boot number: %i\n", bootCount);

    //Print the wakeup reason for ESP32
    print_wakeup_reason();

    // Setup timer to wakeup board
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(thermistor_channel, atten);
    adc1_config_channel_atten(sensor_channel, atten);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    //Setup variables
    char tCelsius [33];            // Hold temperature in celsius

    // Get the temperature for thermistor and sensor
    // Average them and convert from double to int
    PRINTF("\n-----\n");
    itoa ((thermistor_read_temp() + sensor_read_temp())/2 * 100.0, tCelsius, 10);

    // Enable Flash (aka non-volatile storage, NVS)
    // The WiFi stack uses this to store some persistent information
    ESP_ERROR_CHECK(nvs_flash_init());

    // Initialize the network software stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the event loop, a separate thread which checks for events
    // (like WiFi being connected or receiving an MQTT message) and dispatches
    // functions to handle them.
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Now connect to WiFi
    ESP_ERROR_CHECK(example_connect());

    // Initialize the MQTT client
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = BROKER_URI,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
    

    printf("Sending MQTT message...\n");
    esp_mqtt_client_publish(client, "Crusty_Crab/temperature", tCelsius, 0, 1, 0);

    // Deep sleep
    PRINTF("Going to deep sleep now\n");
    esp_deep_sleep_start();
}