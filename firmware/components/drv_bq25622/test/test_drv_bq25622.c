#include "unity.h"

#include "drv_bq25622.h"
#include "fake_i2c_bus.h"

static void test_drv_bq25622__reg00_pack_unpack_round_trip__returns_same_fields(void)
{
    const drv_bq25622_reg00_fields_t in = {
        .en_hiz = true,
        .iindpm_code = 0x12,
    };

    drv_bq25622_reg00_fields_t out = {0};
    const uint8_t packed = drv_bq25622_pack_reg00(&in);
    drv_bq25622_unpack_reg00(packed, &out);

    TEST_ASSERT_EQUAL_UINT8(in.iindpm_code, out.iindpm_code);
    TEST_ASSERT_EQUAL(in.en_hiz, out.en_hiz);
}

static void test_drv_bq25622__reg01_pack_unpack_round_trip__returns_same_fields(void)
{
    const drv_bq25622_reg01_fields_t in = {
        .charge_enable = false,
        .sys_min_code = 0x07,
    };

    drv_bq25622_reg01_fields_t out = {0};
    const uint8_t packed = drv_bq25622_pack_reg01(&in);
    drv_bq25622_unpack_reg01(packed, &out);

    TEST_ASSERT_EQUAL_UINT8(in.sys_min_code, out.sys_min_code);
    TEST_ASSERT_EQUAL(in.charge_enable, out.charge_enable);
}

static void test_drv_bq25622__apply_safe_boot_config__writes_and_reads_back(void)
{
    fake_i2c_bus_t fake;
    fake_i2c_bus_init(&fake);
    fake_i2c_bus_set_present(&fake, 0x6BU, true);

    drv_bq25622_t dev;
    TEST_ASSERT_EQUAL(ESP_OK, drv_bq25622_init(&dev, fake_i2c_bus_get_handle(&fake), 0x6BU));

    bool readback_match = false;
    TEST_ASSERT_EQUAL(ESP_OK, drv_bq25622_apply_safe_boot_config(&dev, NULL, &readback_match));
    TEST_ASSERT_TRUE(readback_match);

    const uint8_t reg00 = fake_i2c_bus_get_reg(&fake, 0x6BU, DRV_BQ25622_REG_00);
    const uint8_t reg01 = fake_i2c_bus_get_reg(&fake, 0x6BU, DRV_BQ25622_REG_01);

    drv_bq25622_reg00_fields_t unpack0 = {0};
    drv_bq25622_reg01_fields_t unpack1 = {0};
    drv_bq25622_unpack_reg00(reg00, &unpack0);
    drv_bq25622_unpack_reg01(reg01, &unpack1);

    TEST_ASSERT_FALSE(unpack0.en_hiz);
    TEST_ASSERT_FALSE(unpack1.charge_enable);
}

void drv_bq25622_run_all_tests(void)
{
    RUN_TEST(test_drv_bq25622__reg00_pack_unpack_round_trip__returns_same_fields);
    RUN_TEST(test_drv_bq25622__reg01_pack_unpack_round_trip__returns_same_fields);
    RUN_TEST(test_drv_bq25622__apply_safe_boot_config__writes_and_reads_back);
}
