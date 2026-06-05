#include "yango_project_api.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "demo_spi";

void demo_spi_loop_run(void)
{
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = 23,
        .miso_io_num = 19,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 64,
    };
    ESP_ERROR_CHECK(spi_bus_initialize((spi_host_device_t)CONFIG_OHOS_DEMO_SPI_HOST, &bus_cfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 1000000,
        .mode = 0,
        .spics_io_num = 5,
        .queue_size = 1,
    };

    spi_device_handle_t handle;
    ESP_ERROR_CHECK(spi_bus_add_device((spi_host_device_t)CONFIG_OHOS_DEMO_SPI_HOST, &dev_cfg, &handle));

    const uint8_t tx_data[] = "ohos-spi-loop";
    uint8_t rx_data[sizeof(tx_data)] = {0};

    ESP_LOGI(TAG, "spi loop started");
    while (1) {
        spi_transaction_t t = {
            .length = sizeof(tx_data) * 8,
            .tx_buffer = tx_data,
            .rx_buffer = rx_data,
        };

        esp_err_t err = spi_device_transmit(handle, &t);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "spi transfer done; rx[0]=0x%02x", rx_data[0]);
        } else {
            ESP_LOGE(TAG, "spi transfer failed: %d", (int)err);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
