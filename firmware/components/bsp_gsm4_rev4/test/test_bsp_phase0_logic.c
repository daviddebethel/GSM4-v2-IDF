#include "unity.h"

#include <string.h>

#include "board_pins.h"
#include "bsp.h"

static void test_bsp_i2c__compare_sets__returns_deterministic_missing_and_unexpected(void)
{
    const uint8_t expected[] = {0x10U, 0x18U, 0x20U, 0x21U, 0x50U, 0x52U, 0x6BU};
    const uint8_t found[] = {0x6BU, 0x20U, 0x21U, 0x10U, 0x30U};

    uint8_t missing[8] = {0};
    uint8_t unexpected[8] = {0};
    size_t missing_count = 0U;
    size_t unexpected_count = 0U;

    bsp_i2c_compare_sets(
        expected,
        sizeof(expected),
        found,
        sizeof(found),
        missing,
        sizeof(missing),
        &missing_count,
        unexpected,
        sizeof(unexpected),
        &unexpected_count
    );

    TEST_ASSERT_EQUAL_UINT32(3U, missing_count);
    TEST_ASSERT_EQUAL_HEX8(0x18U, missing[0]);
    TEST_ASSERT_EQUAL_HEX8(0x50U, missing[1]);
    TEST_ASSERT_EQUAL_HEX8(0x52U, missing[2]);

    TEST_ASSERT_EQUAL_UINT32(1U, unexpected_count);
    TEST_ASSERT_EQUAL_HEX8(0x30U, unexpected[0]);
}

static void test_bsp_gpio__strap_policy__rejects_unsafe_early_drive(void)
{
    TEST_ASSERT_FALSE(bsp_gpio_is_safe_early_output_request(PIN_BOOT_BUTTON, 1));
    TEST_ASSERT_FALSE(bsp_gpio_is_safe_early_output_request(PIN_MDM_PWRKEY, 1));
    TEST_ASSERT_FALSE(bsp_gpio_is_safe_early_output_request(PIN_SPI_CS2, 1));
    TEST_ASSERT_FALSE(bsp_gpio_is_safe_early_output_request(PIN_SYS_EN, 1));
    TEST_ASSERT_TRUE(bsp_gpio_is_safe_early_output_request(PIN_SYS_EN, BSP_DEFAULT_SYS_EN_LEVEL));
}

static void test_bsp_log__sanitize__redacts_secret_fields_and_keeps_fingerprints(void)
{
    const char *input =
        "user=ops password=topsecret token=abc123 key_id=charger_main "
        "sha256=012345 fingerprint=aa:bb api_key=xyz";

    char sanitized[256];
    TEST_ASSERT_EQUAL(ESP_OK, bsp_log_sanitize(input, sanitized, sizeof(sanitized)));

    TEST_ASSERT_NOT_NULL(strstr(sanitized, "password=[REDACTED]"));
    TEST_ASSERT_NOT_NULL(strstr(sanitized, "token=[REDACTED]"));
    TEST_ASSERT_NOT_NULL(strstr(sanitized, "api_key=[REDACTED]"));

    TEST_ASSERT_NULL(strstr(sanitized, "topsecret"));
    TEST_ASSERT_NULL(strstr(sanitized, "abc123"));
    TEST_ASSERT_NULL(strstr(sanitized, "xyz"));

    TEST_ASSERT_NOT_NULL(strstr(sanitized, "key_id=charger_main"));
    TEST_ASSERT_NOT_NULL(strstr(sanitized, "sha256=012345"));
    TEST_ASSERT_NOT_NULL(strstr(sanitized, "fingerprint=aa:bb"));
}

void bsp_phase0_logic_run_all_tests(void)
{
    RUN_TEST(test_bsp_i2c__compare_sets__returns_deterministic_missing_and_unexpected);
    RUN_TEST(test_bsp_gpio__strap_policy__rejects_unsafe_early_drive);
    RUN_TEST(test_bsp_log__sanitize__redacts_secret_fields_and_keeps_fingerprints);
}
