#include "CanManager.h"

bool CanManager::init() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        CAN_TX_PIN, 
        CAN_RX_PIN, 
        TWAI_MODE_NORMAL
    );
    
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); 
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        Serial.println("[ERROR] Failed to install TWAI driver!");
        return false;
    }
    Serial.println("[TWAI] Driver installed successfully.");

    if (twai_start() != ESP_OK) {
        Serial.println("[ERROR] Failed to start CAN bus!");
        return false;
    }
    Serial.println("[TWAI] CAN bus started and ready.");
    return true;
}

bool CanManager::receiveMessage(twai_message_t &message) {
    return (twai_receive(&message, 0) == ESP_OK);
}

bool CanManager::transmitMessage(const twai_message_t &message) {
    return (twai_transmit(&message, pdMS_TO_TICKS(100)) == ESP_OK);
}
