#include "menu_logic.h"

void MenuLogic::moveCursor(int32_t direction) {
    if (direction > 0) {
        m_cursor = (m_cursor + 1) % MENU_ITEM_COUNT;
    } else if (direction < 0) {
        m_cursor = (m_cursor == 0) ? (MENU_ITEM_COUNT - 1) : (m_cursor - 1);
    }
}
