#include "app_manager.h"
#include "config.h"
#include "vfo_logic.h"
#include "si5351_driver.h"
#include "display_driver.h"
#include "encoder.h"

namespace AppManager {

namespace {

VfoLogic s_logic;
bool s_dirty = true; // fuerza el primer render

} // namespace

void begin() {
    VfoSynth::setFrequency(s_logic.frequencyHz());
}

void handleEvent(const Event& ev) {
    switch (ev.type) {

        case EventType::ENCODER_STEP:
            if (s_logic.stepFrequency(ev.data)) {
                VfoSynth::setFrequency(s_logic.frequencyHz());
                s_dirty = true;
            }
            break;

        case EventType::ENCODER_BUTTON_SHORT:
            s_logic.cycleStep();
            s_dirty = true;
            break;

        case EventType::ENCODER_BUTTON_LONG:
            // Reservado: entrada a menu (memorias, calibracion) en
            // releases futuras. Por ahora no hace nada.
            break;

        case EventType::TICK:
            Encoder::pollButton();
            if (s_dirty) {
                VfoDisplay::render(s_logic.frequencyHz(), s_logic.stepHz());
                s_dirty = false;
            }
            break;
    }
}

} // namespace AppManager
