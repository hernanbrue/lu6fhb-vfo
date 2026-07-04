#include "si5351_driver.h"
#include "config.h"
#include <si5351.h>
#include <Wire.h>

namespace VfoSynth {

namespace {
Si5351 s_si5351;
}

bool begin() {
    // El segundo parametro es la correccion de calibracion en partes por
    // billon (xtal correction). Se ajusta en la release 0.2 (calibracion);
    // por ahora arranca en 0.
    bool ok = s_si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    if (!ok) {
        return false;
    }
    s_si5351.set_freq(VFO_FREQ_DEFAULT * SI5351_FREQ_MULT, SI5351_CLK0);
    s_si5351.output_enable(SI5351_CLK0, 1);
    s_si5351.output_enable(SI5351_CLK1, 0);
    s_si5351.output_enable(SI5351_CLK2, 0);
    return true;
}

void setFrequency(uint64_t freqHz) {
    s_si5351.set_freq(freqHz * SI5351_FREQ_MULT, SI5351_CLK0);
}

void setOutputEnabled(bool enabled) {
    s_si5351.output_enable(SI5351_CLK0, enabled ? 1 : 0);
}

} // namespace VfoSynth
