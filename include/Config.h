#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "driver/twai.h"

// ============================================================================
// USTAWIENIA PINÓW I MAGISTRALI CAN (TWAI ESP32)
// ============================================================================
#define CAN_TX_PIN GPIO_NUM_5
#define CAN_RX_PIN GPIO_NUM_4

// USTAWIENIA WI-FI (Access Point)
#define WIFI_AP_SSID "ESP32-CAN-Analyzer"
#define WIFI_AP_PASS "" // Puste = otwarta sieć lub minimum 8 znaków jeśli podane

#endif // CONFIG_H
