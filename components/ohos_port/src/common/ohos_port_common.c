#include "ohos_port.h"

#include <stdbool.h>
#include <stdio.h>

#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h"

static const char *TAG = "ohos_port";
static esp_timer_handle_t s_tick_timer;
static ohos_port_soc_caps_t s_caps;

__attribute__((weak)) void ohos_port_tick_hook(void)
{
}

static void tick_cb(void *arg)
{
    (void)arg;
    ohos_port_tick_hook();
}

static void detect_caps(void)
{
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
    s_caps.arch = OHOS_PORT_ARCH_XTENSA;
#else
    s_caps.arch = OHOS_PORT_ARCH_RISCV32;
#endif

#if defined(CONFIG_FREERTOS_UNICORE)
    s_caps.cores = 1;
#else
    s_caps.cores = 2;
#endif

    s_caps.feature_bits = OHOS_PORT_CAP_GPIO | OHOS_PORT_CAP_I2C | OHOS_PORT_CAP_SPI | OHOS_PORT_CAP_WIFI;

#if defined(CONFIG_IDF_TARGET_ESP32)
    s_caps.idf_target = "esp32";
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
    s_caps.idf_target = "esp32s2";
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    s_caps.idf_target = "esp32s3";
#elif defined(CONFIG_IDF_TARGET_ESP32C2)
    s_caps.idf_target = "esp32c2";
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
    s_caps.idf_target = "esp32c3";
#elif defined(CONFIG_IDF_TARGET_ESP32C5)
    s_caps.idf_target = "esp32c5";
#elif defined(CONFIG_IDF_TARGET_ESP32C6)
    s_caps.idf_target = "esp32c6";
#elif defined(CONFIG_IDF_TARGET_ESP32H2)
    s_caps.idf_target = "esp32h2";
#elif defined(CONFIG_IDF_TARGET_ESP32P4)
    s_caps.idf_target = "esp32p4";
#else
    s_caps.idf_target = "unknown";
#endif
}

int ohos_port_clock_init(void)
{
    return 0;
}

int ohos_port_uart_console_init(void)
{
    ESP_LOGI(TAG, "console init done");
    return 0;
}

int ohos_port_tick_start(void)
{
#if !CONFIG_OHOS_PORT_ENABLE_TICK_TIMER
    ESP_LOGW(TAG, "tick timer disabled by config");
    return 0;
#else
    const esp_timer_create_args_t args = {
        .callback = tick_cb,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "ohos_tick",
        .skip_unhandled_events = true,
    };

    const uint64_t period_us = 1000000ULL / CONFIG_OHOS_TICK_HZ;
    esp_err_t err = esp_timer_create(&args, &s_tick_timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_timer_create failed: %d", (int)err);
        return -1;
    }
    err = esp_timer_start_periodic(s_tick_timer, period_us);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_timer_start_periodic failed: %d", (int)err);
        return -1;
    }
    ESP_LOGI(TAG, "tick started: %lu us", (unsigned long)period_us);
    return 0;
#endif
}

void ohos_port_irq_enable(void)
{
}

void ohos_port_irq_disable(void)
{
}

const ohos_port_soc_caps_t *ohos_port_soc_caps_get(void)
{
    return &s_caps;
}

int ohos_port_init(void)
{
    detect_caps();

    if (ohos_port_clock_init() != 0) {
        return -1;
    }
    ohos_port_arch_init();
    ohos_port_target_init();
    if (ohos_port_uart_console_init() != 0) {
        return -1;
    }

#if CONFIG_OHOS_PORT_LOG_BOOT_BANNER
    ESP_LOGI(TAG, "OpenHarmony lightweight port boot");
    ESP_LOGI(TAG, "target=%s arch=%u cores=%u", s_caps.idf_target, s_caps.arch, s_caps.cores);
#endif

    return ohos_port_tick_start();
}
