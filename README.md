# RTOS-Based Embedded Monitoring System

A simulation-based embedded systems project developed as part of Vikas Savakar's Embedded System training at Persevex Education Consultancy LLP.

## Project Overview

This project demonstrates a real-time monitoring and control application on an ESP32 using FreeRTOS. Sensor acquisition, control logic, and UART monitoring are handled as separate RTOS tasks.

## Main Features

- ESP32-based embedded system
- FreeRTOS task-based firmware
- DHT22 temperature and humidity monitoring
- Analog process-value input using a potentiometer
- Inter-task communication using a FreeRTOS queue
- Priority-based task execution
- Alarm control using LED and buzzer outputs
- UART monitoring at 115200 baud
- Sensor-fault detection
- Threshold-based control logic

## Hardware / Simulation

The system is developed and tested using the Wokwi online simulator.

Components used in the simulation:
- ESP32 DevKit
- DHT22 sensor
- Potentiometer
- Green LED
- Red LED
- Buzzer

## Software

- Arduino IDE / Arduino-ESP32
- Wokwi
- C/C++
- FreeRTOS APIs available with the ESP32 Arduino environment
- VS Code

## Repository Structure

- `Embedded_RTOS_Project1.ino` - main firmware source
- `diagram.json` - Wokwi circuit definition
- `wokwi.toml` - Wokwi firmware configuration
- `platformio.ini` - PlatformIO project configuration
- `.gitignore` - excludes generated build files

## Current Simulation Result

The simulation has been successfully started and is producing normal readings such as 25.0 °C temperature and 50.0% humidity with the system reporting NORMAL status.

Further threshold and fault test cases will be documented as the project testing is completed.

## Academic Context

The project focuses on applying embedded-systems concepts such as RTOS tasks, scheduling, queues, peripheral interfacing, sensor acquisition, and firmware structure in a practical simulation environment.

## Author

Vikas Savakar  
B.Tech Electrical and Electronics Engineering  
REVA University
