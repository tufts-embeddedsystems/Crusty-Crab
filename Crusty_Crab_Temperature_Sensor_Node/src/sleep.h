// Luan Banh
// Sleep Modes: deep and light

#pragma once

#include "main_config.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

RTC_DATA_ATTR int bootCount = 0;

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : PRINTF("Wakeup caused by external signal using RTC_IO\n"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : PRINTF("Wakeup caused by external signal using RTC_CNTL\n"); break;
    case ESP_SLEEP_WAKEUP_TIMER : PRINTF("Wakeup caused by timer\n"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : PRINTF("Wakeup caused by touchpad\n"); break;
    case ESP_SLEEP_WAKEUP_ULP : PRINTF("Wakeup caused by ULP program\n"); break;
    default : PRINTF("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}