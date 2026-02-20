#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bsp.h"
#include "esp_log.h"

static const char *TAG = "BOOT";

void app_main(void)
{
    const esp_err_t err = bsp_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "phase0 bringup failed err=0x%x", err);
    }

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}
