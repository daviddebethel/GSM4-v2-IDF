#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "i2c_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    DRV_BQ25622_REG_00 = 0x00,
    DRV_BQ25622_REG_01 = 0x01,
    DRV_BQ25622_REG_STATUS = 0x08,
    DRV_BQ25622_REG_FAULT = 0x09,
};

typedef struct {
    const i2c_bus_t *bus;
    uint8_t addr;
} drv_bq25622_t;

typedef struct {
    bool en_hiz;
    uint8_t iindpm_code;
} drv_bq25622_reg00_fields_t;

typedef struct {
    bool charge_enable;
    uint8_t sys_min_code;
} drv_bq25622_reg01_fields_t;

typedef struct {
    drv_bq25622_reg00_fields_t reg00;
    drv_bq25622_reg01_fields_t reg01;
} drv_bq25622_safe_config_t;

typedef struct {
    uint8_t raw_status;
    uint8_t raw_fault;
    bool power_good;
    uint8_t charge_state;
    bool fault_present;
} drv_bq25622_status_t;

esp_err_t drv_bq25622_init(drv_bq25622_t *dev, const i2c_bus_t *bus, uint8_t addr);
esp_err_t drv_bq25622_probe(const i2c_bus_t *bus, uint8_t addr);

esp_err_t drv_bq25622_read_reg(drv_bq25622_t *dev, uint8_t reg, uint8_t *value);
esp_err_t drv_bq25622_write_reg(drv_bq25622_t *dev, uint8_t reg, uint8_t value);

uint8_t drv_bq25622_pack_reg00(const drv_bq25622_reg00_fields_t *fields);
void drv_bq25622_unpack_reg00(uint8_t value, drv_bq25622_reg00_fields_t *fields);

uint8_t drv_bq25622_pack_reg01(const drv_bq25622_reg01_fields_t *fields);
void drv_bq25622_unpack_reg01(uint8_t value, drv_bq25622_reg01_fields_t *fields);

esp_err_t drv_bq25622_apply_safe_boot_config(
    drv_bq25622_t *dev,
    const drv_bq25622_safe_config_t *safe_config,
    bool *readback_match
);

esp_err_t drv_bq25622_poll_status(drv_bq25622_t *dev, drv_bq25622_status_t *status);
const char *drv_bq25622_charge_state_name(uint8_t state);

#ifdef __cplusplus
}
#endif
