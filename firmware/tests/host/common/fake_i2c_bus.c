#include "fake_i2c_bus.h"

#include <string.h>

static bool fake_i2c_bus_should_error(fake_i2c_bus_t *fake, uint8_t addr, uint8_t reg)
{
    if ((fake == NULL) || !fake->inject_error_enabled) {
        return false;
    }

    if ((fake->inject_error_addr != addr) || (fake->inject_error_reg != reg)) {
        return false;
    }

    if (fake->inject_error_once) {
        fake->inject_error_enabled = false;
    }

    return true;
}

static esp_err_t fake_i2c_probe(void *ctx, uint8_t addr)
{
    fake_i2c_bus_t *fake = (fake_i2c_bus_t *)ctx;
    if (fake == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    return fake->present[addr] ? ESP_OK : ESP_ERR_NOT_FOUND;
}

static esp_err_t fake_i2c_read_reg(void *ctx, uint8_t addr, uint8_t reg, uint8_t *value)
{
    fake_i2c_bus_t *fake = (fake_i2c_bus_t *)ctx;
    if ((fake == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!fake->present[addr]) {
        return ESP_ERR_NOT_FOUND;
    }

    if (fake_i2c_bus_should_error(fake, addr, reg)) {
        return fake->inject_error_code;
    }

    fake->read_count++;
    *value = fake->registers[addr][reg];
    return ESP_OK;
}

static esp_err_t fake_i2c_write_reg(void *ctx, uint8_t addr, uint8_t reg, uint8_t value)
{
    fake_i2c_bus_t *fake = (fake_i2c_bus_t *)ctx;
    if (fake == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!fake->present[addr]) {
        return ESP_ERR_NOT_FOUND;
    }

    if (fake_i2c_bus_should_error(fake, addr, reg)) {
        return fake->inject_error_code;
    }

    fake->write_count++;
    if (fake->write_trace_len < (sizeof(fake->write_trace_reg) / sizeof(fake->write_trace_reg[0]))) {
        fake->write_trace_addr[fake->write_trace_len] = addr;
        fake->write_trace_reg[fake->write_trace_len] = reg;
        fake->write_trace_value[fake->write_trace_len] = value;
        fake->write_trace_len++;
    }
    fake->registers[addr][reg] = value;
    return ESP_OK;
}

static esp_err_t fake_i2c_read_multi(void *ctx, uint8_t addr, uint8_t reg, uint8_t *buf, size_t len)
{
    fake_i2c_bus_t *fake = (fake_i2c_bus_t *)ctx;
    if ((fake == NULL) || (buf == NULL) || (len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!fake->present[addr]) {
        return ESP_ERR_NOT_FOUND;
    }

    if (fake_i2c_bus_should_error(fake, addr, reg)) {
        return fake->inject_error_code;
    }

    for (size_t i = 0; i < len; ++i) {
        buf[i] = fake->registers[addr][(uint8_t)(reg + i)];
    }

    fake->read_count += (uint32_t)len;
    return ESP_OK;
}

static esp_err_t fake_i2c_write_multi(void *ctx, uint8_t addr, uint8_t reg, const uint8_t *buf, size_t len)
{
    fake_i2c_bus_t *fake = (fake_i2c_bus_t *)ctx;
    if ((fake == NULL) || (buf == NULL) || (len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!fake->present[addr]) {
        return ESP_ERR_NOT_FOUND;
    }

    if (fake_i2c_bus_should_error(fake, addr, reg)) {
        return fake->inject_error_code;
    }

    for (size_t i = 0; i < len; ++i) {
        fake->registers[addr][(uint8_t)(reg + i)] = buf[i];
        if (fake->write_trace_len < (sizeof(fake->write_trace_reg) / sizeof(fake->write_trace_reg[0]))) {
            fake->write_trace_addr[fake->write_trace_len] = addr;
            fake->write_trace_reg[fake->write_trace_len] = (uint8_t)(reg + i);
            fake->write_trace_value[fake->write_trace_len] = buf[i];
            fake->write_trace_len++;
        }
    }

    fake->write_count += (uint32_t)len;
    return ESP_OK;
}

void fake_i2c_bus_init(fake_i2c_bus_t *fake)
{
    if (fake == NULL) {
        return;
    }

    memset(fake, 0, sizeof(*fake));
    fake->bus.ctx = fake;
    fake->bus.probe = fake_i2c_probe;
    fake->bus.read_reg = fake_i2c_read_reg;
    fake->bus.write_reg = fake_i2c_write_reg;
    fake->bus.read_multi = fake_i2c_read_multi;
    fake->bus.write_multi = fake_i2c_write_multi;
}

i2c_bus_t *fake_i2c_bus_get_handle(fake_i2c_bus_t *fake)
{
    if (fake == NULL) {
        return NULL;
    }

    return &fake->bus;
}

void fake_i2c_bus_set_present(fake_i2c_bus_t *fake, uint8_t addr, bool present)
{
    if (fake == NULL) {
        return;
    }

    fake->present[addr] = present;
}

void fake_i2c_bus_set_reg(fake_i2c_bus_t *fake, uint8_t addr, uint8_t reg, uint8_t value)
{
    if (fake == NULL) {
        return;
    }

    fake->registers[addr][reg] = value;
}

uint8_t fake_i2c_bus_get_reg(const fake_i2c_bus_t *fake, uint8_t addr, uint8_t reg)
{
    if (fake == NULL) {
        return 0U;
    }

    return fake->registers[addr][reg];
}

void fake_i2c_bus_inject_error(
    fake_i2c_bus_t *fake,
    uint8_t addr,
    uint8_t reg,
    esp_err_t error_code,
    bool once
)
{
    if (fake == NULL) {
        return;
    }

    fake->inject_error_enabled = true;
    fake->inject_error_once = once;
    fake->inject_error_addr = addr;
    fake->inject_error_reg = reg;
    fake->inject_error_code = error_code;
}
