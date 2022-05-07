// Luan Banh
// Use to test the boards, mostly check temp readings and battery reading

#include "thermistor.h"
#include "gnimo_sensor.h"
#include "sleep.h"
#include "battery_sensor.h"
#include "payload.h"
#include "read_sensors.h"

void wakeup_routine(void)
{
    // Initial setup
    ++bootCount;
    PRINTF("Boot number: %i\n", bootCount);

    //Print the wakeup reason for ESP32
    print_wakeup_reason();

    // Setup timer to wakeup board
    esp_sleep_enable_timer_wakeup(5 * uS_TO_S_FACTOR);

    //Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(thermistor_channel, atten);
    adc1_config_channel_atten(sensor_channel, atten);
    adc1_config_channel_atten(battery_sensor_channel, atten);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
}

void turn_off_onboard_led(void)
{
    // Permanately turn RED led off
    gpio_set_direction(5, GPIO_MODE_OUTPUT);
    REG_WRITE(GPIO_OUT_W1TS_REG, BIT5);
}

void blink_vdd(void)
{
    gpio_set_direction(TEMP_SENSOR_EN_PIN, GPIO_MODE_OUTPUT);
    PRINTF("vdd on\n");
    REG_WRITE(GPIO_OUT_W1TS_REG, BIT10);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    PRINTF("vdd off\n");
    REG_WRITE(GPIO_OUT_W1TC_REG, BIT10);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void app_main(void)
{
    // Turn off the onboard led
    turn_off_onboard_led();

    // Bootcount and setup adc characteristics
    wakeup_routine();
    
    ADC_DATA adc_data;

    // Get sensors readings
    // blink_vdd();
    read_sensors(&adc_data);
    sensor_read_temp(adc_data.adcSamplesSensor);
    thermistor_read_temp(adc_data.adcSamplesTherm);
    battery_read(adc_data.adcSamplesBattery);
    // int adc = adc1_get_raw(sensor_channel); // read from pin and store
    // PRINTF("ADC: %d, Votlage: %d mV\n", adc, esp_adc_cal_raw_to_voltage(adc, adc_chars));
    // vTaskDelay(10000 / portTICK_PERIOD_MS);
    
    // Deep sleep
    PRINTF("Going to deep sleep now\n");
    esp_deep_sleep_start();

    // Time
    // uint64_t epoch = (uint64_t) time(NULL) + 1650993508;

    // ADC_DATA adc_data;
    // read_sensors(&adc_data);

    // int32_t tSensor = sensor_read_temp(adc_data.adcSamplesSensor);
    // int32_t tTherm = thermistor_read_temp(adc_data.adcSamplesTherm);
    // uint8_t Percent = battery_read(adc_data.adcSamplesBattery);

    // Update data field
    // char *user_data = "OK";
    // if (Percent < LOW_BATTERY_LEVEL)
    //     user_data = "LOW BATTERY";
    // int data_len = strlen(user_data);

    // //Setup variables
    // PACKET packet = {
    //         epoch,                   // Get the time
    //         tSensor,                 // Temperature of sensor
    //         tTherm,                  // Temperature of thermistor
    //         Percent,                 // Battery percentage
    //         data_len                 // Data_len
    // };       
    // // print_packet_info(&packet);

    // // Pass struct to void* and pass that to char*
    // void *pVoid = (void *)&packet;
    // char *pBuffer = (char *)pVoid;
    // pBuffer[sizeof(PACKET)] = '\0';

    // PRINTF("Creating payload\n");
    // char payload[19 + data_len];
    // memset(payload, '\0', sizeof(payload));
    // memcpy(payload,    pBuffer,    8);
    // memcpy(payload+8,  pBuffer+8,  4);
    // memcpy(payload+12, pBuffer+12, 4);
    // memcpy(payload+16, pBuffer+16, 1);
    // memcpy(payload+17, pBuffer+18, 2);
    // if (data_len > 0)
    //     memcpy(payload+19, user_data, data_len);

    // // Enable Flash (aka non-volatile storage, NVS)
    // // The WiFi stack uses this to store some persistent information
    // ESP_ERROR_CHECK(nvs_flash_init());

    // // Initialize the network software stack
    // ESP_ERROR_CHECK(esp_netif_init());

    // // Initialize the event loop, a separate thread which checks for events
    // // (like WiFi being connected or receiving an MQTT message) and dispatches
    // // functions to handle them.
    // ESP_ERROR_CHECK(esp_event_loop_create_default());

    // // Now connect to WiFi
    // esp_err_t err = example_connect();

    // if (err == ESP_OK)
    // {
    //     // Initialize the MQTT client
    //     esp_mqtt_client_config_t mqtt_cfg = {
    //         .uri = BROKER_URI,
    //     };
    //     esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    //     esp_mqtt_client_start(client);

    //     PRINTF("Sending MQTT message...\n");
    //     esp_mqtt_client_publish(client, "nodes/crusty-crab/test2", payload, sizeof(payload), 1, 0);
    // }

    // Deep sleep
    // PRINTF("Going to deep sleep now\n");
    // esp_deep_sleep_start();
}