# Sunflower Greenhouse Monitor
**Platform:** Wio Terminal by Seeed Studio

## Overview

This project is a comprehensive greenhouse monitoring system built on the Wio Terminal platform. It is designed to track and log environmental conditions for sunflower growth experiments, with a focus on limited sunlight and directed blue wavelength light (less than 1000 lux). The system integrates multiple sensors and provides real-time display, statistics, and SD card logging.

## Hardware

- **Wio Terminal** (Seeed Studio)
- **SCD-41 Sensor** (I2C): Measures CO₂, temperature, and relative humidity.
- **Soil Moisture Sensor** (Capacitive, v1.2): Connected to analog port (A0).
- **Light Sensor** (Wio Terminal built-in, analog): Measures ambient light (lux).
- **SD Card**: For data logging.

## Features

- **Live Display:** Real-time readings of CO₂, temperature, humidity, soil moisture, and light intensity on the Wio Terminal screen.
- **Rolling Statistics:** Mean, standard deviation, min, and max for each parameter, updated live.
- **Data Logging:** Periodic logging of all sensor data to SD card in CSV format, including timestamps.
- **Customizable Layout:** Easily adjust screen element positions in code.
- **Robust Sensor Handling:** Automatic recovery for SCD-41 communication issues.
- **RTC Integration:** Accurate timestamping using the onboard real-time clock.

## Latest Firmware (v3.3, 2025-04-27)

The latest code introduces:
- **Non-blocking SCD-41 communication** with watchdog and auto-restart.
- **2-second screen refresh** for near real-time updates.
- **Tunable layout** for all on-screen elements.
- **Expanded statistics** for all measured parameters.
- **Light sensor integration** for tracking ambient lux.
- **Improved SD logging** with robust CSV output.

### Main Loop Highlights

- Reads all sensors (CO₂, Temp, Humidity, Lux, Soil).
- Updates rolling statistics for each parameter.
- Refreshes the display every 2 seconds.
- Logs data to SD card every 10 minutes.
- Handles SCD-41 timeouts and restarts measurement if needed.

### Example Data Log (CSV)

```csv
timestamp,co2,temp,humidity,soil,lux
2025-04-27 12:00:00,400,22.5,45,50,100
2025-04-27 12:00:01,401,22.5,45,51,101
2025-04-27 12:00:02,402,22.5,45,52,102
