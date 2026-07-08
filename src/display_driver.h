#pragma once
#include <stdint.h>
#include "vfo_output_settings.h"

// ---------------------------------------------------------------------
// Display (OLED 128x64, SSD1306 por I2C)
// ---------------------------------------------------------------------
// Igual que con el sintetizador: unico punto de contacto con la
// libreria grafica. AppManager le pasa datos (frecuencia, paso), nunca
// coordenadas de pixeles ni llamadas a Adafruit_GFX directamente.

namespace VfoDisplay {

bool begin();

// Muestra APP_TITLE centrado, a pantalla completa. Se llama una sola
// vez al arrancar, antes de entrar al loop principal.
void renderSplash();

// Redibuja la pantalla completa con el estado actual del VFO.
// Se llama solo cuando algo cambio (no en cada iteracion del loop).
void render(uint64_t freqHz, uint32_t stepHz);

// Redibuja la pantalla del menu: 4 items fijos (RF, Potencia, Banda,
// Salir), con el cursor en cursorIndex y los valores actuales.
void renderMenu(uint8_t cursorIndex, bool rfEnabled, DriveStrengthMa drive,
                 const char* bandLabel);

} // namespace VfoDisplay
