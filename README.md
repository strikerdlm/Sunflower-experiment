# Sunflower Greenhouse Monitor

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)  
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#)

**Platform:** Wio Terminal by Seeed Studio

A comprehensive greenhouse monitoring system for sunflower growth experiments. Tracks and logs environmental conditions (CO₂, temperature, humidity, soil moisture, and ambient light) with real-time display and SD card logging.

## Table of Contents

- [Installation](#installation)  
- [Usage](#usage)  
- [Hardware Requirements](#hardware-requirements)  
- [Documentation](#documentation)  
- [Contributing](#contributing)  
- [Code of Conduct](#code-of-conduct)  
- [License](#license)

## Installation

### Prerequisites

- Git  
- Arduino IDE (v2.0+) or PlatformIO  
- Arduino libraries:
  - TFT_eSPI  
  - SparkFun_SCD4x_Arduino_Library  
  - Seeed_Arduino_LCD  
  - Seeed_FS  

### Clone Repository

```bash
git clone https://github.com/strikerdlm/Sunflower-experiment.git
cd Sunflower-experiment
```

## Usage

### Arduino IDE

1. Open `src/sunflower.ino` in the Arduino IDE.  
2. Select **Wio Terminal** board (Tools → Board → Seeed → Wio Terminal).  
3. Click **Upload** to flash the firmware.

### PlatformIO

1. Ensure `platformio.ini` is configured for Wio Terminal.  
2. Run:

```bash
platformio run --target upload
```

## Hardware Requirements

- Wio Terminal (Seeed Studio)  
- SCD40/SCD41 CO₂, temperature, and humidity sensor  
- Soil moisture sensor (analog A0)  
- SD card for data logging  

## Documentation

See the `docs/` directory for:

- Installation guide  
- Hardware setup instructions  
- Calibration procedures  
- Troubleshooting guide  

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.

## Code of Conduct

This project follows the [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you agree to abide by its terms.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

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
