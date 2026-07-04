#pragma once
#include "events.h"

// ---------------------------------------------------------------------
// Encoder
// ---------------------------------------------------------------------
// Decodifica el encoder rotativo por interrupcion (fases A/B) y el
// pulsador por polling con antirrebote llamado desde el tick periodico.
// Nunca toca el display ni el sintetizador: solo encola eventos.

namespace Encoder {

// Inicializa pines e interrupciones. Debe llamarse una sola vez en setup().
void begin();

// Debe llamarse desde el manejo del evento TICK (cada TICK_PERIOD_MS).
// Se encarga del antirrebote y de distinguir pulsacion corta/larga
// del pulsador del encoder.
void pollButton();

} // namespace Encoder
