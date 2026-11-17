#include "Obd2Parser.h"

twai_message_t Obd2Parser::createQuery(uint8_t pid) {
    twai_message_t message;
    message.identifier = 0x7DF; // Broadcast ID for OBD-II requests
    message.extd = 0;           // Standard 11-bit ID
    message.rtr = 0;            // Data frame
    message.data_length_code = 8;
    
    message.data[0] = 0x02; // Number of bytes of the request (Mode + PID)
    message.data[1] = 0x01; // Mode 01: Get current data
    message.data[2] = pid;  // Dedicated PID code
    message.data[3] = 0x55; // Padding
    message.data[4] = 0x55;
    message.data[5] = 0x55;
    message.data[6] = 0x55;
    message.data[7] = 0x55;

    return message;
}

bool Obd2Parser::parseResponse(const twai_message_t &message, OBD2Data &data) {
    // 1. Parse standard OBD-II responses (ID 0x7E8, Service 01 -> 0x41)
    if (message.identifier == 0x7E8 && message.data_length_code >= 4 && message.data[1] == 0x41) {
        uint8_t pid = message.data[2];
        data.lastPid = pid;

        switch (pid) {
            case 0x0C: { // Engine RPM
                data.rpm = ((uint16_t)message.data[3] * 256 + message.data[4]) / 4;
                data.rpmValid = true;
                return true;
            }
            case 0x0D: { // Vehicle Speed (km/h)
                data.speed = message.data[3];
                data.speedValid = true;
                return true;
            }
            case 0x05: { // Coolant Temperature (°C)
                data.coolTemp = (int)message.data[3] - 40;
                data.coolTempValid = true;
                return true;
            }
            case 0x04: { // Calculated Engine Load (%)
                data.engineLoad = (uint8_t)((message.data[3] * 100) / 255);
                data.loadValid = true;
                return true;
            }
            case 0x0F: { // Intake Air Temperature (°C)
                data.intakeTemp = (int)message.data[3] - 40;
                data.intakeTempValid = true;
                return true;
            }
            case 0x11: { // Throttle Position (%)
                data.throttlePos = (uint8_t)((message.data[3] * 100) / 255);
                data.throttleValid = true;
                return true;
            }
            case 0x42: { // Control Module Voltage (V)
                data.batteryVoltage = ((uint16_t)message.data[3] * 256 + message.data[4]) / 1000.0f;
                data.voltageValid = true;
                return true;
            }
            case 0x2F: { // Fuel Tank Level (%)
                data.fuelLevel = (uint8_t)((message.data[3] * 100) / 255);
                data.fuelValid = true;
                return true;
            }
            default:
                return true;
        }
    }

    // 2. Parse Mercedes W169 Native Bus frames & DA 6F / 0x238 / 0x300 / 0x328 frames
    if (message.identifier == 0xDA6F && message.data_length_code >= 8) {
        data.da6fValid = true;
        data.da6f_b0 = message.data[0]; data.da6f_b1 = message.data[1];
        data.da6f_b2 = message.data[2]; data.da6f_b3 = message.data[3];
        data.da6f_b4 = message.data[4]; data.da6f_b5 = message.data[5];
        data.da6f_b6 = message.data[6]; data.da6f_b7 = message.data[7];
        return true;
    }

    if (message.identifier == 0x238 && message.data_length_code >= 8) {
        data.id238Valid = true;
        data.id238_b0 = message.data[0]; data.id238_b1 = message.data[1];
        data.id238_b2 = message.data[2]; data.id238_b3 = message.data[3];
        data.id238_b4 = message.data[4]; data.id238_b5 = message.data[5];
        data.id238_b6 = message.data[6]; data.id238_b7 = message.data[7];
        return true;
    }

    if (message.identifier == 0x300 && message.data_length_code >= 8) {
        data.id300Valid = true;
        data.id300_b0 = message.data[0]; data.id300_b1 = message.data[1];
        data.id300_b2 = message.data[2]; data.id300_b3 = message.data[3];
        data.id300_b4 = message.data[4]; data.id300_b5 = message.data[5];
        data.id300_b6 = message.data[6]; data.id300_b7 = message.data[7];
        return true;
    }

    if (message.identifier == 0x328 && message.data_length_code >= 8) {
        data.id328Valid = true;
        data.id328_b0 = message.data[0]; data.id328_b1 = message.data[1];
        data.id328_b2 = message.data[2]; data.id328_b3 = message.data[3];
        data.id328_b4 = message.data[4]; data.id328_b5 = message.data[5];
        data.id328_b6 = message.data[6]; data.id328_b7 = message.data[7];
        return true;
    }

    if (message.identifier == 0x200 && message.data_length_code >= 3) {
        data.nativeGasPedal = message.data[1];
    }

    return false;
}
