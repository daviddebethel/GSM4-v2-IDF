#pragma once

#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DRV_KTS1622_PORT0 = 0,
    DRV_KTS1622_PORT1 = 1,
} drv_kts1622_port_t;

enum {
    DRV_KTS1622_REG_INPUT_PORT0 = 0x00,
    DRV_KTS1622_REG_INPUT_PORT1 = 0x01,
    DRV_KTS1622_REG_OUTPUT_PORT0 = 0x02,
    DRV_KTS1622_REG_OUTPUT_PORT1 = 0x03,
    DRV_KTS1622_REG_POLARITY_PORT0 = 0x04,
    DRV_KTS1622_REG_POLARITY_PORT1 = 0x05,
    DRV_KTS1622_REG_CONFIG_PORT0 = 0x06,
    DRV_KTS1622_REG_CONFIG_PORT1 = 0x07,
};

typedef struct {
    const i2c_bus_t *bus;
    uint8_t addr;
} drv_kts1622_t;

typedef struct {
    uint8_t output_mask_port0;
    uint8_t output_values_port0;
    uint8_t output_mask_port1;
    uint8_t output_values_port1;
    uint8_t direction_mask_port0;
    uint8_t direction_values_port0;
    uint8_t direction_mask_port1;
    uint8_t direction_values_port1;
} drv_kts1622_safe_defaults_t;

esp_err_t drv_kts1622_init(drv_kts1622_t *dev, const i2c_bus_t *bus, uint8_t addr);
esp_err_t drv_kts1622_probe(const i2c_bus_t *bus, uint8_t addr);

esp_err_t drv_kts1622_set_direction(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t value);
esp_err_t drv_kts1622_get_direction(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t *value);

esp_err_t drv_kts1622_set_output_latch(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t value);
esp_err_t drv_kts1622_get_output_latch(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t *value);

esp_err_t drv_kts1622_read_input(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t *value);

esp_err_t drv_kts1622_apply_safe_defaults(
    drv_kts1622_t *dev,
    const drv_kts1622_safe_defaults_t *defaults,
    uint8_t *readback_port0,
    uint8_t *readback_port1
);

esp_err_t drv_kts1622_read_interrupt_status(drv_kts1622_t *dev, uint8_t *port0, uint8_t *port1);
esp_err_t drv_kts1622_clear_interrupt_status(drv_kts1622_t *dev);

#ifdef __cplusplus
}
#endif
