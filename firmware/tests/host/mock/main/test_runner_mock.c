#include "unity.h"

void gsm4_example_clock_run_all_tests(void);
void drv_kts1622_run_all_tests(void);
void drv_bq25622_run_all_tests(void);
void bsp_phase0_logic_run_all_tests(void);
void fake_i2c_bus_mock_run_all_tests(void);

int main(void)
{
    UNITY_BEGIN();
    gsm4_example_clock_run_all_tests();
    drv_kts1622_run_all_tests();
    drv_bq25622_run_all_tests();
    bsp_phase0_logic_run_all_tests();
    fake_i2c_bus_mock_run_all_tests();
    return UNITY_END();
}
