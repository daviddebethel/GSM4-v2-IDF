#include "bsp.h"

#include <inttypes.h>
#include <string.h>

#include "board_pins.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#if !CONFIG_IDF_TARGET_LINUX
#include "driver/gpio.h"
#endif

static const char *TAG = "INT";

#define BSP_INT_QUEUE_LEN 8U
#define BSP_INT_DEBOUNCE_MS 40U

typedef struct {
    drv_kts1622_t kts_low;
    drv_kts1622_t kts_high;
    bool have_expanders;
    QueueHandle_t queue;
    TaskHandle_t task;
    volatile TickType_t last_isr_tick;
    volatile uint32_t suppressed_isr;
    uint32_t event_count;
} bsp_int_ctx_t;

static bsp_int_ctx_t s_int_ctx;

#if !CONFIG_IDF_TARGET_LINUX
static void IRAM_ATTR bsp_gpio_int_isr(void *arg)
{
    (void)arg;

    const TickType_t now = xTaskGetTickCountFromISR();
    if ((now - s_int_ctx.last_isr_tick) < pdMS_TO_TICKS(BSP_INT_DEBOUNCE_MS)) {
        s_int_ctx.suppressed_isr++;
        return;
    }

    s_int_ctx.last_isr_tick = now;
    const uint32_t token = 1U;
    BaseType_t hp_task_woken = pdFALSE;
    xQueueSendFromISR(s_int_ctx.queue, &token, &hp_task_woken);
    if (hp_task_woken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}
#endif

static void bsp_interrupt_log_event(const char *source)
{
    const uint32_t dropped = s_int_ctx.suppressed_isr;
    s_int_ctx.suppressed_isr = 0U;
    s_int_ctx.event_count++;

    ESP_LOGI(
        TAG,
        "gpio_int event count=%" PRIu32 " source=%s storm=%u",
        s_int_ctx.event_count,
        source,
        (dropped > 0U) ? 1U : 0U
    );
}

static void bsp_interrupt_task(void *arg)
{
    (void)arg;

    uint32_t token = 0U;
    while (xQueueReceive(s_int_ctx.queue, &token, portMAX_DELAY) == pdTRUE) {
        const char *source = "expander";
        if (token == 0U) {
            source = "selftest";
        }

        if (s_int_ctx.have_expanders) {
            uint8_t p0 = 0U;
            uint8_t p1 = 0U;
            (void)drv_kts1622_read_interrupt_status(&s_int_ctx.kts_low, &p0, &p1);
            (void)drv_kts1622_read_interrupt_status(&s_int_ctx.kts_high, &p0, &p1);
            (void)drv_kts1622_clear_interrupt_status(&s_int_ctx.kts_low);
            (void)drv_kts1622_clear_interrupt_status(&s_int_ctx.kts_high);
        }

        bsp_interrupt_log_event(source);
    }
}

esp_err_t bsp_interrupts_init(const drv_kts1622_t *kts_low, const drv_kts1622_t *kts_high)
{
    memset(&s_int_ctx, 0, sizeof(s_int_ctx));

    if ((kts_low != NULL) && (kts_high != NULL)) {
        s_int_ctx.kts_low = *kts_low;
        s_int_ctx.kts_high = *kts_high;
        s_int_ctx.have_expanders = true;
    }

#if CONFIG_IDF_TARGET_LINUX
    ESP_LOGI(TAG, "idle levels gpio_int=1 global_int=1 active_low=1");
    bsp_interrupt_log_event("selftest");
    return ESP_OK;
#else
    gpio_config_t gpio_int_cfg = {
        .pin_bit_mask = 1ULL << (uint32_t)PIN_GPIO_INT,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_INT_HAS_PULLUP ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    esp_err_t err = gpio_config(&gpio_int_cfg);
    if (err != ESP_OK) {
        return err;
    }

    gpio_config_t global_int_cfg = {
        .pin_bit_mask = 1ULL << (uint32_t)PIN_GLOBAL_INT,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    err = gpio_config(&global_int_cfg);
    if (err != ESP_OK) {
        return err;
    }

    s_int_ctx.queue = xQueueCreate(BSP_INT_QUEUE_LEN, sizeof(uint32_t));
    if (s_int_ctx.queue == NULL) {
        return ESP_ERR_NO_MEM;
    }

    BaseType_t task_ok = xTaskCreatePinnedToCore(
        bsp_interrupt_task,
        "bsp_gpio_int",
        3072,
        NULL,
        6,
        &s_int_ctx.task,
        tskNO_AFFINITY
    );
    if (task_ok != pdPASS) {
        vQueueDelete(s_int_ctx.queue);
        s_int_ctx.queue = NULL;
        return ESP_ERR_NO_MEM;
    }

    err = gpio_install_isr_service(0);
    if ((err != ESP_OK) && (err != ESP_ERR_INVALID_STATE)) {
        return err;
    }

    err = gpio_isr_handler_add((gpio_num_t)PIN_GPIO_INT, bsp_gpio_int_isr, NULL);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(
        TAG,
        "idle levels gpio_int=%d global_int=%d active_low=1",
        gpio_get_level((gpio_num_t)PIN_GPIO_INT),
        gpio_get_level((gpio_num_t)PIN_GLOBAL_INT)
    );

    const uint32_t selftest_token = 0U;
    xQueueSend(s_int_ctx.queue, &selftest_token, pdMS_TO_TICKS(10U));
    return ESP_OK;
#endif
}
