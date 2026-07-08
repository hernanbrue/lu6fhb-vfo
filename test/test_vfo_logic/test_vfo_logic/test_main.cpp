#include <unity.h>
#include "vfo_logic.h"

void setUp() {}
void tearDown() {}

void test_initial_state_uses_defaults() {
    VfoLogic vfo;
    TEST_ASSERT_EQUAL_UINT64(VFO_FREQ_DEFAULT, vfo.frequencyHz());
    TEST_ASSERT_EQUAL_UINT32(VFO_STEP_TABLE[2], vfo.stepHz());
}

void test_step_up_adds_current_step() {
    VfoLogic vfo(7100000ULL, 2); // paso = 100 Hz
    bool changed = vfo.stepFrequency(+1);
    TEST_ASSERT_TRUE(changed);
    TEST_ASSERT_EQUAL_UINT64(7100100ULL, vfo.frequencyHz());
}

void test_step_down_subtracts_current_step() {
    VfoLogic vfo(7100000ULL, 2); // paso = 100 Hz
    bool changed = vfo.stepFrequency(-1);
    TEST_ASSERT_TRUE(changed);
    TEST_ASSERT_EQUAL_UINT64(7099900ULL, vfo.frequencyHz());
}

void test_step_up_respects_upper_band_limit() {
    VfoLogic vfo(VFO_FREQ_MAX_HZ - 50, 2); // paso = 100 Hz, falta menos de un paso
    vfo.stepFrequency(+1);
    TEST_ASSERT_EQUAL_UINT64(VFO_FREQ_MAX_HZ, vfo.frequencyHz());
}

void test_step_down_respects_lower_band_limit() {
    VfoLogic vfo(VFO_FREQ_MIN_HZ + 50, 2); // paso = 100 Hz, falta menos de un paso
    vfo.stepFrequency(-1);
    TEST_ASSERT_EQUAL_UINT64(VFO_FREQ_MIN_HZ, vfo.frequencyHz());
}

void test_step_down_never_underflows_below_min() {
    // Caso limite: frecuencia igual al minimo, un paso hacia abajo no
    // debe hacer wrap-around por overflow de unsigned.
    VfoLogic vfo(VFO_FREQ_MIN_HZ, 2);
    vfo.stepFrequency(-1);
    TEST_ASSERT_EQUAL_UINT64(VFO_FREQ_MIN_HZ, vfo.frequencyHz());
}

void test_cycle_step_wraps_around() {
    VfoLogic vfo(VFO_FREQ_DEFAULT, VFO_STEP_TABLE_LEN - 1); // ultimo paso
    vfo.cycleStep();
    TEST_ASSERT_EQUAL_UINT32(0, vfo.stepIndex());
}

void test_set_frequency_clamps_to_band() {
    VfoLogic vfo;
    bool changed = vfo.setFrequency(VFO_FREQ_MAX_HZ + 1000000ULL);
    TEST_ASSERT_TRUE(changed);
    TEST_ASSERT_EQUAL_UINT64(VFO_FREQ_MAX_HZ, vfo.frequencyHz());
}

void test_set_frequency_reports_no_change_when_same() {
    VfoLogic vfo(7100000ULL, 2);
    bool changed = vfo.setFrequency(7100000ULL);
    TEST_ASSERT_FALSE(changed);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_uses_defaults);
    RUN_TEST(test_step_up_adds_current_step);
    RUN_TEST(test_step_down_subtracts_current_step);
    RUN_TEST(test_step_up_respects_upper_band_limit);
    RUN_TEST(test_step_down_respects_lower_band_limit);
    RUN_TEST(test_step_down_never_underflows_below_min);
    RUN_TEST(test_cycle_step_wraps_around);
    RUN_TEST(test_set_frequency_clamps_to_band);
    RUN_TEST(test_set_frequency_reports_no_change_when_same);
    return UNITY_END();
}
