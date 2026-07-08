#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// Pines - ESP32 clasico (WROOM-32, "38 pines"/"ESP32-S" generico)
// ---------------------------------------------------------------------
// Se decidio descartar el soporte para ESP32-S3 por ahora y quedarse
// solo con el clasico. Si en algun momento se retoma el S3, el commit
// "Soportar ESP32 clasico ademas del S3" en git tiene la version con
// deteccion automatica via CONFIG_IDF_TARGET_ESP32S3 - se puede
// recuperar desde ahi en vez de rehacerla.
//
// Evitados a proposito:
//  - GPIO34-39: solo entrada, sin pull-up interno (no sirven para el
//    encoder, que necesita pull-up)
//  - GPIO0/2/12/15: strapping pins
//  - GPIO6-11: ocupados por la flash interna
#define PIN_ENC_A      32
#define PIN_ENC_B      33
#define PIN_ENC_SW     25
#define PIN_I2C_SDA    21
#define PIN_I2C_SCL    22
#define PIN_KEYPAD_ADC 34  // ADC1_CH6 en el clasico

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
#define VFO_FREQ_DEFAULT  145000000ULL   // TEMPORAL: prueba en 2m. Volver a 7100000ULL (40m)

// Pasos de sintonia seleccionables con el pulsador del encoder.
// El indice activo se guarda en AppManager.
static const uint32_t VFO_STEP_TABLE[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
#define VFO_STEP_TABLE_LEN (sizeof(VFO_STEP_TABLE) / sizeof(VFO_STEP_TABLE[0]))

// Tiempo para distinguir pulsacion larga del pulsador (ms)
#define BUTTON_LONG_PRESS_MS 600

// Antirrebote del pulsador (ms)
#define BUTTON_DEBOUNCE_MS 30

// Periodo del tick periodico usado para refresco de display y housekeeping
#define TICK_PERIOD_MS 100

// Titulo de la aplicacion, usado como cabecera en la pantalla del menu
#define APP_TITLE "LU6FHB VFO"
