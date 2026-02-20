#include "drv_kts1622.h"

#include <stdbool.h>

static esp_err_t drv_kts1622_validate_port(drv_kts1622_port_t port)
{
    if ((port != DRV_KTS1622_PORT0) && (port != DRV_KTS1622_PORT1)) {
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

static uint8_t drv_kts1622_reg_for_port(uint8_t base_reg, drv_kts1622_port_t port)
{
    return (uint8_t)(base_reg + (uint8_t)port);
}

esp_err_t drv_kts1622_init(drv_kts1622_t *dev, const i2c_bus_t *bus, uint8_t addr)
{
    if ((dev == NULL) || (bus == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    dev->bus = bus;
    dev->addr = addr;
    return ESP_OK;
}

esp_err_t drv_kts1622_probe(const i2c_bus_t *bus, uint8_t addr)
{
    return i2c_bus_probe(bus, addr);
}

esp_err_t drv_kts1622_set_direction(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t value)
{
    if (dev == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = drv_kts1622_validate_port(port);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_bus_write_reg(dev->bus, dev->addr, drv_kts1622_reg_for_port(DRV_KTS1622_REG_CONFIG_PORT0, port), value);
}

esp_err_t drv_kts1622_get_direction(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t *value)
{
    if ((dev == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = drv_kts1622_validate_port(port);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_bus_read_reg(dev->bus, dev->addr, drv_kts1622_reg_for_port(DRV_KTS1622_REG_CONFIG_PORT0, port), value);
}

esp_err_t drv_kts1622_set_output_latch(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t value)
{
    if (dev == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = drv_kts1622_validate_port(port);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_bus_write_reg(dev->bus, dev->addr, drv_kts1622_reg_for_port(DRV_KTS1622_REG_OUTPUT_PORT0, port), value);
}

esp_err_t drv_kts1622_get_output_latch(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t *value)
{
    if ((dev == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = drv_kts1622_validate_port(port);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_bus_read_reg(dev->bus, dev->addr, drv_kts1622_reg_for_port(DRV_KTS1622_REG_OUTPUT_PORT0, port), value);
}

esp_err_t drv_kts1622_read_input(drv_kts1622_t *dev, drv_kts1622_port_t port, uint8_t *value)
{
    if ((dev == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = drv_kts1622_validate_port(port);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_bus_read_reg(dev->bus, dev->addr, drv_kts1622_reg_for_port(DRV_KTS1622_REG_INPUT_PORT0, port), value);
}

static esp_err_t drv_kts1622_read_masked(
    drv_kts1622_t *dev,
    uint8_t reg,
    uint8_t mask,
    uint8_t values,
    uint8_t *merged
)
{
    uint8_t current = 0U;
    esp_err_t err = i2c_bus_read_reg(dev->bus, dev->addr, reg, &current);
    if (err != ESP_OK) {
        return err;
    }

    current = (uint8_t)((current & (uint8_t)~mask) | (values & mask));
    if (merged != NULL) {
        *merged = current;
    }
    return ESP_OK;
}

esp_err_t drv_kts1622_apply_safe_defaults(
    drv_kts1622_t *dev,
    const drv_kts1622_safe_defaults_t *defaults,
    uint8_t *readback_port0,
    uint8_t *readback_port1
)
{
    if ((dev == NULL) || (defaults == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t out0 = 0U;
    uint8_t out1 = 0U;
    uint8_t cfg0 = 0U;
    uint8_t cfg1 = 0U;

    esp_err_t err = drv_kts1622_read_masked(
        dev,
        DRV_KTS1622_REG_OUTPUT_PORT0,
        defaults->output_mask_port0,
        defaults->output_values_port0,
        &out0
    );
    if (err != ESP_OK) {
        return err;
    }

    err = drv_kts1622_read_masked(
        dev,
        DRV_KTS1622_REG_OUTPUT_PORT1,
        defaults->output_mask_port1,
        defaults->output_values_port1,
        &out1
    );
    if (err != ESP_OK) {
        return err;
    }

    err = drv_kts1622_read_masked(
        dev,
        DRV_KTS1622_REG_CONFIG_PORT0,
        defaults->direction_mask_port0,
        defaults->direction_values_port0,
        &cfg0
    );
    if (err != ESP_OK) {
        return err;
    }

    err = drv_kts1622_read_masked(
        dev,
        DRV_KTS1622_REG_CONFIG_PORT1,
        defaults->direction_mask_port1,
        defaults->direction_values_port1,
        &cfg1
    );
    if (err != ESP_OK) {
        return err;
    }

    // Port 0 is written first so EXTPWR_CTL can be forced immediately.
    err = i2c_bus_write_reg(dev->bus, dev->addr, DRV_KTS1622_REG_OUTPUT_PORT0, out0);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_bus_write_reg(dev->bus, dev->addr, DRV_KTS1622_REG_OUTPUT_PORT1, out1);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_bus_write_reg(dev->bus, dev->addr, DRV_KTS1622_REG_CONFIG_PORT0, cfg0);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_bus_write_reg(dev->bus, dev->addr, DRV_KTS1622_REG_CONFIG_PORT1, cfg1);
    if (err != ESP_OK) {
        return err;
    }

    uint8_t verify0 = 0U;
    uint8_t verify1 = 0U;
    err = i2c_bus_read_reg(dev->bus, dev->addr, DRV_KTS1622_REG_OUTPUT_PORT0, &verify0);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_bus_read_reg(dev->bus, dev->addr, DRV_KTS1622_REG_OUTPUT_PORT1, &verify1);
    if (err != ESP_OK) {
        return err;
    }

    if (((verify0 & defaults->output_mask_port0) != (out0 & defaults->output_mask_port0)) ||
        ((verify1 & defaults->output_mask_port1) != (out1 & defaults->output_mask_port1))) {
        return ESP_ERR_INVALID_STATE;
    }

    if (readback_port0 != NULL) {
        *readback_port0 = verify0;
    }
    if (readback_port1 != NULL) {
        *readback_port1 = verify1;
    }

    return ESP_OK;
}

esp_err_t drv_kts1622_read_interrupt_status(drv_kts1622_t *dev, uint8_t *port0, uint8_t *port1)
{
    if ((dev == NULL) || (port0 == NULL) || (port1 == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = i2c_bus_read_reg(dev->bus, dev->addr, DRV_KTS1622_REG_INPUT_PORT0, port0);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_bus_read_reg(dev->bus, dev->addr, DRV_KTS1622_REG_INPUT_PORT1, port1);
}

esp_err_t drv_kts1622_clear_interrupt_status(drv_kts1622_t *dev)
{
    uint8_t port0 = 0U;
    uint8_t port1 = 0U;
    return drv_kts1622_read_interrupt_status(dev, &port0, &port1);
}
