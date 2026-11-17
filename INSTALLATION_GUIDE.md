# ?? ESP32 CAN Bus & OBD-II Analyzer - Complete Installation & Troubleshooting Guide

Welcome to the ultimate guide for setting up, wiring, flashing, and troubleshooting your **ESP32 NodeMCU (38-pin / ESP-WROOM-32)** paired with the **SN65HVD230 (VP230)** transceiver for CAN Bus & OBD-II monitoring.

---

## 1. ?? Hardware Wiring Guide

### Step 1: ESP32 <--> SN65HVD230 (VP230) Module
> ?? **CRITICAL WARNING:** The SN65HVD230 module requires **3.3V** power. Never connect it to the 5V (VIN) pin, as it may permanently damage the ESP32 logic pins.

| VP230 Module Pin | ESP32 NodeMCU Pin (38-pin) | Description |
| :--- | :--- | :--- |
| **VCC / 3V3** | **3.3V** | Power supply |
| **GND** | **GND** | Common ground |
| **CTX (TX)** | **GPIO 5** | CAN Transmit |
| **CRX (RX)** | **GPIO 4** | CAN Receive |

### Step 2: VP230 <--> Vehicle OBD-II Port
| VP230 Module Pin | Vehicle OBD-II Port Pin | Description |
| :--- | :--- | :--- |
| **CANH** | **Pin 6** | CAN High |
| **CANL** | **Pin 14** | CAN Low |
| **GND** *(Recommended)* | **Pin 4 or 5** | Chassis / Signal Ground |

---

## 2. ?? Software Setup & Flashing

### Prerequisites
1. Install **VS Code**.
2. Install the **PlatformIO IDE** extension inside VS Code.
3. Open the project folder (`esp32-can-bus`) in VS Code.

### Building & Uploading the Firmware
Open your terminal in VS Code and run the following commands:
1. **Compile the project:**
   ```powershell
   & "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev
   ```
2. **Upload to ESP32:**
   ```powershell
   & "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev --target upload
   ```

---

## 3. ??? Troubleshooting & Common Issues

### Issue 1: Yellow Exclamation Mark on "CP2102 USB to UART" in Device Manager
* **Symptom:** Windows shows a yellow triangle next to the bridge controller, and flashing fails.
* **Cause:** Missing or outdated USB-to-UART bridge drivers.
* **Solution:**
  1. Download the official **CP210x Universal Windows Driver** from [Silicon Labs](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers).
  2. Extract the downloaded `.zip` file.
  3. Locate the file named **`silabser.inf`** inside the extracted folder.
  4. Right-click on `silabser.inf` and select **Install**.
  5. Reconnect your ESP32 via USB. The yellow exclamation mark will disappear.

---

### Issue 2: `Error: Failed to connect to ESP32: No serial data received`
* **Symptom:** PlatformIO tries to connect (`Connecting......................`), times out, and fails.
* **Causes & Solutions:**
  * **Cause A: Interference from transceiver wiring.**
    * *Solution:* Disconnect the wires connected to **GPIO 4 (RX)** and **GPIO 5 (TX)** during flashing. Reconnect them after a successful upload.
  * **Cause B: Manual Bootloader Entry required.**
    * *Solution:* 
      1. Press and hold the physical **BOOT** button on the ESP32 board.
      2. Press and release the **EN / RST** (Reset) button while still holding **BOOT**.
      3. Release the **BOOT** button.
      4. Immediately run the upload command again.
  * **Cause C: Faulty USB cable.**
    * *Solution:* Many cheap USB cables carry power only and lack data lines (`D+`/`D-`). Switch to a known, high-quality data transmission cable.

---

### Issue 3: `pio : The term 'pio' is not recognized`
* **Symptom:** Running `pio run` in PowerShell results in a command not found error.
* **Solution:**
  * Use the full path to the PlatformIO executable instead:
    ```powershell
    & "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e esp32dev
    ```

---

## 4. ?? Using the Web Dashboard

1. Power up your ESP32 (in the vehicle via a 5V USB car charger or via USB on your PC).
2. On your phone or laptop, open Wi-Fi settings and connect to:
   * **SSID:** `ESP32-CAN-Analyzer`
   * **Password:** *(None / Open)*
3. Open your web browser and go to:
   * **URL:** `http://192.168.4.1`
4. Enjoy real-time OBD-II gauges (RPM, Speed, Coolant Temp) and the Live CAN Sniffer table!
