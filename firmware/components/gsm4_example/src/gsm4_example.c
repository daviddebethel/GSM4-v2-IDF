#include "gsm4_example.h"

#include <limits.h>

#include "esp_timer.h"

int gsm4_example_sum_clamped(int a, int b)
{
    long long total = (long long) a + (long long) b;

    if (total > INT_MAX) {
        return INT_MAX;
    }

    if (total < INT_MIN) {
        return INT_MIN;
    }

    return (int) total;
}

int32_t gsm4_example_uptime_ms(void)
{
    int64_t uptime_us = esp_timer_get_time();

    if (uptime_us <= 0) {
        return 0;
    }

    if (uptime_us > ((int64_t) INT32_MAX * 1000LL)) {
        return INT32_MAX;
    }

    return (int32_t) (uptime_us / 1000LL);
}
