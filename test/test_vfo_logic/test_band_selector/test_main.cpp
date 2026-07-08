#include <unity.h>
#include "band_selector.h"

void setUp() {}
void tearDown() {}

void test_starts_at_40m() {
    BandSelector bands;
    TEST_ASSERT_EQUAL(Band::M40, bands.current());
    TEST_ASSERT_EQUAL_UINT64(7100000ULL, bands.currentFreqHz());
}

void test_cycle_goes_through_all_bands_in_order() {
    BandSelector bands; // arranca en 40m
    TEST_ASSERT_EQUAL(Band::M20, bands.cycle());
    TEST_ASSERT_EQUAL_UINT64(14200000ULL, bands.currentFreqHz());

    TEST_ASSERT_EQUAL(Band::M10, bands.cycle());
    TEST_ASSERT_EQUAL_UINT64(28400000ULL, bands.currentFreqHz());

    TEST_ASSERT_EQUAL(Band::M2, bands.cycle());
    TEST_ASSERT_EQUAL_UINT64(145000000ULL, bands.currentFreqHz());
}

void test_cycle_wraps_back_to_40m() {
    BandSelector bands;
    bands.cycle(); // 20m
    bands.cycle(); // 10m
    bands.cycle(); // 2m
    bands.cycle(); // wrap -> 40m
    TEST_ASSERT_EQUAL(Band::M40, bands.current());
    TEST_ASSERT_EQUAL_UINT64(7100000ULL, bands.currentFreqHz());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_starts_at_40m);
    RUN_TEST(test_cycle_goes_through_all_bands_in_order);
    RUN_TEST(test_cycle_wraps_back_to_40m);
    return UNITY_END();
}
