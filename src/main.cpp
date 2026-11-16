#include <Arduino.h>
#include "CanManager.h"
#include "Obd2Parser.h"
#include "WebDashboard.h"

// TRYB PRACY:
// true  = Tryb CAN Sniffer (TWAI_MODE_LISTEN_ONLY) - 100% bezpieczny!
// false = Tryb Aktywny (TWAI_MODE_NORMAL) - Pozwala wysyłać zapytania OBD-II.
bool listenOnlyMode = true;

OBD2Data obdData;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n==========================================");
    Serial.println("   ESP32 CAN Bus / OBD-II Analyzer");
    Serial.println("==========================================");
    Serial.printf("Tryb pracy: %s\n", listenOnlyMode ? "SNIFFER (Listen-Only)" : "AKTYWNY (OBD-II Polling)");

    CanManager::init(listenOnlyMode);
    WebDashboard::init();
}

void loop() {
    WebDashboard::handleClients();

    twai_message_t message;
    if (CanManager::receiveMessage(message)) {
        Serial.printf("[%9lu ms] ID: 0x%03X | DLC: %d | Data: ", millis(), message.identifier, message.data_length_code);
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("%02X ", message.data[i]);
        }
        Serial.println();

        Obd2Parser::parseResponse(message, obdData);
        WebDashboard::broadcastCanFrame(message, obdData);
    }

    if (!listenOnlyMode) {
        static unsigned long lastQueryTime = 0;
        if (millis() - lastQueryTime > 1000) {
            lastQueryTime = millis();
            twai_message_t query = Obd2Parser::createQuery(0x0C);
            CanManager::transmitMessage(query);
        }
    }
}
