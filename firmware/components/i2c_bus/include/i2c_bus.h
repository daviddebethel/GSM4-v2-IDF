#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef esp_err_t (*i2c_bus_read_reg_fn)(void *ctx, uint8_t addr, uint8_t reg, uint8_t *value);
typedef esp_err_t (*i2c_bus_write_reg_fn)(void *ctx, uint8_t addr, uint8_t reg, uint8_t value);
typedef esp_err_t (*i2c_bus_read_multi_fn)(void *ctx, uint8_t addr, uint8_t reg, uint8_t *buf, size_t len);
typedef esp_err_t (*i2c_bus_write_multi_fn)(void *ctx, uint8_t addr, uint8_t reg, const uint8_t *buf, size_t len);
typedef esp_err_t (*i2c_bus_probe_fn)(void *ctx, uint8_t addr);

typedef struct {
    void *ctx;
    i2c_bus_probe_fn probe;
    i2c_bus_read_reg_fn read_reg;
    i2c_bus_write_reg_fn write_reg;
    i2c_bus_read_multi_fn read_multi;
    i2c_bus_write_multi_fn write_multi;
} i2c_bus_t;

typedef struct {
    int port;
    int scl_io_num;
    int sda_io_num;
    uint32_t clk_speed_hz;
    uint32_t timeout_ms;
} i2c_bus_idf_config_t;

static inline esp_err_t i2c_bus_probe(const i2c_bus_t *bus, uint8_t addr)
{
    if ((bus == NULL) || (bus->probe == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    return bus->probe(bus->ctx, addr);
}

static inline esp_err_t i2c_bus_read_reg(
    const i2c_bus_t *bus,
    uint8_t addr,
    uint8_t reg,
    uint8_t *value
)
{
    if ((bus == NULL) || (bus->read_reg == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    return bus->read_reg(bus->ctx, addr, reg, value);
}

static inline esp_err_t i2c_bus_write_reg(const i2c_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t value)
{
    if ((bus == NULL) || (bus->write_reg == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    return bus->write_reg(bus->ctx, addr, reg, value);
}

static inline esp_err_t i2c_bus_read_multi(
    const i2c_bus_t *bus,
    uint8_t addr,
    uint8_t reg,
    uint8_t *buf,
    size_t len
)
{
    if ((bus == NULL) || (bus->read_multi == NULL) || (buf == NULL) || (len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    return bus->read_multi(bus->ctx, addr, reg, buf, len);
}

static inline esp_err_t i2c_bus_write_multi(
    const i2c_bus_t *bus,
    uint8_t addr,
    uint8_t reg,
    const uint8_t *buf,
    size_t len
)
{
    if ((bus == NULL) || (bus->write_multi == NULL) || (buf == NULL) || (len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    return bus->write_multi(bus->ctx, addr, reg, buf, len);
}

esp_err_t i2c_bus_idf_init(i2c_bus_t *bus, const i2c_bus_idf_config_t *cfg);
esp_err_t i2c_bus_idf_deinit(i2c_bus_t *bus);

#ifdef __cplusplus
}
#endif
