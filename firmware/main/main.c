#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "BOOT";

#if CONFIG_GSM4_TEST_HARNESS_MODE
#define GSM4_TEST_HARNESS_MODE_VALUE 1
#else
#define GSM4_TEST_HARNESS_MODE_VALUE 0
#endif

#if CONFIG_GSM4_RELEASE_SAFE_DEFAULTS
#define GSM4_RELEASE_SAFE_DEFAULTS_VALUE 1
#else
#define GSM4_RELEASE_SAFE_DEFAULTS_VALUE 0
#endif

void app_main(void)
{
#if CONFIG_GSM4_SERIAL_DEBUG_ENABLED
    ESP_LOGI(
        TAG,
        "gsm4-v2-idf bringup profile=%s test_harness=%d release_safe=%d",
        CONFIG_GSM4_BUILD_PROFILE_NAME,
        GSM4_TEST_HARNESS_MODE_VALUE,
        GSM4_RELEASE_SAFE_DEFAULTS_VALUE
    );
#endif

    while (1) {
#if CONFIG_GSM4_SERIAL_DEBUG_ENABLED
        ESP_LOGI(TAG, "gsm4-v2-idf heartbeat");
#endif
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
