#include "i2c_bus.h"

#include <string.h>

#include "sdkconfig.h"

#if CONFIG_IDF_TARGET_LINUX

esp_err_t i2c_bus_idf_init(i2c_bus_t *bus, const i2c_bus_idf_config_t *cfg)
{
    (void)bus;
    (void)cfg;
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t i2c_bus_idf_deinit(i2c_bus_t *bus)
{
    (void)bus;
    return ESP_ERR_NOT_SUPPORTED;
}

#else

#include "driver/gpio.h"
#include "driver/i2c.h"

typedef struct {
    bool initialized;
    i2c_port_t port;
    TickType_t timeout_ticks;
} i2c_bus_idf_ctx_t;

static i2c_bus_idf_ctx_t s_idf_ctx[I2C_NUM_MAX];

static esp_err_t i2c_bus_idf_probe_cb(void *ctx_raw, uint8_t addr)
{
    if (ctx_raw == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_bus_idf_ctx_t *ctx = (i2c_bus_idf_ctx_t *)ctx_raw;
    if (!ctx->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        return ESP_ERR_NO_MEM;
    }

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (uint8_t)((addr << 1U) | I2C_MASTER_WRITE), true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(ctx->port, cmd, ctx->timeout_ticks);
    i2c_cmd_link_delete(cmd);
    return err;
}

static esp_err_t i2c_bus_idf_read_reg_cb(void *ctx_raw, uint8_t addr, uint8_t reg, uint8_t *value)
{
    if ((ctx_raw == NULL) || (value == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_bus_idf_ctx_t *ctx = (i2c_bus_idf_ctx_t *)ctx_raw;
    if (!ctx->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return i2c_master_write_read_device(ctx->port, addr, &reg, 1U, value, 1U, ctx->timeout_ticks);
}

static esp_err_t i2c_bus_idf_write_reg_cb(void *ctx_raw, uint8_t addr, uint8_t reg, uint8_t value)
{
    if (ctx_raw == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_bus_idf_ctx_t *ctx = (i2c_bus_idf_ctx_t *)ctx_raw;
    if (!ctx->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    const uint8_t payload[2] = {reg, value};
    return i2c_master_write_to_device(ctx->port, addr, payload, sizeof(payload), ctx->timeout_ticks);
}

static esp_err_t i2c_bus_idf_read_multi_cb(
    void *ctx_raw,
    uint8_t addr,
    uint8_t reg,
    uint8_t *buf,
    size_t len
)
{
    if ((ctx_raw == NULL) || (buf == NULL) || (len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_bus_idf_ctx_t *ctx = (i2c_bus_idf_ctx_t *)ctx_raw;
    if (!ctx->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return i2c_master_write_read_device(ctx->port, addr, &reg, 1U, buf, len, ctx->timeout_ticks);
}

#define I2C_BUS_MAX_MULTI_WRITE_LEN 64U

static esp_err_t i2c_bus_idf_write_multi_cb(
    void *ctx_raw,
    uint8_t addr,
    uint8_t reg,
    const uint8_t *buf,
    size_t len
)
{
    if ((ctx_raw == NULL) || (buf == NULL) || (len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (len > I2C_BUS_MAX_MULTI_WRITE_LEN) {
        return ESP_ERR_INVALID_SIZE;
    }

    i2c_bus_idf_ctx_t *ctx = (i2c_bus_idf_ctx_t *)ctx_raw;
    if (!ctx->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    uint8_t payload[I2C_BUS_MAX_MULTI_WRITE_LEN + 1U];
    payload[0] = reg;
    memcpy(&payload[1], buf, len);

    return i2c_master_write_to_device(ctx->port, addr, payload, len + 1U, ctx->timeout_ticks);
}

esp_err_t i2c_bus_idf_init(i2c_bus_t *bus, const i2c_bus_idf_config_t *cfg)
{
    if ((bus == NULL) || (cfg == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    if ((cfg->port < 0) || (cfg->port >= I2C_NUM_MAX)) {
        return ESP_ERR_INVALID_ARG;
    }

    const i2c_port_t port = (i2c_port_t)cfg->port;
    i2c_bus_idf_ctx_t *ctx = &s_idf_ctx[port];

    if (ctx->initialized) {
        i2c_driver_delete(ctx->port);
        memset(ctx, 0, sizeof(*ctx));
    }

    i2c_config_t i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = (gpio_num_t)cfg->sda_io_num,
        .scl_io_num = (gpio_num_t)cfg->scl_io_num,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = (cfg->clk_speed_hz == 0U) ? 100000U : cfg->clk_speed_hz,
        .clk_flags = 0,
    };

    esp_err_t err = i2c_param_config(port, &i2c_cfg);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_driver_install(port, i2c_cfg.mode, 0, 0, 0);
    if (err != ESP_OK) {
        return err;
    }

    ctx->initialized = true;
    ctx->port = port;
    ctx->timeout_ticks = pdMS_TO_TICKS((cfg->timeout_ms == 0U) ? 50U : cfg->timeout_ms);

    bus->ctx = ctx;
    bus->probe = i2c_bus_idf_probe_cb;
    bus->read_reg = i2c_bus_idf_read_reg_cb;
    bus->write_reg = i2c_bus_idf_write_reg_cb;
    bus->read_multi = i2c_bus_idf_read_multi_cb;
    bus->write_multi = i2c_bus_idf_write_multi_cb;

    return ESP_OK;
}

esp_err_t i2c_bus_idf_deinit(i2c_bus_t *bus)
{
    if ((bus == NULL) || (bus->ctx == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_bus_idf_ctx_t *ctx = (i2c_bus_idf_ctx_t *)bus->ctx;
    if (!ctx->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err = i2c_driver_delete(ctx->port);
    if (err != ESP_OK) {
        return err;
    }

    memset(ctx, 0, sizeof(*ctx));
    memset(bus, 0, sizeof(*bus));
    return ESP_OK;
}

#endif
