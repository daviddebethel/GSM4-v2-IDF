#include "unity.h"

#include "fake_i2c_bus.h"

static void test_fake_i2c_bus__read_write_reg__uses_register_map(void)
{
    fake_i2c_bus_t fake;
    fake_i2c_bus_init(&fake);
    fake_i2c_bus_set_present(&fake, 0x21U, true);

    i2c_bus_t *bus = fake_i2c_bus_get_handle(&fake);
    TEST_ASSERT_NOT_NULL(bus);

    TEST_ASSERT_EQUAL(ESP_OK, i2c_bus_write_reg(bus, 0x21U, 0x02U, 0x5AU));

    uint8_t readback = 0U;
    TEST_ASSERT_EQUAL(ESP_OK, i2c_bus_read_reg(bus, 0x21U, 0x02U, &readback));
    TEST_ASSERT_EQUAL_HEX8(0x5AU, readback);

    TEST_ASSERT_TRUE(fake.write_count >= 1U);
    TEST_ASSERT_TRUE(fake.read_count >= 1U);
}

void fake_i2c_bus_unit_run_all_tests(void)
{
    RUN_TEST(test_fake_i2c_bus__read_write_reg__uses_register_map);
}
