#include <unity.h>
#include "menu_logic.h"

void setUp() {}
void tearDown() {}

void test_starts_at_first_item() {
    MenuLogic menu;
    TEST_ASSERT_EQUAL(MenuItem::RF_TOGGLE, menu.selectedItem());
    TEST_ASSERT_EQUAL_UINT8(0, menu.cursorIndex());
}

void test_move_forward_advances_one_item() {
    MenuLogic menu;
    menu.moveCursor(+1);
    TEST_ASSERT_EQUAL(MenuItem::POWER, menu.selectedItem());
}

void test_move_forward_reaches_band_and_exit_in_order() {
    MenuLogic menu;
    menu.moveCursor(+1); // POWER
    menu.moveCursor(+1); // BAND
    TEST_ASSERT_EQUAL(MenuItem::BAND, menu.selectedItem());
    menu.moveCursor(+1); // EXIT
    TEST_ASSERT_EQUAL(MenuItem::EXIT, menu.selectedItem());
}

void test_move_forward_wraps_after_last_item() {
    MenuLogic menu;
    menu.moveCursor(+1); // POWER
    menu.moveCursor(+1); // BAND
    menu.moveCursor(+1); // EXIT
    menu.moveCursor(+1); // wrap -> RF_TOGGLE
    TEST_ASSERT_EQUAL(MenuItem::RF_TOGGLE, menu.selectedItem());
}

void test_move_backward_wraps_to_last_item() {
    MenuLogic menu;
    menu.moveCursor(-1);
    TEST_ASSERT_EQUAL(MenuItem::EXIT, menu.selectedItem());
}

void test_reset_returns_to_first_item() {
    MenuLogic menu;
    menu.moveCursor(+1);
    menu.moveCursor(+1);
    menu.reset();
    TEST_ASSERT_EQUAL(MenuItem::RF_TOGGLE, menu.selectedItem());
    TEST_ASSERT_EQUAL_UINT8(0, menu.cursorIndex());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_starts_at_first_item);
    RUN_TEST(test_move_forward_advances_one_item);
    RUN_TEST(test_move_forward_reaches_band_and_exit_in_order);
    RUN_TEST(test_move_forward_wraps_after_last_item);
    RUN_TEST(test_move_backward_wraps_to_last_item);
    RUN_TEST(test_reset_returns_to_first_item);
    return UNITY_END();
}
