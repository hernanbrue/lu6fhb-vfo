#include "app_manager.h"
#include "config.h"
#include "vfo_logic.h"
#include "menu_logic.h"
#include "vfo_output_settings.h"
#include "band_selector.h"
#include "si5351_driver.h"
#include "display_driver.h"
#include "encoder.h"

namespace AppManager {

namespace {

enum class Mode { MAIN, MENU };

VfoLogic       s_vfoLogic;
MenuLogic      s_menuLogic;
OutputSettings s_outputSettings;
BandSelector   s_bandSelector;
Mode           s_mode = Mode::MAIN;
bool           s_dirty = true; // fuerza el primer render

void enterMenu() {
    s_mode = Mode::MENU;
    // A proposito NO se llama a s_menuLogic.reset() aca: el menu
    // recuerda la ultima posicion del cursor entre una entrada y otra.
    // Si la ultima vez usaste "Banda", la proxima pulsacion larga te
    // deja ahi mismo, sin tener que volver a navegar desde RF/Potencia.
    s_dirty = true;
}

void exitMenu() {
    s_mode = Mode::MAIN;
    s_dirty = true;
}

// Ejecuta la accion del item actualmente seleccionado en el menu.
void activateSelectedMenuItem() {
    switch (s_menuLogic.selectedItem()) {

        case MenuItem::RF_TOGGLE: {
            bool enabled = s_outputSettings.toggleRf();
            VfoSynth::setOutputEnabled(enabled);
            break;
        }

        case MenuItem::POWER: {
            DriveStrengthMa drive = s_outputSettings.cycleDrive();
            VfoSynth::setDriveStrength(drive);
            break;
        }

        case MenuItem::BAND: {
            s_bandSelector.cycle();
            s_vfoLogic.setFrequency(s_bandSelector.currentFreqHz());
            VfoSynth::setFrequency(s_vfoLogic.frequencyHz());
            // Salida automatica del menu: elegiste banda, arrancas a
            // sintonizar directo, sin pulsacion larga extra.
            exitMenu();
            return; // exitMenu ya marco dirty
        }

        case MenuItem::EXIT:
            exitMenu();
            return; // exitMenu ya marco dirty
    }
    s_dirty = true;
}

} // namespace

void begin() {
    VfoSynth::setFrequency(s_vfoLogic.frequencyHz());
}

void handleEvent(const Event& ev) {
    switch (ev.type) {

        case EventType::ENCODER_STEP:
            if (s_mode == Mode::MAIN) {
                if (s_vfoLogic.stepFrequency(ev.data)) {
                    VfoSynth::setFrequency(s_vfoLogic.frequencyHz());
                    s_dirty = true;
                }
            } else {
                s_menuLogic.moveCursor(ev.data);
                s_dirty = true;
            }
            break;

        case EventType::ENCODER_BUTTON_SHORT:
            if (s_mode == Mode::MAIN) {
                s_vfoLogic.cycleStep();
                s_dirty = true;
            } else {
                activateSelectedMenuItem();
            }
            break;

        case EventType::ENCODER_BUTTON_LONG:
            if (s_mode == Mode::MAIN) {
                enterMenu();
            } else {
                exitMenu();
            }
            break;

        case EventType::TICK:
            Encoder::pollButton();
            if (s_dirty) {
                if (s_mode == Mode::MAIN) {
                    VfoDisplay::render(s_vfoLogic.frequencyHz(), s_vfoLogic.stepHz());
                } else {
                    VfoDisplay::renderMenu(s_menuLogic.cursorIndex(),
                                           s_outputSettings.rfEnabled(),
                                           s_outputSettings.drive(),
                                           s_bandSelector.currentLabel());
                }
                s_dirty = false;
            }
            break;
    }
}

} // namespace AppManager
