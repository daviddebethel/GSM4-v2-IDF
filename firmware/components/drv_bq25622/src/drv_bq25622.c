#include "drv_bq25622.h"

#define DRV_BQ25622_REG00_EN_HIZ_MASK    0x80U
#define DRV_BQ25622_REG00_IINDPM_MASK    0x1FU

#define DRV_BQ25622_REG01_CHG_EN_MASK    0x10U
#define DRV_BQ25622_REG01_SYS_MIN_MASK   0x0EU
#define DRV_BQ25622_REG01_SYS_MIN_SHIFT  1U

#define DRV_BQ25622_STATUS_PG_MASK       0x04U
#define DRV_BQ25622_STATUS_CHG_MASK      0x18U
#define DRV_BQ25622_STATUS_CHG_SHIFT     3U

static const drv_bq25622_safe_config_t s_safe_defaults = {
    .reg00 = {
        .en_hiz = false,
        .iindpm_code = 0x08,
    },
    .reg01 = {
        .charge_enable = false,
        .sys_min_code = 0x04,
    },
};

esp_err_t drv_bq25622_init(drv_bq25622_t *dev, const i2c_bus_t *bus, uint8_t addr)
{
    if ((dev == NULL) || (bus == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    dev->bus = bus;
    dev->addr = addr;
    return ESP_OK;
}

esp_err_t drv_bq25622_probe(const i2c_bus_t *bus, uint8_t addr)
{
    return i2c_bus_probe(bus, addr);
}

esp_err_t drv_bq25622_read_reg(drv_bq25622_t *dev, uint8_t reg, uint8_t *value)
{
    if ((dev == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    return i2c_bus_read_reg(dev->bus, dev->addr, reg, value);
}

esp_err_t drv_bq25622_write_reg(drv_bq25622_t *dev, uint8_t reg, uint8_t value)
{
    if (dev == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    return i2c_bus_write_reg(dev->bus, dev->addr, reg, value);
}

uint8_t drv_bq25622_pack_reg00(const drv_bq25622_reg00_fields_t *fields)
{
    if (fields == NULL) {
        return 0U;
    }

    uint8_t value = (uint8_t)(fields->iindpm_code & DRV_BQ25622_REG00_IINDPM_MASK);
    if (fields->en_hiz) {
        value |= DRV_BQ25622_REG00_EN_HIZ_MASK;
    }

    return value;
}

void drv_bq25622_unpack_reg00(uint8_t value, drv_bq25622_reg00_fields_t *fields)
{
    if (fields == NULL) {
        return;
    }

    fields->en_hiz = (value & DRV_BQ25622_REG00_EN_HIZ_MASK) != 0U;
    fields->iindpm_code = (uint8_t)(value & DRV_BQ25622_REG00_IINDPM_MASK);
}

uint8_t drv_bq25622_pack_reg01(const drv_bq25622_reg01_fields_t *fields)
{
    if (fields == NULL) {
        return 0U;
    }

    uint8_t value = (uint8_t)((fields->sys_min_code << DRV_BQ25622_REG01_SYS_MIN_SHIFT) & DRV_BQ25622_REG01_SYS_MIN_MASK);
    if (fields->charge_enable) {
        value |= DRV_BQ25622_REG01_CHG_EN_MASK;
    }

    return value;
}

void drv_bq25622_unpack_reg01(uint8_t value, drv_bq25622_reg01_fields_t *fields)
{
    if (fields == NULL) {
        return;
    }

    fields->charge_enable = (value & DRV_BQ25622_REG01_CHG_EN_MASK) != 0U;
    fields->sys_min_code = (uint8_t)((value & DRV_BQ25622_REG01_SYS_MIN_MASK) >> DRV_BQ25622_REG01_SYS_MIN_SHIFT);
}

esp_err_t drv_bq25622_apply_safe_boot_config(
    drv_bq25622_t *dev,
    const drv_bq25622_safe_config_t *safe_config,
    bool *readback_match
)
{
    if (dev == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const drv_bq25622_safe_config_t *cfg = (safe_config == NULL) ? &s_safe_defaults : safe_config;

    const uint8_t reg00 = drv_bq25622_pack_reg00(&cfg->reg00);
    const uint8_t reg01 = drv_bq25622_pack_reg01(&cfg->reg01);

    esp_err_t err = drv_bq25622_write_reg(dev, DRV_BQ25622_REG_00, reg00);
    if (err != ESP_OK) {
        return err;
    }

    err = drv_bq25622_write_reg(dev, DRV_BQ25622_REG_01, reg01);
    if (err != ESP_OK) {
        return err;
    }

    uint8_t rb0 = 0U;
    uint8_t rb1 = 0U;
    err = drv_bq25622_read_reg(dev, DRV_BQ25622_REG_00, &rb0);
    if (err != ESP_OK) {
        return err;
    }

    err = drv_bq25622_read_reg(dev, DRV_BQ25622_REG_01, &rb1);
    if (err != ESP_OK) {
        return err;
    }

    bool match = (rb0 == reg00) && (rb1 == reg01);
    if (readback_match != NULL) {
        *readback_match = match;
    }

    return match ? ESP_OK : ESP_ERR_INVALID_STATE;
}

esp_err_t drv_bq25622_poll_status(drv_bq25622_t *dev, drv_bq25622_status_t *status)
{
    if ((dev == NULL) || (status == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t reg_status = 0U;
    uint8_t reg_fault = 0U;

    esp_err_t err = drv_bq25622_read_reg(dev, DRV_BQ25622_REG_STATUS, &reg_status);
    if (err != ESP_OK) {
        return err;
    }

    err = drv_bq25622_read_reg(dev, DRV_BQ25622_REG_FAULT, &reg_fault);
    if (err != ESP_OK) {
        return err;
    }

    status->raw_status = reg_status;
    status->raw_fault = reg_fault;
    status->power_good = (reg_status & DRV_BQ25622_STATUS_PG_MASK) != 0U;
    status->charge_state = (uint8_t)((reg_status & DRV_BQ25622_STATUS_CHG_MASK) >> DRV_BQ25622_STATUS_CHG_SHIFT);
    status->fault_present = reg_fault != 0U;
    return ESP_OK;
}

const char *drv_bq25622_charge_state_name(uint8_t state)
{
    switch (state & 0x03U) {
        case 0x00:
            return "idle";
        case 0x01:
            return "precharge";
        case 0x02:
            return "fast";
        case 0x03:
            return "done";
        default:
            return "unknown";
    }
}
