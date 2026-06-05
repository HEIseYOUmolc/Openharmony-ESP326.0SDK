#include "ohos_liteos_m.h"

#include "ohos_port.h"

#include "esp_log.h"
#include "los_task.h"

static const char *TAG = "ohos_liteos_m";
static int s_liteos_started;

int ohos_liteos_start(void)
{
    if (s_liteos_started) {
        ESP_LOGW(TAG, "LiteOS-M already initialized");
        return 0;
    }

    int ret = ohos_port_init();
    if (ret != 0) {
        ESP_LOGE(TAG, "ohos_port_init failed: %d", ret);
        return ret;
    }

    UINT32 los_ret = LOS_KernelInit();
    if (los_ret != LOS_OK) {
        ESP_LOGE(TAG, "LOS_KernelInit failed: 0x%08x", (unsigned int)los_ret);
        return -1;
    }

    s_liteos_started = 1;
    ESP_LOGI(TAG, "LiteOS-M kernel initialized");
    return 0;
}
