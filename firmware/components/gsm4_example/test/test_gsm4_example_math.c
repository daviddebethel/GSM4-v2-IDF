#include "unity.h"

#include <limits.h>

#include "gsm4_example.h"

static void test_gsm4_example__adds_regular_values__returns_sum(void)
{
    TEST_ASSERT_EQUAL_INT(7, gsm4_example_sum_clamped(3, 4));
}

static void test_gsm4_example__clamps_integer_overflow__returns_int_max(void)
{
    TEST_ASSERT_EQUAL_INT(INT_MAX, gsm4_example_sum_clamped(INT_MAX, 1));
}

void gsm4_example_math_run_all_tests(void)
{
    RUN_TEST(test_gsm4_example__adds_regular_values__returns_sum);
    RUN_TEST(test_gsm4_example__clamps_integer_overflow__returns_int_max);
}
