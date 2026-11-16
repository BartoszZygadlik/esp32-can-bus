# Podsumowanie Rozmowy - Projekt ESP32 CAN Bus & OBD-II Analyzer

## 1. Wybrane komponenty i środowisko
* **Mikrokontroler:** ESP32 (wybrany z uwagi na wbudowany kontroler CAN TWAI, wysoką wydajność oraz Wi-Fi/Bluetooth).
* **Transceiver CAN:** SN65HVD230 (zasilanie 3.3V).
* **Środowisko:** VS Code + wtyczka PlatformIO IDE (`framework = arduino`, natywna obsługa ESP32 `driver/twai.h`).
* **Lokalizacja projektu:** `C:\Users\zygaebar\esp32-can-bus`

---

## 2. Schemat połączeń (Pinout)

### ESP32 <--> SN65HVD230:
* **ESP32 3V3**  -->  **SN65HVD230 VCC**
* **ESP32 GND**  -->  **SN65HVD230 GND**
* **ESP32 GPIO 5 (TX)**  -->  **SN65HVD230 CTX**
* **ESP32 GPIO 4 (RX)**  -->  **SN65HVD230 CRX**

### SN65HVD230 <--> Gniazdo OBD-II samochodu:
* **SN65HVD230 CANH**  -->  **OBD-II Pin 6** (CAN High)
* **SN65HVD230 CANL**  -->  **OBD-II Pin 14** (CAN Low)

---

## 3. Stan Projektu (Pliki w repozytorium)

### Pliki w projekcie:
1. `platformio.ini` - Konfiguracja płytki `esp32dev`, monitora 115200 baud.
2. `src/main.cpp` - Kod z natywną obsługą ESP32 TWAI:
   * **Tryb Sniffer (`listenOnlyMode = true`):** Bezpieczne nasłuchiwanie magistrali CAN bez wysyłania ramek/ACK.
   * **Tryb Aktywny (`listenOnlyMode = false`):** Odpytywanie OBD-II (PID 0x0C - RPM, PID 0x0D - Speed, PID 0x05 - Temp).
3. `README.md` - Opis uruchomienia i podłączenia.

---

## 4. Proponowane kolejne kroki rozwoju
1. **Web Dashboard (Wi-Fi):** Wyświetlanie ramek CAN / wskaźników żywo na stronie WWW.
2. **Emulacja ELM327 (Bluetooth):** Łączność z aplikacjami Car Scanner / Torque.
3. **Rejestrator CAN (Karta SD):** Zapis ramek CAN do plików `.csv` / `.log`.
4. **Dedykowany parser ramek pod konkretny model auta.**
