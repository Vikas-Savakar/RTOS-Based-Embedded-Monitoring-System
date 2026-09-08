# RTOS-Based Embedded Monitoring System

[![Build Firmware](https://github.com/Vikas-Savakar/RTOS-Based-Embedded-Monitoring-System/actions/workflows/build.yml/badge.svg)](https://github.com/Vikas-Savakar/RTOS-Based-Embedded-Monitoring-System/actions/workflows/build.yml)

Simulation-based embedded firmware project using an ESP32, FreeRTOS tasks, a DHT22 sensor, an analog process-value input, UART monitoring, and alarm outputs.

This repository is designed to be easy to understand, reproduce, and discuss in an embedded-systems placement interview.

## Project at a glance

| Item | Details |
|---|---|
| MCU | ESP32 DevKit |
| Firmware | Arduino C++ with FreeRTOS APIs |
| Simulation | Wokwi |
| Sensor | DHT22 temperature/humidity |
| Analog input | Potentiometer on ADC GPIO34 |
| Outputs | Green LED, red LED, buzzer |
| Communication | UART at 115200 baud |
| Inter-task communication | FreeRTOS queue |
| Tasks | Sensor Acquisition, Control Logic, UART Monitor |

## What the project demonstrates

- Periodic sensor acquisition using a dedicated FreeRTOS task
- Priority-based task scheduling
- Inter-task communication using a queue
- Threshold-based alarm control
- Sensor-fault detection
- ADC input monitoring
- UART-based runtime diagnostics
- Modular firmware structure with configuration constants
- Simulation-first development and verification

## System architecture

```text
                    +----------------------+
                    |      ESP32 MCU       |
                    |    FreeRTOS Kernel   |
                    +----------+-----------+
                               |
        +----------------------+----------------------+
        |                      |                      |
        v                      v                      v
+---------------+      +---------------+      +---------------+
| Sensor Task   | ---> | Control Task  |      | Monitor Task  |
| DHT22 + ADC   |      | Alarm logic   |      | UART output   |
+-------+-------+      +-------+-------+      +-------+-------+
        |                      |                      |
        +---- FreeRTOS Queue --+                      |
                               |                      |
                         +-----+------+               |
                         | LEDs/Buzzer |               |
                         +------------+               |
                                                      |
                                                Serial Monitor
```

The sensor task updates a one-element queue with the latest sample. The control task reads the latest sample and drives the alarm outputs. The monitor task reports the current state through UART.

## Pin map

| Function | ESP32 pin |
|---|---:|
| DHT22 data | GPIO4 |
| Potentiometer ADC | GPIO34 |
| Green LED | GPIO2 |
| Red LED | GPIO15 |
| Buzzer | GPIO27 |
| UART | USB serial / UART0 |

### Alarm logic

The system reports `ALARM` when either:

- temperature is above **32.0 °C**, or
- ADC process value is above **3000**.

A DHT22 read failure is reported as `SENSOR FAULT` and also activates the alarm outputs.

## Quick start — Arduino IDE

1. Install **Arduino IDE**.
2. Install the **ESP32** board package from Espressif.
3. Install the library **DHT sensor library for ESPx** (DHTesp).
4. Open `Embedded_RTOS_Project1.ino`.
5. Select an ESP32 board such as **ESP32 Dev Module**.
6. Compile and upload to a compatible ESP32 board.
7. Open Serial Monitor at **115200 baud**.

The project does not require PlatformIO.

## Quick start — Wokwi + VS Code

Wokwi needs a compiled firmware image before starting the local simulator.

### 1. Install

- Visual Studio Code
- Wokwi for VS Code extension
- Arduino IDE (or Arduino CLI)

### 2. Build the firmware

From the project folder in PowerShell:

```powershell
.\scripts\build-wokwi.ps1
```

The script creates the `build/` folder and generates the firmware and ELF files required by Wokwi.

### 3. Start the simulator

Open `diagram.json` in VS Code and start the Wokwi simulator.

The repository already contains `diagram.json` and `wokwi.toml`, so the circuit configuration is version-controlled with the source code.

## Build script

`scripts/build-wokwi.ps1` automatically finds Arduino CLI when it is installed through Arduino IDE, installs the required ESP32 core/library if needed, and compiles the project into the `build/` directory.

If Arduino CLI is already available in your PATH, the script can use that installation as well.

## Test plan

See [`docs/TEST_PLAN.md`](docs/TEST_PLAN.md) for the planned verification cases.

The baseline simulation has been run successfully with normal readings. Threshold and fault cases should be executed before claiming them as completed test results.

## Repository structure

```text
RTOS-Based-Embedded-Monitoring-System/
├── Embedded_RTOS_Project1.ino   # Main firmware
├── diagram.json                  # Wokwi circuit
├── wokwi.toml                    # Wokwi firmware configuration
├── scripts/
│   └── build-wokwi.ps1          # Reproducible local build script
├── docs/
│   └── TEST_PLAN.md              # Verification plan and expected behaviour
├── .gitignore
└── README.md
```

Generated build files, PlatformIO folders, local libraries, and IDE caches are intentionally excluded from Git.

## Skills demonstrated

**Embedded C/C++:** pointers, structures, constants, peripheral interfacing, sensor handling, serial debugging

**Microcontrollers:** ESP32 GPIO, ADC, UART, DHT22 interfacing

**RTOS:** tasks, priorities, delays, queues, periodic execution

**Firmware engineering:** fault handling, modular functions, configuration separation, readable naming, reproducible builds

**Tools:** Arduino IDE, Arduino CLI, VS Code, Wokwi, Git, GitHub

## Placement / interview talking points

This project can be discussed as a practical example of:

1. Why separate sensor acquisition, control, and monitoring into RTOS tasks?
2. Why use a queue instead of having every task read the sensor independently?
3. Why is the queue length one?
4. What happens when the DHT22 read fails?
5. How does task priority affect the control task?
6. Why is GPIO34 used for the analog input?
7. What is the difference between a normal reading and an alarm condition?
8. Why was simulation used before hardware deployment?

## Demo / Verification

The baseline Wokwi simulation has been run successfully with normal DHT22 values. The serial monitor reports temperature, humidity, ADC process value, and `NORMAL` status while the green status LED remains active.

For placement discussions, demonstrate the alarm cases live in Wokwi rather than claiming unexecuted test cases as completed.

## Current scope and limitation

This is a **simulation-based training project**. The repository demonstrates firmware behaviour and embedded design concepts using Wokwi. It should not be described as a field-tested industrial controller.

A hardware version could replace the simulated inputs/outputs with physical sensors and actuators while keeping the same basic task structure.

## Author

**Vikas Savakar**
B.Tech Electrical and Electronics Engineering
REVA University
