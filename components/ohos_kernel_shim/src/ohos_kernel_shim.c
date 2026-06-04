#include "ohos_kernel_shim.h"

#include "ohos_port.h"

#include "esp_log.h"

static const char *TAG = "ohos_kernel_shim";

__attribute__((weak)) int liteos_m_real_start(void)
{
    ESP_LOGW(TAG, "LiteOS-M real startup not linked; running shim mode only");
    return 0;
}

int ohos_kernel_bootstrap(void)
{
    int ret = ohos_port_init();
    if (ret != 0) {
        ESP_LOGE(TAG, "ohos_port_init failed: %d", ret);
        return ret;
    }

    ret = liteos_m_real_start();
    if (ret != 0) {
        ESP_LOGE(TAG, "liteos_m_real_start failed: %d", ret);
        return ret;
    }

    ESP_LOGI(TAG, "kernel shim bootstrap complete");
    return 0;
}
