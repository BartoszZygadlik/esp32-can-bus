#ifndef OBD2_PARSER_H
#define OBD2_PARSER_H

#include <Arduino.h>
#include "driver/twai.h"

// Struct storing decoded OBD-II data
struct OBD2Data {
    bool rpmValid = false;
    uint16_t rpm = 0;

    bool speedValid = false;
    uint8_t speed = 0;

    bool coolTempValid = false;
    int coolTemp = 0;

    uint8_t lastPid = 0;
};

class Obd2Parser {
public:
    // Generates an OBD-II query frame (Service 01) for the given PID
    static twai_message_t createQuery(uint8_t pid);

    // Parses a response frame and updates the provided data struct
    static bool parseResponse(const twai_message_t &message, OBD2Data &data);
};

#endif // OBD2_PARSER_H
