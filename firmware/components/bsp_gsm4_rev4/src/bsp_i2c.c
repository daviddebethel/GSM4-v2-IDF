#include "bsp.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "board_pins.h"
#include "esp_log.h"

static const char *TAG = "I2C";

static const uint8_t s_expected_i2c_devices[] = {
    I2C_ADDR_ES8388,
    I2C_ADDR_LIS3DH,
    I2C_ADDR_KTS1622_LOW,
    I2C_ADDR_KTS1622_HIGH,
    I2C_ADDR_DS4420_0,
    I2C_ADDR_DS4420_1,
    I2C_ADDR_BQ25622,
};

#define BSP_I2C_PORT 0
#define BSP_I2C_HZ   100000U

void bsp_i2c_get_expected_devices(const uint8_t **expected, size_t *expected_count)
{
    if (expected != NULL) {
        *expected = s_expected_i2c_devices;
    }
    if (expected_count != NULL) {
        *expected_count = sizeof(s_expected_i2c_devices);
    }
}

esp_err_t bsp_i2c_init(i2c_bus_t *bus)
{
    if (bus == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const i2c_bus_idf_config_t cfg = {
        .port = BSP_I2C_PORT,
        .scl_io_num = PIN_I2C_SCL,
        .sda_io_num = PIN_I2C_SDA,
        .clk_speed_hz = BSP_I2C_HZ,
        .timeout_ms = 50U,
    };

    esp_err_t err = i2c_bus_idf_init(bus, &cfg);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "init ok port=%d scl=%d sda=%d hz=%u", BSP_I2C_PORT, PIN_I2C_SCL, PIN_I2C_SDA, BSP_I2C_HZ);
    }

    return err;
}

esp_err_t bsp_i2c_scan(const i2c_bus_t *bus, uint8_t *found, size_t found_capacity, size_t *found_count)
{
    if ((bus == NULL) || (found == NULL) || (found_count == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t count = 0U;
    for (uint8_t addr = 0x03U; addr <= 0x77U; ++addr) {
        if (i2c_bus_probe(bus, addr) != ESP_OK) {
            continue;
        }

        if (count >= found_capacity) {
            return ESP_ERR_NO_MEM;
        }

        found[count++] = addr;
    }

    *found_count = count;
    return ESP_OK;
}

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
)
{
    bool expected_map[128] = {0};
    bool found_map[128] = {0};

    for (size_t i = 0; i < expected_count; ++i) {
        expected_map[expected[i]] = true;
    }

    for (size_t i = 0; i < found_count; ++i) {
        found_map[found[i]] = true;
    }

    size_t miss = 0U;
    size_t extra = 0U;
    for (uint8_t addr = 0U; addr < 128U; ++addr) {
        if (expected_map[addr] && !found_map[addr] && (miss < missing_capacity)) {
            missing[miss++] = addr;
        }

        if (!expected_map[addr] && found_map[addr] && (extra < unexpected_capacity)) {
            unexpected[extra++] = addr;
        }
    }

    if (missing_count != NULL) {
        *missing_count = miss;
    }
    if (unexpected_count != NULL) {
        *unexpected_count = extra;
    }
}

void bsp_i2c_format_addr_list(const uint8_t *addrs, size_t count, char *buf, size_t buf_len)
{
    if ((buf == NULL) || (buf_len == 0U)) {
        return;
    }

    size_t used = 0U;
    buf[0] = '\0';

    if (used + 1U < buf_len) {
        buf[used++] = '[';
        buf[used] = '\0';
    }

    for (size_t i = 0; i < count; ++i) {
        int wrote = snprintf(&buf[used], buf_len - used, "%s0x%02X", (i == 0U) ? "" : ",", addrs[i]);
        if ((wrote <= 0) || ((size_t)wrote >= (buf_len - used))) {
            break;
        }
        used += (size_t)wrote;
    }

    if (used + 2U <= buf_len) {
        buf[used++] = ']';
        buf[used] = '\0';
    }
}

esp_err_t bsp_i2c_scan_and_log(const i2c_bus_t *bus, bsp_i2c_scan_result_t *result)
{
    if ((bus == NULL) || (result == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    memset(result, 0, sizeof(*result));

    esp_err_t err = bsp_i2c_scan(bus, result->found, BSP_I2C_MAX_SCAN_RESULTS, &result->found_count);
    if (err != ESP_OK) {
        return err;
    }

    bsp_i2c_compare_sets(
        s_expected_i2c_devices,
        sizeof(s_expected_i2c_devices),
        result->found,
        result->found_count,
        result->missing,
        BSP_I2C_MAX_SCAN_RESULTS,
        &result->missing_count,
        result->unexpected,
        BSP_I2C_MAX_SCAN_RESULTS,
        &result->unexpected_count
    );

    char found_buf[128];
    char missing_buf[128];
    char unexpected_buf[128];
    bsp_i2c_format_addr_list(result->found, result->found_count, found_buf, sizeof(found_buf));
    bsp_i2c_format_addr_list(result->missing, result->missing_count, missing_buf, sizeof(missing_buf));
    bsp_i2c_format_addr_list(result->unexpected, result->unexpected_count, unexpected_buf, sizeof(unexpected_buf));

    ESP_LOGI(TAG, "scan found=%s missing=%s unexpected=%s", found_buf, missing_buf, unexpected_buf);
    return ESP_OK;
}
