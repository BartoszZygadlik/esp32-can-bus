#ifndef OBD2_PARSER_H
#define OBD2_PARSER_H

#include <Arduino.h>
#include "driver/twai.h"

// Struktura przechowująca zdekodowane dane OBD-II
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
    // Generuje ramkę zapytania OBD-II (Service 01) dla podanego PID
    static twai_message_t createQuery(uint8_t pid);

    // Parsuje ramkę odpowiedzi i aktualizuje przekazaną strukturę danych
    static bool parseResponse(const twai_message_t &message, OBD2Data &data);
};

#endif // OBD2_PARSER_H
