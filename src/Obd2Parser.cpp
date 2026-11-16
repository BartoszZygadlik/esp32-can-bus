#include "Obd2Parser.h"

twai_message_t Obd2Parser::createQuery(uint8_t pid) {
    twai_message_t message;
    message.identifier = 0x7DF; // Broadcast ID dla zapytań OBD-II
    message.extd = 0;           // Standardowe 11-bitowe ID
    message.rts = 0;
    message.data_length_code = 8;
    
    message.data[0] = 0x02; // Liczba bajtów zapytania (Mode + PID)
    message.data[1] = 0x01; // Mode 01: Pobierz aktualne dane
    message.data[2] = pid;  // Dedykowany kod PID
    message.data[3] = 0x55; // Wypełnienie (padding)
    message.data[4] = 0x55;
    message.data[5] = 0x55;
    message.data[6] = 0x55;
    message.data[7] = 0x55;

    return message;
}

bool Obd2Parser::parseResponse(const twai_message_t &message, OBD2Data &data) {
    // Odpowiedź ECU silnika ma ID 0x7E8 oraz Service 01 (bajt 1 == 0x41)
    if (message.identifier == 0x7E8 && message.data_length_code >= 4 && message.data[1] == 0x41) {
        uint8_t pid = message.data[2];
        data.lastPid = pid;

        switch (pid) {
            case 0x0C: { // Obroty silnika (RPM)
                data.rpm = ((uint16_t)message.data[3] * 256 + message.data[4]) / 4;
                data.rpmValid = true;
                return true;
            }
            case 0x0D: { // Prędkość pojazdu (km/h)
                data.speed = message.data[3];
                data.speedValid = true;
                return true;
            }
            case 0x05: { // Temperatura płynu chłodzącego (°C)
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
