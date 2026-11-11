# RTOS-Based Embedded Monitoring System

[![Build Firmware](https://github.com/Vikas-Savakar/RTOS-Based-Embedded-Monitoring-System/actions/workflows/build.yml/badge.svg)](https://github.com/Vikas-Savakar/RTOS-Based-Embedded-Monitoring-System/actions/workflows/build.yml)

ESP32 firmware that uses FreeRTOS tasks to read a temperature/humidity sensor and an analog input, then raise alarms when values go out of range. Runs in the Wokwi simulator, no hardware needed.

**[▶ Run it in your browser (Wokwi)](https://wokwi.com/projects/480422369979282433)** — no install needed. Click the green Start button.

## What it does

- Reads a DHT22 (temperature, humidity) and a potentiometer (ADC) in a dedicated task
- Passes readings to a control task through a FreeRTOS queue
- Turns on a green LED when normal, a red LED and buzzer on alarm or sensor fault
- Prints status over UART at 115200 baud

| Item | Details |
|---|---|
| MCU | ESP32 DevKit |
| Sensor / input | DHT22, potentiometer on GPIO34 |
| Outputs | Green LED, red LED, buzzer |
| Tasks | Sensor Acquisition, Control Logic, UART Monitor |

## Run locally (optional)

1. Install [VS Code](https://code.visualstudio.com/), the **Wokwi** extension and Arduino IDE.
2. Clone the repo and build the firmware:
   ```powershell
   git clone https://github.com/Vikas-Savakar/RTOS-Based-Embedded-Monitoring-System.git
   cd RTOS-Based-Embedded-Monitoring-System
   .\scripts\build-wokwi.ps1
   ```
3. Open `diagram.json` in VS Code and start the simulator.

Or open `Embedded_RTOS_Project1.ino` in Arduino IDE and upload to an ESP32.

## Status

Simulation-based project. The normal-operation case has been run in Wokwi. It is not a field-tested controller. More detail is in [`docs/`](docs).
