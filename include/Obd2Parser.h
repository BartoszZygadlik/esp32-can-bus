#ifndef OBD2_PARSER_H
#define OBD2_PARSER_H

#include <Arduino.h>
#include "driver/twai.h"

// Struct storing decoded OBD-II and Mercedes W169 native bus data (including DA 6F, 0x238, 0x300, 0x328)
struct OBD2Data {
    bool rpmValid = false;
    uint16_t rpm = 0;

    bool speedValid = false;
    uint8_t speed = 0;

    bool coolTempValid = false;
    int coolTemp = 0;

    bool loadValid = false;
    uint8_t engineLoad = 0;

    bool intakeTempValid = false;
    int intakeTemp = 0;

    bool voltageValid = false;
    float batteryVoltage = 0.0f;

    bool fuelValid = false;
    uint8_t fuelLevel = 0;

    bool throttleValid = false;
    uint8_t throttlePos = 0;

    // Mercedes W169 Native Bus Data & Frame Analysis (DA 6F, 0x238, 0x300, 0x328, 0x200)
    uint16_t nativeGasPedal = 0;

    bool da6fValid = false;
    uint8_t da6f_b0 = 0, da6f_b1 = 0, da6f_b2 = 0, da6f_b3 = 0;
    uint8_t da6f_b4 = 0, da6f_b5 = 0, da6f_b6 = 0, da6f_b7 = 0;

    bool id238Valid = false;
    uint8_t id238_b0 = 0, id238_b1 = 0, id238_b2 = 0, id238_b3 = 0;
    uint8_t id238_b4 = 0, id238_b5 = 0, id238_b6 = 0, id238_b7 = 0;

    bool id300Valid = false;
    uint8_t id300_b0 = 0, id300_b1 = 0, id300_b2 = 0, id300_b3 = 0;
    uint8_t id300_b4 = 0, id300_b5 = 0, id300_b6 = 0, id300_b7 = 0;

    bool id328Valid = false;
    uint8_t id328_b0 = 0, id328_b1 = 0, id328_b2 = 0, id328_b3 = 0;
    uint8_t id328_b4 = 0, id328_b5 = 0, id328_b6 = 0, id328_b7 = 0;

    uint8_t lastPid = 0;
};

class Obd2Parser {
public:
    static twai_message_t createQuery(uint8_t pid);
    static bool parseResponse(const twai_message_t &message, OBD2Data &data);
};

#endif // OBD2_PARSER_H
