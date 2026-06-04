#include "ohos_port.h"
#include "ohos_port_board.h"

#include "esp_log.h"

static const char *TAG = "ohos_target_esp32";

static const ohos_port_board_profile_t s_profile = {
    .board_name = "esp32_default",
    .uart_port = 0,
    .i2c_port = 0,
    .spi_host = 2,
    .gpio_demo_pin = 2,
};

void ohos_port_target_init(void)
{
    ESP_LOGI(TAG, "target init done");
}

const ohos_port_board_profile_t *ohos_port_board_profile_get(void)
{
    return &s_profile;
}
