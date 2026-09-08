# Verification Test Plan

This document records the verification cases for the RTOS-based monitoring firmware. A test is marked complete only after the corresponding simulation or hardware run has actually been performed.

| ID | Test | Input / Action | Expected result |
|---|---|---|---|
| T01 | Startup | Start the simulation | Startup message and RTOS task creation message appear on UART |
| T02 | Normal operation | DHT22 at normal values, ADC below limit | Green LED ON, red LED OFF, buzzer OFF, UART reports `NORMAL` |
| T03 | Temperature alarm | Set DHT22 temperature above 32 °C | Red LED ON, buzzer ON, UART reports `ALARM` |
| T04 | Process-value alarm | Increase potentiometer above ADC 3000 | Red LED ON, buzzer ON, UART reports `ALARM` |
| T05 | Recovery | Return both inputs below limits | System returns to `NORMAL` |
| T06 | Sensor fault | Cause the DHT22 read to fail | UART reports `SENSOR FAULT`; alarm outputs activate |
| T07 | Monitoring responsiveness | Observe UART while changing inputs | Runtime readings continue without stopping the simulator |

## Baseline result

T01 and T02 have been demonstrated in the Wokwi simulation during development. The baseline run produced normal DHT22 readings and `NORMAL` status.

T03-T07 are the next verification cases to execute and capture before presenting them as completed results.
