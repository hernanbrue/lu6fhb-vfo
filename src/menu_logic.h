#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// MenuLogic
// ---------------------------------------------------------------------
// Igual que VfoLogic: sin dependencias de Arduino/FreeRTOS/hardware,
// testeable en la PC. Solo maneja el cursor entre los items del menu;
// que hace cada item (encender RF, cambiar potencia, cambiar banda)
// vive en OutputSettings/BandSelector y se despacha desde AppManager.

enum class MenuItem : uint8_t {
    RF_TOGGLE = 0,
    POWER     = 1,
    BAND      = 2,
    EXIT      = 3
};

constexpr uint8_t MENU_ITEM_COUNT = 4;

class MenuLogic {
public:
    MenuLogic() : m_cursor(0) {}

    // Mueve el cursor +1/-1, con wrap-around.
    void moveCursor(int32_t direction);

    MenuItem selectedItem() const { return static_cast<MenuItem>(m_cursor); }
    uint8_t cursorIndex() const { return m_cursor; }

    // Disponible por si en algun momento se necesita forzar la
    // posicion inicial (ej. al encender). AppManager NO la llama al
    // entrar al menu a proposito: el menu recuerda la ultima posicion
    // entre una entrada y otra (ver app_manager.cpp).
    void reset() { m_cursor = 0; }

private:
    uint8_t m_cursor;
};
