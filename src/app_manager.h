#pragma once
#include "events.h"

// ---------------------------------------------------------------------
// AppManager
// ---------------------------------------------------------------------
// El unico modulo que conoce tanto a VfoSynth como a VfoDisplay.
// Recibe eventos (nunca los genera) y decide que hacer con el estado
// del VFO. Encoder, display y synth no se conocen entre si: todo pasa
// por aca. Si el dia de manana aparece una entrada nueva (CAT por USB,
// interfaz web), solo tiene que encolar los mismos tipos de evento o
// agregar uno nuevo aca - no toca encoder.cpp ni display_driver.cpp.

namespace AppManager {

void begin();
void handleEvent(const Event& ev);

} // namespace AppManager
