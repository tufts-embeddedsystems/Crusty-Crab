// Luan Banh
// main.c
// Implementation of the temperature sensor.
// Wakeup every half an hour
// Bootcount starts at 0 then got up by 1 every wakeup
// First wakeup will send data to server then will send data every 1 hour
// 

#include "thermistor.h"
#include "gnimo_sensor.h"
#include "sleep.h"
#include "battery_sensor.h"
#include "payload.h"
#include "read_sensors.h"

void app_main(void)
{
    // Initial setup
    ++bootCount;
    PRINTF("Boot number: %i\n", bootCount);

    //Print the wakeup reason for ESP32
    print_wakeup_reason();

    // Setup timer to wakeup board
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    // Wake up every half an hour, if not 1 hour, deep sleep
    if (bootCount == 2)
    {
        PRINTF("Not 1 hour yet, going to deep sleep now\n");
        esp_deep_sleep_start();
    }
    bootCount = 1;

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(thermistor_channel, atten);
    adc1_config_channel_atten(sensor_channel, atten);
    adc1_config_channel_atten(battery_sensor_channel, atten);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    // Time
    uint64_t epoch = (uint64_t) time(NULL);

    // Get sensors readings
    ADC_DATA adc_data;
    read_sensors(&adc_data);

    int32_t tSensor = sensor_read_temp(adc_data.adcSamplesSensor);
    int32_t tTherm = thermistor_read_temp(adc_data.adcSamplesTherm);
    uint8_t Percent = battery_read(adc_data.adcSamplesBattery);

    // Update data field
    char *user_data = "OK";
    if (Percent < LOW_BATTERY_LEVEL)
        user_data = "LOW BATTERY";
    int data_len = strlen(user_data);

    //Setup variables
    PACKET packet = {
            epoch,                   // Get the time
            tSensor,                 // Temperature of sensor
            tTherm,                  // Temperature of thermistor
            Percent,                 // Battery percentage
            data_len                 // Data_len
    };       

    // Pass struct to void* and pass that to char*
    void *pVoid = (void *)&packet;
    char *pBuffer = (char *)pVoid;
    pBuffer[sizeof(PACKET)] = '\0';

    // Create the payload from the struct
    char payload[19 + data_len];
    memset(payload, '\0', sizeof(payload));
    memcpy(payload,    pBuffer,    8);
    memcpy(payload+8,  pBuffer+8,  4);
    memcpy(payload+12, pBuffer+12, 4);
    memcpy(payload+16, pBuffer+16, 1);
    memcpy(payload+17, pBuffer+18, 2);
    if (data_len > 0)
        memcpy(payload+19, user_data, data_len);

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
    esp_err_t err = example_connect();

    if (err == ESP_OK)
    {
        // Initialize the MQTT client
        esp_mqtt_client_config_t mqtt_cfg = {
            .uri = BROKER_URI,
        };
        esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
        esp_mqtt_client_start(client);

        PRINTF("Sending MQTT message...\n");
        esp_mqtt_client_publish(client, "nodes/crusty-crab/node9", payload, sizeof(payload), 1, 0);
        print_packet_info(&packet);
    }

    // Deep sleep
    PRINTF("Going to deep sleep now\n");
    esp_deep_sleep_start();
}