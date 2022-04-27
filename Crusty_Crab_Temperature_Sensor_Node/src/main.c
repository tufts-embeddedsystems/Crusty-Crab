// Luan Banh
// Temperature reading from thermistor and sensor

#include "thermistor.h"
#include "gnimo_sensor.h"
#include "sleep.h"
#include "battery_sensor.h"
#include "payload.h"

void app_main(void)
{
    // Initial setup
    ++bootCount;
    PRINTF("Boot number: %i\n", bootCount);

    //Print the wakeup reason for ESP32
    print_wakeup_reason();

    // Setup timer to wakeup board
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(thermistor_channel, atten);
    adc1_config_channel_atten(sensor_channel, atten);
    adc1_config_channel_atten(battery_sensor_channel, atten);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    // Time
    uint64_t epoch = (uint64_t) time(NULL) + 1650993508;

    // Get sensors readings
    int32_t tSensor = sensor_read_temp();
    int32_t tTherm = thermistor_read_temp();
    uint8_t Percent = battery_read();

    // Update data field
    char *data = "Alright, hope this works";
    int data_len = strlen(data);

    //Setup variables
    PACKET packet = {
            epoch,                   // Get the time
            tSensor,                 // Temperature of sensor
            tTherm,                  // Temperature of thermistor
            Percent,                 // Battery percentage
            data_len                 // Data_len
    };       
    print_packet_info(&packet);

    // Pass struct to void* and pass that to char*
    void *pVoid = (void *)&packet;
    char *pBuffer = (char *)pVoid;
    pBuffer[sizeof(PACKET)] = '\0';

    PRINTF("Creating payload\n");
    char payload[19 + data_len];
    memset(payload, '\0', sizeof(payload));
    memcpy(payload,    pBuffer,    8);
    memcpy(payload+8,  pBuffer+8,  4);
    memcpy(payload+12, pBuffer+12, 4);
    memcpy(payload+16, pBuffer+16, 1);
    memcpy(payload+17, pBuffer+18, 2);
    if (data_len > 0)
        memcpy(payload+19, data, data_len);
    
    print_payload_info(payload, data_len);

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
        esp_mqtt_client_publish(client, "nodes/crusty-crab/test3", payload, sizeof(payload), 1, 0);
    }

    // Deep sleep
    PRINTF("Going to deep sleep now\n");
    esp_deep_sleep_start();
}