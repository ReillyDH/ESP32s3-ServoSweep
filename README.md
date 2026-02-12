# ESP32s3-ServoSweep
A simple Servo Sweep project for the Freenove ESP32-S3 WROOM-1 board.


## Overview 
This project demonstrates how to drive a typical RC Servo motor by sending a PWM signal using the MCPWM driver. This PWM pulses at a 50Hz frequency.

## Hardware Setup
- ESP32s3 Development Board
- RC Servo Motor
- Connections:
    - VCC -> 5V
    - GND -> GND
    - Signal -> GPIO21

## Software
- Built using ESP-IDF
- Tested on ESP32 DevKit V1
- Steps to build and flash:
  1. Clone the repository
  2. `idf.py set-target esp32s3`
  3. `idf.py build`
  4. `idf.py flash monitor`

## Features
- Generates a 50Hz PWM Signal using the MCPWM driver
- Structured, modular code with PWM initialization separated into helper functions