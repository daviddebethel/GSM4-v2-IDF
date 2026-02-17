#include "unity.h"

#include "gsm4_example.h"
#include "Mockesp_timer.h"

static void test_gsm4_example__converts_microseconds_to_milliseconds__returns_ms(void)
{
    esp_timer_get_time_ExpectAndReturn(1234567);
    TEST_ASSERT_EQUAL_INT32(1234, gsm4_example_uptime_ms());
}

static void test_gsm4_example__clamps_negative_uptime_to_zero__returns_zero(void)
{
    esp_timer_get_time_ExpectAndReturn(-1);
    TEST_ASSERT_EQUAL_INT32(0, gsm4_example_uptime_ms());
}

void gsm4_example_clock_run_all_tests(void)
{
    RUN_TEST(test_gsm4_example__converts_microseconds_to_milliseconds__returns_ms);
    RUN_TEST(test_gsm4_example__clamps_negative_uptime_to_zero__returns_zero);
}
