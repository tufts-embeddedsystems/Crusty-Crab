// Minimal example of using ESP-MQTT to publish messages to a broker
// Complete documentation with detailed example code is here:
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html

#include "freertos/FreeRTOS.h"
#include "freertos/task.h" // Used for timer delay
#include "nvs_flash.h"
#include "esp_netif.h"
#include "mqtt_client.h"
#include "minimal_wifi.h"

#define BROKER_URI "mqtt://en1-pi.eecs.tufts.edu"

void app_main() {
    // `ESP_ERROR_CHECK` is a macro which checks that the return value of a function is
    // `ESP_OK`.  If not, it prints some debug information and aborts the program.

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

    while(1){
        printf("Sending MQTT message...\n");
        esp_mqtt_client_publish(client, "luan/testing", "hello!", 0, 0, 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}