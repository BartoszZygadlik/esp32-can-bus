#ifndef WEB_DASHBOARD_H
#define WEB_DASHBOARD_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "Config.h"
#include "Obd2Parser.h"

class WebDashboard {
public:
    static void init();
    static void handleClients();
    static void broadcastCanFrame(const twai_message_t &message, const OBD2Data &obdData);

private:
    static WebServer server;
    static WebSocketsServer webSocket;
    static void handleRoot();
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif // WEB_DASHBOARD_H
