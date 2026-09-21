# ESP32 Cold-Chain Monitoring System

An **IoT-ready Cold-Chain Monitoring System** built with an **ESP32 microcontroller** designed to track temperature-sensitive medical supplies (such as insulin) and ensure strict compliance with storage guidelines.

## 🚀 Features

* **Real-Time Temperature Tracking:** Continuously monitors ambient temperature via a **DHT22 sensor**.
* **Threshold Enforcement:** Strictly enforces pharmaceutical cold-chain limits (**2°C to 8°C**).
* **RFID Asset Verification:** Uses an **MFRC522 RFID reader** to scan cargo/medication batches and query their status instantly.
* **Live LCD Interface:** Displays live warehouse monitoring metrics (`Zone: Whse-Alpha`) on a **16x2 I2C LCD screen** when idle, switching to item-specific data upon an RFID scan.
* **Immediate Alerts:** Features dual visual indicators (Green LED for safe, Red LED for breach) and an active buzzer for multi-beep warning patterns during temperature violations.

---

## 🛠️ Hardware Components

| Component | Description |
| :--- | :--- |
| **Microcontroller** | ESP32 NodeMCU |
| **Temperature Sensor** | DHT22 |
| **Display** | 16x2 I2C LCD Display |
| **RFID Module** | MFRC522 Reader / Writer |
| **Indicators** | Active Buzzer, Green LED, Red LED |

---

## 📌 Pin Configuration

* **RFID (SS Pin):** GPIO 27
* **RFID (RST Pin):** GPIO 13
* **DHT22 Sensor:** GPIO 32
* **Buzzer:** GPIO 26
* **Green LED:** GPIO 25
* **Red LED:** GPIO 33
* **I2C SDA / SCL:** GPIO 21 / GPIO 22

---

## ⚙️ How It Works

1. **Idle Monitoring:** The system continually reads the temperature. If it falls within the $2^\circ\text{C}$ to $8^\circ\text{C}$ range, the Green LED is active and the LCD shows live telemetry.
2. **Breach Handling:** If the temperature drifts outside safe limits, the Red LED turns on and a breach flag is raised.
3. **RFID Scans:** Tapping an RFID tag pulls up asset information (`Med: Insulin`), logs the exact temperature at the moment of scanning, and displays either `[SAFE]` or `[BREACH]`. Breaches trigger an audible 3-beep alarm sequence.

---

## 📦 Dependencies / Libraries

Make sure you have the following Arduino libraries installed in your IDE:
* `SPI` (Built-in)
* `Wire` (Built-in)
* `MFRC522` by GithubCommunity
* `LiquidCrystal_I2C` by Frank de Brabander
* `DHT sensor library` by Adafruit
