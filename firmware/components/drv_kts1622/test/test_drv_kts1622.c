#include "unity.h"

#include "board_pins.h"
#include "drv_kts1622.h"
#include "fake_i2c_bus.h"

#define IOEXP_BIT(pin) ((uint8_t)(1U << (uint8_t)((pin).bit)))

static void test_drv_kts1622__safe_defaults_logic__sets_required_bits_low(void)
{
    fake_i2c_bus_t fake;
    fake_i2c_bus_init(&fake);
    fake_i2c_bus_set_present(&fake, I2C_ADDR_KTS1622_HIGH, true);

    fake_i2c_bus_set_reg(&fake, I2C_ADDR_KTS1622_HIGH, DRV_KTS1622_REG_OUTPUT_PORT0, 0xFFU);
    fake_i2c_bus_set_reg(&fake, I2C_ADDR_KTS1622_HIGH, DRV_KTS1622_REG_OUTPUT_PORT1, 0xFFU);
    fake_i2c_bus_set_reg(&fake, I2C_ADDR_KTS1622_HIGH, DRV_KTS1622_REG_CONFIG_PORT0, 0xFFU);
    fake_i2c_bus_set_reg(&fake, I2C_ADDR_KTS1622_HIGH, DRV_KTS1622_REG_CONFIG_PORT1, 0xFFU);

    drv_kts1622_t dev;
    TEST_ASSERT_EQUAL(
        ESP_OK,
        drv_kts1622_init(&dev, fake_i2c_bus_get_handle(&fake), I2C_ADDR_KTS1622_HIGH)
    );

    drv_kts1622_safe_defaults_t defaults = {
        .output_mask_port0 =
            IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL) |
            IOEXP_BIT(IOEXP21_P0_CHRG_EN) |
            IOEXP_BIT(IOEXP21_P0_LOAD_CTL) |
            IOEXP_BIT(IOEXP21_P0_BEACON_CTL) |
            IOEXP_BIT(IOEXP21_P0_IO1_CTL) |
            IOEXP_BIT(IOEXP21_P0_IO2_CTL),
        .output_values_port0 = (EXTPWR_CTL_BOOT_LEVEL != 0U) ? IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL) : 0U,
        .output_mask_port1 = IOEXP_BIT(IOEXP21_P1_12V_EN),
        .output_values_port1 = 0U,
        .direction_mask_port0 =
            IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL) |
            IOEXP_BIT(IOEXP21_P0_CHRG_EN) |
            IOEXP_BIT(IOEXP21_P0_LOAD_CTL) |
            IOEXP_BIT(IOEXP21_P0_BEACON_CTL) |
            IOEXP_BIT(IOEXP21_P0_IO1_CTL) |
            IOEXP_BIT(IOEXP21_P0_IO2_CTL),
        .direction_values_port0 = 0U,
        .direction_mask_port1 = IOEXP_BIT(IOEXP21_P1_12V_EN),
        .direction_values_port1 = 0U,
    };

    uint8_t readback0 = 0U;
    uint8_t readback1 = 0U;
    TEST_ASSERT_EQUAL(
        ESP_OK,
        drv_kts1622_apply_safe_defaults(&dev, &defaults, &readback0, &readback1)
    );
    TEST_ASSERT_GREATER_THAN_UINT32(0U, fake.write_trace_len);
    TEST_ASSERT_EQUAL_UINT8(DRV_KTS1622_REG_OUTPUT_PORT0, fake.write_trace_reg[0]);

    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL), readback0);
    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P0_CHRG_EN), readback0);
    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P0_LOAD_CTL), readback0);
    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P0_BEACON_CTL), readback0);
    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P0_IO1_CTL), readback0);
    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P0_IO2_CTL), readback0);
    TEST_ASSERT_BITS_LOW(IOEXP_BIT(IOEXP21_P1_12V_EN), readback1);

    const uint8_t cfg0 = fake_i2c_bus_get_reg(&fake, I2C_ADDR_KTS1622_HIGH, DRV_KTS1622_REG_CONFIG_PORT0);
    const uint8_t cfg1 = fake_i2c_bus_get_reg(&fake, I2C_ADDR_KTS1622_HIGH, DRV_KTS1622_REG_CONFIG_PORT1);
    TEST_ASSERT_BITS_LOW(defaults.direction_mask_port0, cfg0);
    TEST_ASSERT_BITS_LOW(defaults.direction_mask_port1, cfg1);
}

void drv_kts1622_run_all_tests(void)
{
    RUN_TEST(test_drv_kts1622__safe_defaults_logic__sets_required_bits_low);
}
