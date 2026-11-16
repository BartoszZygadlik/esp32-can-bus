#include "WebDashboard.h"

WebServer WebDashboard::server(80);
WebSocketsServer WebDashboard::webSocket(81);

void WebDashboard::init() {
    // Uruchomienie ESP32 jako Access Point
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
    IPAddress IP = WiFi.softAPIP();
    
    Serial.println("\n[Wi-Fi] Access Point uruchomiony!");
    Serial.printf("[Wi-Fi] Nazwa sieci (SSID): %s\n", WIFI_AP_SSID);
    Serial.printf("[Wi-Fi] Adres IP Dashboardu: http://%s\n", IP.toString().c_str());

    // Routing HTTP
    server.on("/", HTTP_GET, handleRoot);
    server.begin();

    // Uruchomienie serwera WebSocket
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("[WebSocket] Serwer uruchomiony na porcie 81.");
}

void WebDashboard::handleClients() {
    server.handleClient();
    webSocket.loop();
}

void WebDashboard::broadcastCanFrame(const twai_message_t &message, const OBD2Data &obdData) {
    // Przygotowanie komunikatu JSON dla WebSocket
    String json = "{";
    json += "\"id\":\"0x" + String(message.identifier, HEX) + "\",";
    json += "\"dlc\":" + String(message.data_length_code) + ",";
    json += "\"data\":\"";
    for (int i = 0; i < message.data_length_code; i++) {
        char buf[4];
        sprintf(buf, "%02X ", message.data[i]);
        json += buf;
    }
    json += "\",";
    json += "\"rpm\":" + String(obdData.rpmValid ? obdData.rpm : 0) + ",";
    json += "\"speed\":" + String(obdData.speedValid ? obdData.speed : 0) + ",";
    json += "\"temp\":" + String(obdData.coolTempValid ? obdData.coolTemp : 0);
    json += "}";

    webSocket.broadcastTXT(json);
}

void WebDashboard::handleRoot() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    html += "<title>ESP32 CAN Bus & OBD-II Analyzer</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; background: #121212; color: #e0e0e0; margin: 0; padding: 20px; }";
    html += "h1, h2 { color: #00bcd4; text-align: center; }";
    html += ".container { max-width: 900px; margin: auto; }";
    html += ".gauges { display: flex; justify-content: space-around; flex-wrap: wrap; margin-bottom: 20px; }";
    html += ".card { background: #1e1e1e; border-radius: 8px; padding: 20px; width: 250px; text-align: center; margin: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }";
    html += ".card h3 { margin: 0 0 10px 0; color: #b0bec5; font-size: 16px; }";
    html += ".value { font-size: 32px; font-weight: bold; color: #4caf50; }";
    html += ".table-container { background: #1e1e1e; border-radius: 8px; padding: 15px; max-height: 400px; overflow-y: auto; }";
    html += "table { width: 100%; border-collapse: collapse; font-family: monospace; font-size: 14px; }";
    html += "th, td { padding: 8px; text-align: left; border-bottom: 1px solid #333; }";
    html += "th { background: #263238; color: #00bcd4; position: sticky; top: 0; }";
    html += ".controls { text-align: center; margin-bottom: 20px; }";
    html += "button { background: #00bcd4; color: #121212; border: none; padding: 10px 20px; font-size: 16px; font-weight: bold; border-radius: 4px; cursor: pointer; }";
    html += "button:hover { background: #008ba3; }";
    html += "</style></head><body>";
    html += "<div class=\"container\">";
    html += "<h1>🚗 ESP32 CAN Bus Dashboard</h1>";
    html += "<div class=\"gauges\">";
    html += "<div class=\"card\"><h3>OBD-II RPM</h3><div class=\"value\" id=\"rpm\">0 RPM</div></div>";
    html += "<div class=\"card\"><h3>Vehicle Speed</h3><div class=\"value\" id=\"speed\">0 km/h</div></div>";
    html += "<div class=\"card\"><h3>Coolant Temp</h3><div class=\"value\" id=\"temp\">0 &deg;C</div></div>";
    html += "</div>";
    html += "<div class=\"controls\"><button onclick=\"clearLog()\">Wyczyść tabelę ramek</button></div>";
    html += "<h2>Rejestr ramek CAN (Live Sniffer)</h2>";
    html += "<div class=\"table-container\"><table id=\"canTable\">";
    html += "<tr><th>Czas (ms)</th><th>ID</th><th>DLC</th><th>Dane</th></tr>";
    html += "</table></div></div>";
    html += "<script>";
    html += "const ws = new WebSocket('ws://' + window.location.hostname + ':81/');";
    html += "ws.onmessage = function(event) {";
    html += "  const pkt = JSON.parse(event.data);";
    html += "  document.getElementById('rpm').innerText = pkt.rpm + ' RPM';";
    html += "  document.getElementById('speed').innerText = pkt.speed + ' km/h';";
    html += "  document.getElementById('temp').innerText = pkt.temp + ' °C';";
    html += "  const table = document.getElementById('canTable');";
    html += "  const row = table.insertRow(1);";
    html += "  row.innerHTML = '<td>' + new Date().toLocaleTimeString() + '</td><td>' + pkt.id + '</td><td>' + pkt.dlc + '</td><td>' + pkt.data + '</td>';";
    html += "  if (table.rows.length > 50) { table.deleteRow(table.rows.length - 1); }";
    html += "};";
    html += "function clearLog() {";
    html += "  const table = document.getElementById('canTable');";
    html += "  while(table.rows.length > 1) { table.deleteRow(1); }";
    html += "}";
    html += "</script></body></html>";

    server.send(200, "text/html", html);
}

void WebDashboard::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_CONNECTED) {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[WebSocket] Klient #%u połączył się z IP: %s\n", num, ip.toString().c_str());
    } else if (type == WStype_DISCONNECTED) {
        Serial.printf("[WebSocket] Klient #%u rozłączył się\n", num);
    }
}
