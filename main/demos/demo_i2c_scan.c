#include "demo_api.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "demo_i2c";

void demo_i2c_scan_run(void)
{
    const i2c_port_num_t port = (i2c_port_num_t)CONFIG_OHOS_DEMO_I2C_PORT;

    i2c_master_bus_config_t bus_config = {
        .i2c_port = port,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    ESP_LOGI(TAG, "i2c scan started on port=%d", (int)port);
    while (1) {
        for (uint8_t addr = 1; addr < 127; addr++) {
            i2c_device_config_t dev_cfg = {
                .dev_addr_length = I2C_ADDR_BIT_LEN_7,
                .device_address = addr,
                .scl_speed_hz = 100000,
            };

            i2c_master_dev_handle_t dev_handle;
            if (i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle) == ESP_OK) {
                uint8_t dummy = 0;
                esp_err_t err = i2c_master_transmit(dev_handle, &dummy, 1, 20);
                i2c_master_bus_rm_device(dev_handle);
                if (err == ESP_OK) {
                    ESP_LOGI(TAG, "found i2c device: 0x%02x", addr);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
