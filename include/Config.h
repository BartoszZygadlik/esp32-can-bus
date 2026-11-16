#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "driver/twai.h"

// ============================================================================
// CAN BUS AND TWAI SETTINGS
// ============================================================================
#define CAN_TX_PIN GPIO_NUM_5
#define CAN_RX_PIN GPIO_NUM_4

// WI-FI ACCESS POINT SETTINGS
#define WIFI_AP_SSID "ESP32-CAN-Analyzer"
#define WIFI_AP_PASS "" // Leave empty for open network or 8+ characters

#endif // CONFIG_H
