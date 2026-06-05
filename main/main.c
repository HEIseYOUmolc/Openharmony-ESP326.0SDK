#include "demo_runner.h"
#include "ohos_liteos_m.h"

#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "app_main";

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "nvs_flash_init failed: %d", (int)err);
        return;
    }

    if (ohos_liteos_start() != 0) {
        ESP_LOGE(TAG, "LiteOS-M startup failed");
        return;
    }

    demo_run_selected();
}
