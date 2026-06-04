#include "demo_runner.h"
#include "ohos_kernel_shim.h"

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

    if (ohos_kernel_bootstrap() != 0) {
        ESP_LOGE(TAG, "OpenHarmony bootstrap failed");
        return;
    }

    demo_run_selected();
}
