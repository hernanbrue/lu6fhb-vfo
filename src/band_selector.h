#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// BandSelector
// ---------------------------------------------------------------------
// Igual que OutputSettings: pura, sin dependencias de hardware. Cicla
// entre bandas preestablecidas y expone la frecuencia de referencia de
// cada una. AppManager es quien aplica esa frecuencia sobre VfoLogic y
// VfoSynth.

enum class Band : uint8_t {
    M40 = 0,
    M20 = 1,
    M10 = 2,
    M2  = 3
};

constexpr uint8_t BAND_COUNT = 4;

struct BandPreset {
    const char* label;
    uint64_t freqHz;
};

// Frecuencias de referencia de cada banda.
constexpr BandPreset BAND_PRESETS[BAND_COUNT] = {
    {"40m", 7100000ULL},
    {"20m", 14200000ULL},
    {"10m", 28400000ULL},
    {"2m",  145000000ULL},
};

class BandSelector {
public:
    BandSelector() : m_index(0) {}

    // Pasa a la siguiente banda (con wrap) y devuelve la nueva.
    Band cycle();

    Band current() const { return static_cast<Band>(m_index); }
    uint64_t currentFreqHz() const { return BAND_PRESETS[m_index].freqHz; }
    const char* currentLabel() const { return BAND_PRESETS[m_index].label; }

private:
    uint8_t m_index;
};
