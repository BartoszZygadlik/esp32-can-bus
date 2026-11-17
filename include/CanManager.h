#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <Arduino.h>
#include "driver/twai.h"
#include "Config.h"

class CanManager {
public:
    static bool init();
    static bool receiveMessage(twai_message_t &message);
    static bool transmitMessage(const twai_message_t &message);
};

#endif // CAN_MANAGER_H
