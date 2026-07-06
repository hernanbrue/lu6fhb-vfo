#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// Pines
// ---------------------------------------------------------------------
// CONFIG_IDF_TARGET_ESP32S3 lo define automaticamente el toolchain
// segun el "board" elegido en platformio.ini. Asi, el mismo codigo
// compila para cualquiera de las dos placas sin tocar nada mas: solo
// se elige el environment correcto al compilar/subir
// (pio run -e esp32-s3 -t upload  o  pio run -e esp32-classic -t upload).

#if CONFIG_IDF_TARGET_ESP32S3
    // --- ESP32-S3 N16R8 ---
    // Evitados a proposito:
    //  - GPIO26-32: ocupados por la PSRAM/flash octal del modulo N16R8
    //  - GPIO19/20: USB nativo (D-/D+)
    //  - GPIO0/3/45/46: strapping pins (afectan el modo de arranque)
    #define PIN_ENC_A      4
    #define PIN_ENC_B      5
    #define PIN_ENC_SW     6
    #define PIN_I2C_SDA    8
    #define PIN_I2C_SCL    9
    #define PIN_KEYPAD_ADC 1   // ADC1_CH0 en el S3
#else
    // --- ESP32 clasico (WROOM-32, "38 pines"/"ESP32-S" generico) ---
    // Evitados a proposito:
    //  - GPIO34-39: solo entrada, sin pull-up interno (no sirven para
    //    el encoder, que necesita pull-up)
    //  - GPIO0/2/12/15: strapping pins
    //  - GPIO6-11: ocupados por la flash interna
    #define PIN_ENC_A      32
    #define PIN_ENC_B      33
    #define PIN_ENC_SW     25
    #define PIN_I2C_SDA    21
    #define PIN_I2C_SCL    22
    #define PIN_KEYPAD_ADC 34  // ADC1_CH6 en el clasico
#endif

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
#define VFO_FREQ_MAX_HZ   150000000ULL   // 150 MHz (cubre HF y 2m/145 MHz)
#define VFO_FREQ_DEFAULT  145000000ULL   // TEMPORAL: prueba en 2m. Volver a
                                          // 7100000ULL (40m) cuando termines.

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