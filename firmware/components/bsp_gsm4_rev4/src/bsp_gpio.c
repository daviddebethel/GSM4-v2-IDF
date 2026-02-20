#include "bsp.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "board_pins.h"
#include "esp_log.h"
#include "sdkconfig.h"

#if !CONFIG_IDF_TARGET_LINUX
#include "driver/gpio.h"
#endif

static const char *TAG = "BSP";

bool bsp_gpio_is_safe_early_output_request(int pin, int level)
{
    (void)level;

    switch (pin) {
        case PIN_BOOT_BUTTON:
            return false;
        case PIN_MDM_PWRKEY:
            return false;
        case PIN_SPI_CS2:
            return false;
        case PIN_SYS_EN:
            return level == BSP_DEFAULT_SYS_EN_LEVEL;
        default:
            return true;
    }
}

#if !CONFIG_IDF_TARGET_LINUX
static esp_err_t bsp_gpio_config_output(gpio_num_t gpio, int level)
{
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << (uint32_t)gpio,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    esp_err_t err = gpio_config(&cfg);
    if (err != ESP_OK) {
        return err;
    }

    return gpio_set_level(gpio, level);
}
#endif

esp_err_t bsp_gpio_safe_defaults(void)
{
#if CONFIG_IDF_TARGET_LINUX
    ESP_LOGI(TAG, "safe gpio defaults: sys_en=%d door_lum=%d boot_pullup=%d", BSP_DEFAULT_SYS_EN_LEVEL, BSP_DEFAULT_DOOR_LUM_LEVEL, BOOT_BUTTON_USE_PULLUP);
    ESP_LOGI(TAG, "strap cautions gpio0=input_pullup gpio2=defer_drive gpio5=inactive gpio12=hold_low");
    return ESP_OK;
#else
    esp_err_t err = bsp_gpio_config_output((gpio_num_t)PIN_SYS_EN, BSP_DEFAULT_SYS_EN_LEVEL);
    if (err != ESP_OK) {
        return err;
    }

    err = bsp_gpio_config_output((gpio_num_t)PIN_DOOR_LUM, BSP_DEFAULT_DOOR_LUM_LEVEL);
    if (err != ESP_OK) {
        return err;
    }

    gpio_config_t boot_cfg = {
        .pin_bit_mask = 1ULL << (uint32_t)PIN_BOOT_BUTTON,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = BOOT_BUTTON_USE_PULLUP ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    err = gpio_config(&boot_cfg);
    if (err != ESP_OK) {
        return err;
    }

    const uint64_t strap_mask =
        (1ULL << (uint32_t)PIN_MDM_PWRKEY) |
        (1ULL << (uint32_t)PIN_SPI_CS2);

    gpio_config_t strap_cfg = {
        .pin_bit_mask = strap_mask,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    err = gpio_config(&strap_cfg);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG, "safe gpio defaults: sys_en=%d door_lum=%d boot_pullup=%d", BSP_DEFAULT_SYS_EN_LEVEL, BSP_DEFAULT_DOOR_LUM_LEVEL, BOOT_BUTTON_USE_PULLUP);
    ESP_LOGI(TAG, "strap cautions gpio0=input_pullup gpio2=defer_drive gpio5=inactive gpio12=hold_low");
    return ESP_OK;
#endif
}

static bool bsp_str_contains_nocase(const char *text, const char *needle)
{
    if ((text == NULL) || (needle == NULL) || (*needle == '\0')) {
        return false;
    }

    size_t needle_len = strlen(needle);
    for (size_t i = 0; text[i] != '\0'; ++i) {
        size_t j = 0;
        while ((j < needle_len) && (text[i + j] != '\0') &&
               (tolower((unsigned char)text[i + j]) == tolower((unsigned char)needle[j]))) {
            ++j;
        }
        if (j == needle_len) {
            return true;
        }
    }

    return false;
}

static bool bsp_key_allowed_plaintext(const char *key)
{
    if (key == NULL) {
        return false;
    }

    if (strcmp(key, "key_id") == 0) {
        return true;
    }

    if (strncmp(key, "sha256", 6) == 0) {
        return true;
    }

    return bsp_str_contains_nocase(key, "fingerprint");
}

static bool bsp_key_requires_redaction(const char *key)
{
    static const char *const k_secret_needles[] = {
        "password",
        "passwd",
        "pin",
        "token",
        "secret",
        "cert",
        "salt",
        "credential",
        "apikey",
        "api_key",
        "private",
        "key",
    };

    if ((key == NULL) || bsp_key_allowed_plaintext(key)) {
        return false;
    }

    for (size_t i = 0; i < (sizeof(k_secret_needles) / sizeof(k_secret_needles[0])); ++i) {
        if (bsp_str_contains_nocase(key, k_secret_needles[i])) {
            return true;
        }
    }

    return false;
}

static esp_err_t bsp_buffer_append(char *output, size_t output_len, size_t *cursor, const char *data, size_t data_len)
{
    if ((output == NULL) || (cursor == NULL) || (data == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }

    if ((*cursor + data_len + 1U) > output_len) {
        return ESP_ERR_INVALID_SIZE;
    }

    memcpy(&output[*cursor], data, data_len);
    *cursor += data_len;
    output[*cursor] = '\0';
    return ESP_OK;
}

esp_err_t bsp_log_sanitize(const char *input, char *output, size_t output_len)
{
    if ((input == NULL) || (output == NULL) || (output_len == 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    output[0] = '\0';
    size_t out_pos = 0U;

    const char *p = input;
    while (*p != '\0') {
        while ((*p != '\0') && isspace((unsigned char)*p)) {
            esp_err_t err = bsp_buffer_append(output, output_len, &out_pos, p, 1U);
            if (err != ESP_OK) {
                return err;
            }
            ++p;
        }

        const char *token_start = p;
        while ((*p != '\0') && !isspace((unsigned char)*p)) {
            ++p;
        }

        const size_t token_len = (size_t)(p - token_start);
        if (token_len == 0U) {
            continue;
        }

        const char *eq = memchr(token_start, '=', token_len);
        if (eq == NULL) {
            esp_err_t err = bsp_buffer_append(output, output_len, &out_pos, token_start, token_len);
            if (err != ESP_OK) {
                return err;
            }
            continue;
        }

        const size_t key_len = (size_t)(eq - token_start);
        if (key_len == 0U) {
            esp_err_t err = bsp_buffer_append(output, output_len, &out_pos, token_start, token_len);
            if (err != ESP_OK) {
                return err;
            }
            continue;
        }

        char key_buf[64];
        const size_t clipped_key_len = (key_len >= sizeof(key_buf)) ? (sizeof(key_buf) - 1U) : key_len;
        for (size_t i = 0; i < clipped_key_len; ++i) {
            key_buf[i] = (char)tolower((unsigned char)token_start[i]);
        }
        key_buf[clipped_key_len] = '\0';

        if (!bsp_key_requires_redaction(key_buf)) {
            esp_err_t err = bsp_buffer_append(output, output_len, &out_pos, token_start, token_len);
            if (err != ESP_OK) {
                return err;
            }
            continue;
        }

        esp_err_t err = bsp_buffer_append(output, output_len, &out_pos, token_start, key_len);
        if (err != ESP_OK) {
            return err;
        }
        err = bsp_buffer_append(output, output_len, &out_pos, "=[REDACTED]", strlen("=[REDACTED]"));
        if (err != ESP_OK) {
            return err;
        }
    }

    return ESP_OK;
}
