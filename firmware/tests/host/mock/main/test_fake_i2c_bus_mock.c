#include "unity.h"

#include "fake_i2c_bus.h"

static void test_fake_i2c_bus__inject_error_once__fails_then_recovers(void)
{
    fake_i2c_bus_t fake;
    fake_i2c_bus_init(&fake);
    fake_i2c_bus_set_present(&fake, 0x6BU, true);

    i2c_bus_t *bus = fake_i2c_bus_get_handle(&fake);
    TEST_ASSERT_NOT_NULL(bus);

    fake_i2c_bus_inject_error(&fake, 0x6BU, 0x00U, ESP_ERR_TIMEOUT, true);

    uint8_t reg_value = 0U;
    TEST_ASSERT_EQUAL(ESP_ERR_TIMEOUT, i2c_bus_read_reg(bus, 0x6BU, 0x00U, &reg_value));

    fake_i2c_bus_set_reg(&fake, 0x6BU, 0x00U, 0x44U);
    TEST_ASSERT_EQUAL(ESP_OK, i2c_bus_read_reg(bus, 0x6BU, 0x00U, &reg_value));
    TEST_ASSERT_EQUAL_HEX8(0x44U, reg_value);
}

void fake_i2c_bus_mock_run_all_tests(void)
{
    RUN_TEST(test_fake_i2c_bus__inject_error_once__fails_then_recovers);
}
