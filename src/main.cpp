#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include "config.h"
#include "events.h"
#include "encoder.h"
#include "si5351_driver.h"
#include "display_driver.h"
#include "app_manager.h"

// Cola de eventos global. Declarada extern en events.h.
QueueHandle_t g_eventQueue;

namespace {

// Genera el tick periodico que dispara el antirrebote del pulsador y
// el refresco de display. Es la unica fuente de "tiempo" del sistema;
// todo lo demas es reactivo a eventos.
//
// Uso un timer de SOFTWARE de FreeRTOS (xTimerCreate) en vez del timer
// de HARDWARE de arduino-esp32 (timerBegin/timerAttachInterrupt).
// Motivo: esa API de hardware cambio entre el core 2.x y el 3.x
// (timerBegin paso de 3 parametros a 1, timerAttachInterruptArg
// desaparecio, timerAlarm es nuevo en 3.x) y depender de ella hace que
// el proyecto se rompa cada vez que PlatformIO resuelve una version
// distinta del core. El timer de software de FreeRTOS es identico en
// cualquier version, porque no es parte de arduino-esp32 sino del
// RTOS subyacente. Para un tick de 100ms (antirrebote + refresco de
// display) sobra en precision.
//
// Importante: el callback de un software timer de FreeRTOS corre en el
// contexto de la tarea "Timer Service", NO en una ISR real. Por eso
// acá se usa pushEvent() (la version normal) y no pushEventFromISR().
void onTickTimer(TimerHandle_t /*xTimer*/) {
    pushEvent(EventType::TICK, 0);
}

TimerHandle_t s_tickTimer = nullptr;

} // namespace

void setup() {
    Serial.begin(115200);

    g_eventQueue = xQueueCreate(32, sizeof(Event));

    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    if (!VfoDisplay::begin()) {
        Serial.println("ERROR: no se detecto el OLED en el bus I2C");
    }
    if (!VfoSynth::begin()) {
        Serial.println("ERROR: no se detecto el SI5351 en el bus I2C");
    }

    Encoder::begin();
    AppManager::begin();

    s_tickTimer = xTimerCreate(
        "vfo_tick",
        pdMS_TO_TICKS(TICK_PERIOD_MS),
        pdTRUE,           // autoreload
        nullptr,          // ID de timer, no lo usamos
        onTickTimer
    );

    if (s_tickTimer == nullptr) {
        Serial.println("ERROR: no se pudo crear el timer de tick");
    } else {
        xTimerStart(s_tickTimer, 0);
    }
}

void loop() {
    Event ev;
    // Bloquea hasta que haya un evento; el loop no hace polling de nada.
    if (xQueueReceive(g_eventQueue, &ev, portMAX_DELAY) == pdTRUE) {
        AppManager::handleEvent(ev);
    }
}
