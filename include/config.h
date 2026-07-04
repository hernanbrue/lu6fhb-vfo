#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// Pines - ESP32-S3 N16R8
// ---------------------------------------------------------------------
// Evitados a proposito:
//  - GPIO26-32: ocupados por la PSRAM/flash octal en el modulo N16R8
//  - GPIO19/20: USB nativo (D-/D+)
//  - GPIO0/3/45/46: strapping pins (afectan el modo de arranque)
//  - GPIO45/46: ademas fijan la tension de VDD_SPI, no usar como GPIO libre

#define PIN_ENC_A   4
#define PIN_ENC_B   5
#define PIN_ENC_SW  6

#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9

// Reservado para el keypad shield (fase futura, requiere divisor de
// tension propio). No se usa todavia. GPIO1 es ADC1_CH0 en el S3.
#define PIN_KEYPAD_ADC 1

// ---------------------------------------------------------------------
// Display
// ---------------------------------------------------------------------
#define OLED_WIDTH    128
#define OLED_HEIGHT   64
#define OLED_I2C_ADDR 0x3C

// ---------------------------------------------------------------------
// VFO
// ---------------------------------------------------------------------
// Limites de frecuencia de trabajo (Hz). Ajustar segun banda de interes.
#define VFO_FREQ_MIN_HZ   1000000ULL     // 1 MHz
#define VFO_FREQ_MAX_HZ   30000000ULL    // 30 MHz
#define VFO_FREQ_DEFAULT  7100000ULL     // 40m, punto de partida

// Pasos de sintonia seleccionables con el pulsador del encoder.
// El indice activo se guarda en AppManager.
static const uint32_t VFO_STEP_TABLE[] = {1, 10, 100, 1000, 10000, 100000};
#define VFO_STEP_TABLE_LEN (sizeof(VFO_STEP_TABLE) / sizeof(VFO_STEP_TABLE[0]))

// Tiempo para distinguir pulsacion larga del pulsador (ms)
#define BUTTON_LONG_PRESS_MS 600

// Antirrebote del pulsador (ms)
#define BUTTON_DEBOUNCE_MS 30

// Periodo del tick periodico usado para refresco de display y housekeeping
#define TICK_PERIOD_MS 100
