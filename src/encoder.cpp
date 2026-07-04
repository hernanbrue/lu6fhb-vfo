#include "encoder.h"
#include "config.h"
#include <Arduino.h>

namespace Encoder {

namespace {

// Tabla de transicion de cuadratura. Indexada por (estado_anterior << 2 |
// estado_actual), donde cada estado es {A,B}. Devuelve +1, -1 o 0 segun
// si la transicion es valida y en que sentido.
const int8_t QUAD_TABLE[16] = {
    0, -1,  1,  0,
    1,  0,  0, -1,
   -1,  0,  0,  1,
    0,  1, -1,  0
};

volatile uint8_t s_lastState = 0;
volatile int8_t  s_accum = 0; // acumula sub-pasos entre flancos validos

// Estado del pulsador (leido por polling, no por ISR: los botones no
// necesitan la latencia de una interrupcion y asi evitamos antirrebote
// dentro de una ISR, que es fragil).
bool     s_buttonWasDown = false;
uint32_t s_buttonDownAt  = 0;
uint32_t s_lastButtonChangeAt = 0;

void IRAM_ATTR onEncoderChange() {
    uint8_t a = digitalRead(PIN_ENC_A);
    uint8_t b = digitalRead(PIN_ENC_B);
    uint8_t state = (a << 1) | b;
    uint8_t idx = (s_lastState << 2) | state;
    int8_t dir = QUAD_TABLE[idx];
    s_lastState = state;

    if (dir != 0) {
        s_accum += dir;
        // 4 flancos por paso mecanico en un EC11 tipico de 20 PPR
        if (s_accum >= 4) {
            s_accum = 0;
            pushEventFromISR(EventType::ENCODER_STEP, +1);
        } else if (s_accum <= -4) {
            s_accum = 0;
            pushEventFromISR(EventType::ENCODER_STEP, -1);
        }
    }
}

} // namespace

void begin() {
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);

    s_lastState = (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B);

    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), onEncoderChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), onEncoderChange, CHANGE);
}

void pollButton() {
    uint32_t now = millis();
    bool isDown = (digitalRead(PIN_ENC_SW) == LOW);

    if (isDown != s_buttonWasDown) {
        // posible cambio de estado: exigir que se sostenga por encima
        // del tiempo de antirrebote antes de confirmarlo
        if (now - s_lastButtonChangeAt < BUTTON_DEBOUNCE_MS) {
            return;
        }
        s_lastButtonChangeAt = now;
        s_buttonWasDown = isDown;

        if (isDown) {
            s_buttonDownAt = now;
        } else {
            uint32_t heldFor = now - s_buttonDownAt;
            if (heldFor >= BUTTON_LONG_PRESS_MS) {
                pushEvent(EventType::ENCODER_BUTTON_LONG);
            } else {
                pushEvent(EventType::ENCODER_BUTTON_SHORT);
            }
        }
    }
}

} // namespace Encoder
