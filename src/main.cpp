#include <Arduino.h>
#include "CanManager.h"
#include "Obd2Parser.h"
#include "WebDashboard.h"

// WORKING MODE:
// true  = CAN Sniffer Mode (TWAI_MODE_LISTEN_ONLY) - 100% safe!
// false = Active Mode (TWAI_MODE_NORMAL) - Allows sending OBD-II queries.
bool listenOnlyMode = true;

OBD2Data obdData;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n==========================================");
    Serial.println("   ESP32 CAN Bus / OBD-II Analyzer");
    Serial.println("==========================================");
    Serial.printf("Work mode: %s\n", listenOnlyMode ? "SNIFFER (Listen-Only)" : "ACTIVE (OBD-II Polling)");

    // 1. Initialize CAN Bus
    CanManager::init(listenOnlyMode);

    // 2. Initialize Wi-Fi and Web Dashboard
    WebDashboard::init();
}

void loop() {
    // 1. Handle Web / WebSocket clients
    WebDashboard::handleClients();

    // 2. Receive CAN frames
    twai_message_t message;
    if (CanManager::receiveMessage(message)) {
        // Logging to Serial
        Serial.printf("[%9lu ms] ID: 0x%03X | DLC: %d | Data: ", millis(), message.identifier, message.data_length_code);
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("%02X ", message.data[i]);
        }
        Serial.println();

        // Parse OBD-II response
        Obd2Parser::parseResponse(message, obdData);

        // Broadcast frame to all clients via WebSocket
        WebDashboard::broadcastCanFrame(message, obdData);
    }

    // 3. In active mode, send OBD-II query every 1 second
    if (!listenOnlyMode) {
        static unsigned long lastQueryTime = 0;
        if (millis() - lastQueryTime > 1000) {
            lastQueryTime = millis();
            
            // Query for Engine RPM (PID 0x0C)
            twai_message_t query = Obd2Parser::createQuery(0x0C);
            CanManager::transmitMessage(query);
        }
    }
}
