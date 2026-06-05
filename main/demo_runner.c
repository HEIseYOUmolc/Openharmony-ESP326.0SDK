#include "demo_runner.h"

#include "yango_project_api.h"

void demo_run_selected(void)
{
#if CONFIG_OHOS_DEMO_GPIO_BLINK
    demo_gpio_blink_run();
#elif CONFIG_OHOS_DEMO_UART_ECHO
    demo_uart_echo_run();
#elif CONFIG_OHOS_DEMO_I2C_SCAN
    demo_i2c_scan_run();
#elif CONFIG_OHOS_DEMO_SPI_LOOP
    demo_spi_loop_run();
#elif CONFIG_OHOS_DEMO_WIFI_STA
    demo_wifi_sta_run();
#elif CONFIG_OHOS_DEMO_TCP_HTTP
    demo_tcp_http_run();
#endif
}
