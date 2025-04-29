#ifndef CONFIG_H
#define CONFIG_H

// Sensor and logging configuration
static const int SOIL_PIN = A0;
static const unsigned long LOG_INTERVAL_MS = 10UL * 60UL * 1000UL;  // 10 minutes
static const unsigned long SCREEN_INT_MS = 2000UL;                 // 2 seconds
static const unsigned long SCD_TIMEOUT_MS = 5500UL;                // 5.5 seconds

// Layout positions
static const uint16_t Y_TITLE      = 0;   // headline “Greenhouse Monitor”
static const uint16_t Y_LABELS     = 40;  // start of live-value labels
static const uint16_t Y_STATS_BASE = 170; // top of statistics table
static const uint16_t X_LABELS     = 0;   // left margin for labels
static const uint16_t X_VALS       = 90;  // x for main values
static const uint16_t X_SOIL_LABEL = 160; // x for Soil label
static const uint16_t X_SOIL_VAL   = 210; // x for Soil value

#endif // CONFIG_H

