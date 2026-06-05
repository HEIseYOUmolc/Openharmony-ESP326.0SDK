#include "yango_project_api.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "demo_gpio";

void demo_gpio_blink_run(void)
{
    gpio_num_t pin = (gpio_num_t)CONFIG_OHOS_DEMO_GPIO_PIN;

    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);

    ESP_LOGI(TAG, "blink started on pin=%d", (int)pin);
    while (1) {
        gpio_set_level(pin, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(pin, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
