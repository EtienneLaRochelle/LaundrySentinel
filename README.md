# Laundry Sentinel

**A smart IoT monitor for older laundry machines that notifies you on Discord when your cycle is finished.**

![Language](https://img.shields.io/badge/Language-C++-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32-green)
![Status](https://img.shields.io/badge/Status-Prototype-orange)

## Overview
Laundry Sentinel upgrades "dumb" appliances into smart devices. Using an **ESP32** and a **Light Sensor (LDR)**, it detects when the "Cycle Finished" LED on a washing machine turns on. It then sends a notification to a specific user on **Discord** to let them know their laundry is ready.

### Key Features
* **User Selection:** Select who is doing laundry via an LCD menu.
* **Smart Detection:** Uses a calibrated light sensor threshold to detect the machine's status LED.
* **Discord Integration:** Sends a direct mention (`@User`) notification when the cycle starts and finishes.
* **Non-Invasive:** Requires no modification to the washing machine's internal electronics.

---

## Wiring & Electronics

| Component | Pin Label | ESP32 GPIO Pin | Description |
| :--- | :--- | :--- | :--- |
| **LCD Screen** | SDA | **GPIO 27** | I2C Data (Custom Pin) |
| | SCL | **GPIO 26** | I2C Clock (Custom Pin) |
| | VCC | 5V / VIN | Power (5V required for contrast) |
| | GND | GND | Ground |
| **Rotary Encoder** | CLK | **GPIO 35** | Encoder Clock |
| | DT | **GPIO 32** | Encoder Data |
| | SW | **GPIO 23** | Button (Active Low) |
| | + | 3.3V | Power |
| | GND | GND | Ground |
| **Light Sensor** | Analog | **GPIO 34** | Analog Input (LDR Divider) |

### Circuit Diagram
```mermaid
graph TD
    ESP32[ESP32 DevKit V1]

    subgraph I2C_LCD [LCD 16x2 I2C]
        VCC_LCD[VCC]
        GND_LCD[GND]
        SDA[SDA]
        SCL[SCL]
    end

    ESP32 -- "5V" --> VCC_LCD
    ESP32 -- "GND" --> GND_LCD
    ESP32 -- "GPIO 27" --> SDA
    ESP32 -- "GPIO 26" --> SCL

    subgraph ENCODER [KY-040 Rotary Encoder]
        CLK[CLK]
        DT[DT]
        SW[SW Button]
        VCC_ENC[+]
        GND_ENC[GND]
    end

    ESP32 -- "GPIO 35" --> CLK
    ESP32 -- "GPIO 32" --> DT
    ESP32 -- "GPIO 23" --> SW
    ESP32 -- "3.3V" --> VCC_ENC
    ESP32 -- "GND" --> GND_ENC

    subgraph LDR_CIRCUIT [Light Sensor Circuit]
        LDR[Photoresistor]
        RES[10k Resistor]
        SIG[Signal]
    end

    POWER_LDR((3.3V)) -- Power --> LDR
    LDR -- Junction --> SIG
    SIG -- Junction --> RES
    RES -- Ground --> GROUND_LDR((GND))
    SIG -- "GPIO 34 (Analog)" --> ESP32