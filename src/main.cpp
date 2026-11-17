#include <Arduino.h>
#include "CanManager.h"
#include "Obd2Parser.h"
#include "WebDashboard.h"

OBD2Data obdData;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n==========================================");
    Serial.println("   ESP32 CAN Bus & OBD-II Analyzer");
    Serial.println("==========================================");
    Serial.println("Work mode: ACTIVE (OBD-II Polling + Native W169 Sniffing)");

    CanManager::init();
    WebDashboard::init();
}

void loop() {
    WebDashboard::handleClients();

    twai_message_t message;
    if (CanManager::receiveMessage(message)) {
        Serial.printf("[%9lu ms] ID: 0x%03lX | DLC: %d | Data: ", millis(), message.identifier, message.data_length_code);
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("%02X ", message.data[i]);
        }
        Serial.println();

        Obd2Parser::parseResponse(message, obdData);
        WebDashboard::broadcastCanFrame(message, obdData);
    }

    // Active OBD-II rotating query for multiple PIDs (RPM, Speed, Coolant Temp, Engine Load, Voltage, Fuel)
    static unsigned long lastQueryTime = 0;
    static uint8_t queryIndex = 0;
    const uint8_t pids[] = {0x0C, 0x0D, 0x05, 0x04, 0x42, 0x2F};
    const uint8_t totalPids = 6;
    
    if (millis() - lastQueryTime > 250) { // Rotate PID every 250ms for snappy updates
        lastQueryTime = millis();
        
        twai_message_t query = Obd2Parser::createQuery(pids[queryIndex]);
        CanManager::transmitMessage(query);
        
        queryIndex = (queryIndex + 1) % totalPids;
    }
}
