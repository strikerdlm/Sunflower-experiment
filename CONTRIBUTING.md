# Contributing to Sunflower Greenhouse Monitor

First off, thank you for considering contributing to the Sunflower Greenhouse Monitor project! It's people like you that make this project such a great tool for agricultural research and hobby gardening.

This document provides guidelines and instructions for contributing to this project. By participating in this project, you agree to abide by its terms and the [Code of Conduct](CODE_OF_CONDUCT.md).

## Table of Contents

- [Getting Started](#getting-started)
- [Development Environment](#development-environment)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Reporting Bugs](#reporting-bugs)
- [Testing Guidelines](#testing-guidelines)
- [Commit Message Conventions](#commit-message-conventions)

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```
   git clone https://github.com/YOUR-USERNAME/Sunflower-experiment.git
   cd Sunflower-experiment
   ```
3. **Set up the upstream remote**:
   ```
   git remote add upstream https://github.com/strikerdlm/Sunflower-experiment.git
   ```
4. **Create a branch** for your feature or bugfix:
   ```
   git checkout -b feature/your-feature-name
   ```
   or
   ```
   git checkout -b fix/issue-you-are-fixing
   ```

## Development Environment

### Required Software

1. **Arduino IDE** (2.0+ recommended) or **PlatformIO** extension for VS Code
2. **Git** for version control
3. **Required libraries**:
   - TFT_eSPI
   - SparkFun_SCD4x_Arduino_Library
   - Seeed_Arduino_LCD
   - Seeed_FS
   
### Hardware Setup

For testing your contributions, you'll need:
- Wio Terminal or compatible hardware
- SCD40/SCD41 CO2 sensor
- Soil moisture sensor
- Optional: SD card for logging functionality

### Initial Setup

1. Install all required libraries through the Arduino Library Manager
2. Ensure proper board selection in Arduino IDE (Tools → Board → Seeed → Wio Terminal)
3. Test the basic functionality by uploading the current firmware to your device

## Pull Request Process

1. **Update your fork** with the latest changes from upstream:
   ```
   git fetch upstream
   git merge upstream/main
   ```

2. **Develop your changes** in your feature branch

3. **Test thoroughly** on actual hardware when possible

4. **Submit a pull request** with a clear title and description:
   - Describe what you changed and why
   - Reference any related issues with #issue_number
   - Include screenshots or videos if UI changes were made
   - Mention any breaking changes

5. **Code review process**:
   - Maintainers will review your PR
   - You may need to make additional changes based on feedback
   - Once approved, a maintainer will merge your PR

## Coding Standards

Follow these Arduino and embedded systems best practices:

1. **Code Organization**
   - Use descriptive function and variable names
   - Keep functions small and focused on a single task
   - Organize related functions into logical groups

2. **Memory Management**
   - Be mindful of memory constraints on embedded systems
   - Avoid dynamic memory allocation when possible
   - Use `F()` macro for string literals in `Serial.print()`
   - Prefer static allocation for predictable memory usage

3. **Timing and Performance**
   - Avoid using `delay()` for timing in the main loop
   - Use millis()-based timing for non-blocking operation
   - Keep critical sections short and efficient

4. **Documentation**
   - Comment all functions with a brief description
   - Document parameters and return values
   - Explain complex algorithms or non-obvious code
   - Use consistent comment style

5. **Variable Naming**
   - `camelCase` for variables and functions
   - `UPPER_CASE` for constants and macros
   - Prefix globals with `g_`
   - Descriptive, non-abbreviated names

6. **Formatting**
   - 2-space indentation
   - Braces on their own line
   - Keep line length under 100 characters
   - Use consistent spacing around operators

## Reporting Bugs

When reporting bugs, please include:

1. **Clear description** of the problem
2. **Steps to reproduce** the issue
3. **Expected behavior** vs actual behavior
4. **Hardware configuration** (which sensors, connections)
5. **Environment details**:
   - Arduino IDE version
   - Library versions
   - Board model and firmware version
6. **Error messages** or serial output (if available)
7. **Photos/videos** (for physical/visual issues)

Create an issue using the bug report template on GitHub.

## Testing Guidelines

1. **Test Types**:
   - Unit tests for utility functions
   - Integration tests for sensor interactions
   - End-to-end tests for complete functionality

2. **Hardware Testing**:
   - Always test on actual hardware before submitting PRs
   - Test across different environmental conditions when possible
   - Verify sensor readings against known references

3. **Specific Tests to Run**:
   - Power-on initialization
   - Sensor data acquisition
   - Screen refresh functionality
   - SD card logging (if applicable)
   - Error condition handling

4. **Documentation**:
   - Document test procedures you've followed in your PR
   - Include test results or screenshots when relevant

## Commit Message Conventions

Follow this format for commit messages:

```
<type>(<scope>): <subject>

<body>

<footer>
```

Where:
- **Types**: feat, fix, docs, style, refactor, test, chore
- **Scope**: sensors, display, logging, config, etc.
- **Subject**: Short (50 chars or less) summary
- **Body**: Detailed explanation (optional)
- **Footer**: Reference issues, breaking changes (optional)

Examples:
```
feat(sensors): add support for BME280 temperature sensor

Implements driver and display integration for the BME280 sensor as an 
alternative to existing temperature measurement.

Closes #42
```

```
fix(display): correct CO2 value positioning on screen

Values were overlapping at higher readings above 1000ppm.
```

Thank you for contributing to the Sunflower Greenhouse Monitor project!

