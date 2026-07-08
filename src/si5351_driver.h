#pragma once
#include <stdint.h>
#include "vfo_output_settings.h"

// ---------------------------------------------------------------------
// Sintetizador (SI5351A)
// ---------------------------------------------------------------------
// Unico punto de contacto con el chip. Si el dia de manana esto se
// reemplaza por otro sintetizador, este es el unico archivo que cambia:
// AppManager solo conoce setFrequency()/begin(), nunca la libreria del
// chip directamente. Esto es HAL minima: la interfaz nace del uso real
// (un sintetizador, una funcion), no de una abstraccion generica
// anticipada para chips que todavia no estan en la mesa.

namespace VfoSynth {

// Inicializa el I2C y el chip. Devuelve false si no lo detecta.
bool begin();

// Aplica una frecuencia de salida en Hz por el canal CLK0.
void setFrequency(uint64_t freqHz);

// Habilita/deshabilita la salida de RF (util para standby / TX-RX).
void setOutputEnabled(bool enabled);

// Ajusta la corriente de drive del canal CLK0.
void setDriveStrength(DriveStrengthMa drive);

} // namespace VfoSynth
