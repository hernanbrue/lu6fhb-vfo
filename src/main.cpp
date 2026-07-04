#include <Arduino.h>
#include <Wire.h>
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
void IRAM_ATTR onTimer(void* arg) {
    pushEventFromISR(EventType::TICK, 0);
}

hw_timer_t* s_timer = nullptr;

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

    // Timer de hardware disparando cada TICK_PERIOD_MS.
    s_timer = timerBegin(0, 80, true); // prescaler 80 -> 1 tick = 1us
    timerAttachInterruptArg(s_timer, onTimer, nullptr);
    timerAlarmWrite(s_timer, TICK_PERIOD_MS * 1000, true);
    timerAlarmEnable(s_timer);
}

void loop() {
    Event ev;
    // Bloquea hasta que haya un evento; el loop no hace polling de nada.
    if (xQueueReceive(g_eventQueue, &ev, portMAX_DELAY) == pdTRUE) {
        AppManager::handleEvent(ev);
    }
}
