#include "bsp.h"

#include <stdio.h>
#include <string.h>

#include "board_pins.h"
#include "drv_bq25622.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if !CONFIG_IDF_TARGET_LINUX
#include "driver/gpio.h"
#endif

#define IOEXP_BIT(pin) ((uint8_t)(1U << (uint8_t)((pin).bit)))

static const char *TAG_BOOT = "BOOT";
static const char *TAG_BSP = "BSP";
static const char *TAG_KTS = "KTS";
static const char *TAG_BQ = "BQ";
static const char *TAG_SEC = "SEC";

static i2c_bus_t s_i2c_bus;
static drv_kts1622_t s_kts_low;
static drv_kts1622_t s_kts_high;
static drv_bq25622_t s_bq;

static int bsp_read_gpio_level(int pin)
{
#if CONFIG_IDF_TARGET_LINUX
    (void)pin;
    return 1;
#else
    return gpio_get_level((gpio_num_t)pin);
#endif
}

void bsp_fill_kts_high_safe_defaults(drv_kts1622_safe_defaults_t *defaults)
{
    if (defaults == NULL) {
        return;
    }

    memset(defaults, 0, sizeof(*defaults));

    const uint8_t p0_mask =
        IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL) |
        IOEXP_BIT(IOEXP21_P0_CHRG_EN) |
        IOEXP_BIT(IOEXP21_P0_LOAD_CTL) |
        IOEXP_BIT(IOEXP21_P0_BEACON_CTL) |
        IOEXP_BIT(IOEXP21_P0_IO1_CTL) |
        IOEXP_BIT(IOEXP21_P0_IO2_CTL);

    const uint8_t p1_mask = IOEXP_BIT(IOEXP21_P1_12V_EN);

    defaults->output_mask_port0 = p0_mask;
    defaults->output_values_port0 = (EXTPWR_CTL_BOOT_LEVEL != 0)
        ? IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL)
        : 0U;

    defaults->output_mask_port1 = p1_mask;
    defaults->output_values_port1 = 0U;

    // 0 = output, 1 = input in KTS1622 config registers.
    defaults->direction_mask_port0 = p0_mask;
    defaults->direction_values_port0 = 0U;
    defaults->direction_mask_port1 = p1_mask;
    defaults->direction_values_port1 = 0U;
}

#if CONFIG_GSM4_PHASE0_TELEMETRY
static void bsp_phase0_telemetry_task(void *arg)
{
    (void)arg;

    for (;;) {
        drv_bq25622_status_t status = {0};
        const esp_err_t bq_err = drv_bq25622_poll_status(&s_bq, &status);

        char fault_buf[12];
        if ((bq_err == ESP_OK) && status.fault_present) {
            snprintf(fault_buf, sizeof(fault_buf), "0x%02X", status.raw_fault);
        } else {
            strcpy(fault_buf, "none");
        }

        ESP_LOGI(
            TAG_BSP,
            "telemetry gpio_int=%d global_int=%d bq_state=%s fault=%s",
            bsp_read_gpio_level(PIN_GPIO_INT),
            bsp_read_gpio_level(PIN_GLOBAL_INT),
            (bq_err == ESP_OK) ? drv_bq25622_charge_state_name(status.charge_state) : "unavail",
            (bq_err == ESP_OK) ? fault_buf : "unavail"
        );

        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}
#endif

esp_err_t bsp_init(void)
{
    ESP_LOGI(TAG_BOOT, "phase0 boot start board=gsm4_rev4 uart0=ok");
    ESP_LOGI(TAG_SEC, "phase0 policy secure_boot=0 flash_encryption=0 nvs_encryption=0");

    esp_err_t err = bsp_gpio_safe_defaults();
    if (err != ESP_OK) {
        ESP_LOGE(TAG_BSP, "safe gpio defaults failed err=0x%x", err);
        return err;
    }

    err = bsp_i2c_init(&s_i2c_bus);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_BSP, "i2c init failed err=0x%x", err);
        return err;
    }

    bsp_i2c_scan_result_t scan = {0};
    err = bsp_i2c_scan_and_log(&s_i2c_bus, &scan);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_BSP, "i2c scan failed err=0x%x", err);
        return err;
    }

    if ((scan.missing_count > 0U) || (scan.unexpected_count > 0U)) {
        ESP_LOGW(TAG_BSP, "i2c scan differs from expected inventory");
    }

    err = drv_kts1622_init(&s_kts_low, &s_i2c_bus, I2C_ADDR_KTS1622_LOW);
    if (err != ESP_OK) {
        return err;
    }
    err = drv_kts1622_probe(&s_i2c_bus, I2C_ADDR_KTS1622_LOW);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_KTS, "0x20 init failed err=0x%x", err);
        return err;
    }
    ESP_LOGI(TAG_KTS, "0x20 init ok");

    err = drv_kts1622_init(&s_kts_high, &s_i2c_bus, I2C_ADDR_KTS1622_HIGH);
    if (err != ESP_OK) {
        return err;
    }
    err = drv_kts1622_probe(&s_i2c_bus, I2C_ADDR_KTS1622_HIGH);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_KTS, "0x21 init failed err=0x%x", err);
        return err;
    }
    ESP_LOGI(TAG_KTS, "0x21 init ok");

    drv_kts1622_safe_defaults_t kts_defaults;
    bsp_fill_kts_high_safe_defaults(&kts_defaults);

    uint8_t readback0 = 0U;
    uint8_t readback1 = 0U;
    err = drv_kts1622_apply_safe_defaults(&s_kts_high, &kts_defaults, &readback0, &readback1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_KTS, "safe defaults apply failed err=0x%x", err);
        return err;
    }

    ESP_LOGI(
        TAG_KTS,
        "safe defaults applied: EXTPWR_CTL=%u CHRG_EN=0 12V_EN=0 LOAD_CTL=0 BEACON_CTL=0 IO1_CTL=0 IO2_CTL=0",
        (unsigned)((readback0 & IOEXP_BIT(IOEXP21_P0_EXTPWR_CTL)) != 0U)
    );
    ESP_LOGI(TAG_KTS, "readback defaults verified");

    err = bsp_interrupts_init(&s_kts_low, &s_kts_high);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_BSP, "interrupt init failed err=0x%x", err);
        return err;
    }

    err = drv_bq25622_init(&s_bq, &s_i2c_bus, I2C_ADDR_BQ25622);
    if (err != ESP_OK) {
        return err;
    }

    err = drv_bq25622_probe(&s_i2c_bus, I2C_ADDR_BQ25622);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_BQ, "init failed addr=0x6B err=0x%x", err);
        return err;
    }
    ESP_LOGI(TAG_BQ, "init ok addr=0x6B");

    bool readback_match = false;
    err = drv_bq25622_apply_safe_boot_config(&s_bq, NULL, &readback_match);
    if ((err != ESP_OK) || !readback_match) {
        ESP_LOGE(TAG_BQ, "safe cfg apply failed err=0x%x", err);
        return (err == ESP_OK) ? ESP_ERR_INVALID_STATE : err;
    }
    ESP_LOGI(TAG_BQ, "safe cfg applied and readback=match");

    drv_bq25622_status_t status = {0};
    err = drv_bq25622_poll_status(&s_bq, &status);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_BQ, "status poll failed err=0x%x", err);
        return err;
    }

    char fault_buf[12];
    if (status.fault_present) {
        snprintf(fault_buf, sizeof(fault_buf), "0x%02X", status.raw_fault);
    } else {
        strcpy(fault_buf, "none");
    }

    ESP_LOGI(
        TAG_BQ,
        "status summary: pg=%u charge_state=%s fault=%s",
        status.power_good ? 1U : 0U,
        drv_bq25622_charge_state_name(status.charge_state),
        fault_buf
    );

#if CONFIG_GSM4_PHASE0_TELEMETRY
    xTaskCreatePinnedToCore(
        bsp_phase0_telemetry_task,
        "phase0_telemetry",
        3072,
        NULL,
        4,
        NULL,
        tskNO_AFFINITY
    );
#endif

    ESP_LOGI(TAG_BOOT, "phase0 bringup complete");
    return ESP_OK;
}
