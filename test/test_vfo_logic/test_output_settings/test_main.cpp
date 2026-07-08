#include <unity.h>
#include "vfo_output_settings.h"

void setUp() {}
void tearDown() {}

void test_default_state_is_rf_on_max_power() {
    OutputSettings settings;
    TEST_ASSERT_TRUE(settings.rfEnabled());
    TEST_ASSERT_EQUAL(DriveStrengthMa::MA8, settings.drive());
}

void test_toggle_rf_flips_state() {
    OutputSettings settings;
    bool now = settings.toggleRf();
    TEST_ASSERT_FALSE(now);
    TEST_ASSERT_FALSE(settings.rfEnabled());

    now = settings.toggleRf();
    TEST_ASSERT_TRUE(now);
    TEST_ASSERT_TRUE(settings.rfEnabled());
}

void test_cycle_drive_goes_through_all_values_and_wraps() {
    OutputSettings settings; // arranca en MA8
    TEST_ASSERT_EQUAL(DriveStrengthMa::MA2, settings.cycleDrive());
    TEST_ASSERT_EQUAL(DriveStrengthMa::MA4, settings.cycleDrive());
    TEST_ASSERT_EQUAL(DriveStrengthMa::MA6, settings.cycleDrive());
    TEST_ASSERT_EQUAL(DriveStrengthMa::MA8, settings.cycleDrive());
    TEST_ASSERT_EQUAL(DriveStrengthMa::MA2, settings.cycleDrive()); // wrap
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_default_state_is_rf_on_max_power);
    RUN_TEST(test_toggle_rf_flips_state);
    RUN_TEST(test_cycle_drive_goes_through_all_values_and_wraps);
    return UNITY_END();
}
