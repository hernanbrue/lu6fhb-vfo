#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// Eventos del sistema
// ---------------------------------------------------------------------
// Todo lo que pasa en el VFO se modela como un evento que se encola
// desde una ISR o desde una tarea, y se procesa en un unico lugar
// (AppManager::handleEvent). Ningun modulo llama directamente a otro:
// el encoder no conoce el display, el display no conoce el encoder.
// Esto es lo que reemplaza al loop() con polling continuo.

enum class EventType : uint8_t {
    ENCODER_STEP,        // data: +1 o -1 (direccion de giro)
    ENCODER_BUTTON_SHORT, // pulsacion corta: cambia el paso de sintonia
    ENCODER_BUTTON_LONG,  // pulsacion larga: reservado para menu futuro
    TICK                  // pulso periodico (refresco de display, etc.)
};

struct Event {
    EventType type;
    int32_t   data;   // significado depende de 'type'
};

// Cola de eventos global (FreeRTOS). Definida en main.cpp.
// Cualquier modulo que genere eventos (encoder, timer, futuro CAT/web)
// encola aca; solo AppManager la lee.
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
extern QueueHandle_t g_eventQueue;

// Encola un evento desde contexto normal (task/loop).
inline void pushEvent(EventType type, int32_t data = 0) {
    Event ev{type, data};
    xQueueSend(g_eventQueue, &ev, 0);
}

// Encola un evento desde una ISR.
inline void pushEventFromISR(EventType type, int32_t data = 0) {
    Event ev{type, data};
    BaseType_t woken = pdFALSE;
    xQueueSendFromISR(g_eventQueue, &ev, &woken);
    if (woken) portYIELD_FROM_ISR();
}
