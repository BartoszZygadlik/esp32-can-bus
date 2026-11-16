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
    // Engine ECU response has ID 0x7E8 and Service 01 (byte 1 == 0x41)
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
            default:
                return true;
        }
    }
    return false;
}
