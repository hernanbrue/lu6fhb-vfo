#pragma once
#include <stdint.h>
#include "config.h"

// ---------------------------------------------------------------------
// VfoLogic
// ---------------------------------------------------------------------
// Estado y reglas del VFO, sin ninguna dependencia de Arduino, FreeRTOS
// ni de los drivers de hardware. Por eso se puede compilar y testear
// en la PC (entorno "native" de PlatformIO) sin tener el ESP32 en la
// mesa. AppManager es la unica capa que conecta esto con el hardware
// real (SI5351, OLED).
//
// Ademas, setFrequency() ya deja el terreno preparado para una fuente
// de entrada futura (ej. WiFi mandando un valor absoluto de frecuencia
// por IP) sin necesidad de tocar esta clase: quien la use solo tiene
// que llamar al mismo metodo que ya usa el encoder indirectamente.

class VfoLogic {
public:
    VfoLogic(uint64_t initialFreqHz = VFO_FREQ_DEFAULT,
              uint32_t initialStepIndex = 2);

    // Aplica un paso de sintonia (+1 o -1), respetando los limites de
    // banda y el paso actual. Devuelve true si la frecuencia cambio.
    bool stepFrequency(int32_t direction);

    // Pasa al siguiente paso de sintonia de VFO_STEP_TABLE (ciclico).
    void cycleStep();

    // Fija una frecuencia absoluta (clampeada a los limites de banda).
    // Pensado para una futura entrada por WiFi/CAT; el encoder no la
    // usa hoy, pero la logica de clamp es la misma para cualquier
    // fuente de entrada.
    bool setFrequency(uint64_t freqHz);

    uint64_t frequencyHz() const { return m_freqHz; }
    uint32_t stepHz() const { return VFO_STEP_TABLE[m_stepIndex]; }
    uint32_t stepIndex() const { return m_stepIndex; }

private:
    uint64_t m_freqHz;
    uint32_t m_stepIndex;

    uint64_t clamp(uint64_t freqHz) const;
};
