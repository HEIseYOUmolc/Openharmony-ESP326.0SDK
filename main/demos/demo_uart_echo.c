#include "demo_api.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "demo_uart";

void demo_uart_echo_run(void)
{
    const uart_port_t uart_num = (uart_port_t)CONFIG_OHOS_DEMO_UART_PORT;
    const int buf_size = 256;

    const uart_config_t cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(uart_num, 1024, 1024, 0, NULL, 0);
    uart_param_config(uart_num, &cfg);
    uart_set_pin(uart_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    ESP_LOGI(TAG, "uart echo started on port=%d", (int)uart_num);

    uint8_t buf[256];
    while (1) {
        int len = uart_read_bytes(uart_num, buf, buf_size, pdMS_TO_TICKS(1000));
        if (len > 0) {
            uart_write_bytes(uart_num, (const char *)buf, len);
            uart_write_bytes(uart_num, "\r\n", 2);
            ESP_LOGI(TAG, "echoed len=%d", len);
        }
    }
}
