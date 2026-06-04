#include "demo_api.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_http_client.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "demo_http";

void demo_tcp_http_run(void)
{
    esp_http_client_config_t cfg = {
        .url = CONFIG_OHOS_HTTP_URL,
        .timeout_ms = 5000,
    };

    ESP_LOGI(TAG, "http client started: %s", CONFIG_OHOS_HTTP_URL);

    while (1) {
        esp_http_client_handle_t client = esp_http_client_init(&cfg);
        if (client == NULL) {
            ESP_LOGE(TAG, "esp_http_client_init failed");
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }

        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            int status = esp_http_client_get_status_code(client);
            int len = esp_http_client_get_content_length(client);
            ESP_LOGI(TAG, "HTTP status=%d len=%d", status, len);
        } else {
            ESP_LOGE(TAG, "HTTP request failed: %d", (int)err);
        }

        esp_http_client_cleanup(client);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
