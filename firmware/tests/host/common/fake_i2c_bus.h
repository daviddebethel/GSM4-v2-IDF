#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool present[128];
    uint8_t registers[128][256];

    bool inject_error_enabled;
    bool inject_error_once;
    uint8_t inject_error_addr;
    uint8_t inject_error_reg;
    esp_err_t inject_error_code;

    uint32_t read_count;
    uint32_t write_count;
    uint8_t write_trace_addr[64];
    uint8_t write_trace_reg[64];
    uint8_t write_trace_value[64];
    size_t write_trace_len;

    i2c_bus_t bus;
} fake_i2c_bus_t;

void fake_i2c_bus_init(fake_i2c_bus_t *fake);
i2c_bus_t *fake_i2c_bus_get_handle(fake_i2c_bus_t *fake);

void fake_i2c_bus_set_present(fake_i2c_bus_t *fake, uint8_t addr, bool present);
void fake_i2c_bus_set_reg(fake_i2c_bus_t *fake, uint8_t addr, uint8_t reg, uint8_t value);
uint8_t fake_i2c_bus_get_reg(const fake_i2c_bus_t *fake, uint8_t addr, uint8_t reg);

void fake_i2c_bus_inject_error(
    fake_i2c_bus_t *fake,
    uint8_t addr,
    uint8_t reg,
    esp_err_t error_code,
    bool once
);

#ifdef __cplusplus
}
#endif
