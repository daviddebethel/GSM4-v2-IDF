#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drv_kts1622.h"
#include "esp_err.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_I2C_MAX_SCAN_RESULTS 32U

typedef struct {
    uint8_t found[BSP_I2C_MAX_SCAN_RESULTS];
    size_t found_count;
    uint8_t missing[BSP_I2C_MAX_SCAN_RESULTS];
    size_t missing_count;
    uint8_t unexpected[BSP_I2C_MAX_SCAN_RESULTS];
    size_t unexpected_count;
} bsp_i2c_scan_result_t;

esp_err_t bsp_init(void);

esp_err_t bsp_gpio_safe_defaults(void);
bool bsp_gpio_is_safe_early_output_request(int pin, int level);

esp_err_t bsp_log_sanitize(const char *input, char *output, size_t output_len);

esp_err_t bsp_i2c_init(i2c_bus_t *bus);
esp_err_t bsp_i2c_scan(const i2c_bus_t *bus, uint8_t *found, size_t found_capacity, size_t *found_count);
esp_err_t bsp_i2c_scan_and_log(const i2c_bus_t *bus, bsp_i2c_scan_result_t *result);
void bsp_i2c_compare_sets(
    const uint8_t *expected,
    size_t expected_count,
    const uint8_t *found,
    size_t found_count,
    uint8_t *missing,
    size_t missing_capacity,
    size_t *missing_count,
    uint8_t *unexpected,
    size_t unexpected_capacity,
    size_t *unexpected_count
);
void bsp_i2c_format_addr_list(const uint8_t *addrs, size_t count, char *buf, size_t buf_len);
void bsp_i2c_get_expected_devices(const uint8_t **expected, size_t *expected_count);

void bsp_fill_kts_high_safe_defaults(drv_kts1622_safe_defaults_t *defaults);

esp_err_t bsp_interrupts_init(const drv_kts1622_t *kts_low, const drv_kts1622_t *kts_high);

#ifdef __cplusplus
}
#endif
