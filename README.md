# 🚗 ESP32 CAN Bus & OBD-II Analyzer

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Framework-orange?style=flat-square&logo=platformio)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/Board-ESP32%20WROOM-blue?style=flat-square&logo=espressif)](https://www.espressif.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg?style=flat-square)](https://opensource.org/licenses/MIT)
[![Build Status](https://github.com/zygaebar/esp32-can-bus/actions/workflows/ci.yml/badge.svg)](https://github.com/zygaebar/esp32-can-bus/actions)

Zaawansowany, modularny analizator magistrali **CAN Bus** oraz skaner diagnostyczny **OBD-II** oparty na mikrokontrolerze **ESP32 (NodeMCU 38-pin / ESP-WROOM-32)** oraz transceiverze **SN65HVD230 (VP230)**. Projekt wyposażony jest w bezprzewodowy **Web Dashboard** działający w czasie rzeczywistym poprzez sieć Wi-Fi (Access Point) oraz protokół WebSocket.

---

## ⚠️ Klauzula Bezpieczeństwa (Safety Disclaimer)

> **UWAGA:** Interakcja z systemami elektronicznymi pojazdu (magistrala CAN) wiąże się z ryzykiem zakłócenia pracy modułów ECU. 
> * Zaleca się rozpoczynanie pracy w trybie **Sniffer (Listen-Only)**, który jest w 100% pasywny i bezpieczny.
> * Autor projektu nie ponosi żadnej odpowiedzialności za ewentualne uszkodzenia pojazdu, błędy komputera (Check Engine) ani straty materialne wynikające z niewłaściwego podłączenia lub użycia sprzętu.

---

## 🔌 Schemat Podłączenia (Pinout)

### 1. ESP32 NodeMCU (38-pin) <--> Moduł SN65HVD230 (VP230)
> ⚡ **WAŻNE:** Moduł SN65HVD230 zasasilany jest wyłącznie napięciem **3.3V**.

| Moduł VP230 | Płytka ESP32 NodeMCU (38-pin) | Opis |
| :--- | :--- | :--- |
| **VCC / 3V3** | **3V3** | Zasilanie 3.3V |
| **GND** | **GND** | Wspólna masa |
| **CTX (TX)** | **GPIO 5** | Nadawanie CAN (TX) |
| **CRX (RX)** | **GPIO 4** | Odbiór CAN (RX) |

### 2. Moduł VP230 <--> Gniazdo OBD-II w Samochodzie
| Moduł VP230 | Gniazdo OBD-II | Opis |
| :--- | :--- | :--- |
| **CANH** | **Pin 6** | CAN High |
| **CANL** | **Pin 14** | CAN Low |
| **GND** *(Zalecane)* | **Pin 4 / 5** | Masa pojazdu |

---

## 🚀 Szybki Start i Uruchomienie

1. Otwórz projekt w **VS Code** z zainstalowanym rozszerzeniem **PlatformIO**.
2. Podłącz ESP32 przez USB do komputera.
3. Kliknij **Build** i **Upload** w PlatformIO.
4. Po uruchomieniu w aucie wyszukaj sieć Wi-Fi w telefonie:
   * **Nazwa (SSID):** `ESP32-CAN-Analyzer`
5. Otwórz przeglądarkę i wpisz adres: **`http://192.168.4.1`**

---

## 📜 Licencja
Projekt udostępniony na licencji **MIT** - zobacz plik [LICENSE](LICENSE) po więcej szczegółów.
