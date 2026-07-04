#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// Display (OLED 128x64, SSD1306 por I2C)
// ---------------------------------------------------------------------
// Igual que con el sintetizador: unico punto de contacto con la
// libreria grafica. AppManager le pasa datos (frecuencia, paso), nunca
// coordenadas de pixeles ni llamadas a Adafruit_GFX directamente.

namespace VfoDisplay {

bool begin();

// Redibuja la pantalla completa con el estado actual del VFO.
// Se llama solo cuando algo cambio (no en cada iteracion del loop).
void render(uint64_t freqHz, uint32_t stepHz);

} // namespace VfoDisplay
