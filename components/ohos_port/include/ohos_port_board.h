#pragma once

#include <stdint.h>

typedef struct {
    const char *board_name;
    int uart_port;
    int i2c_port;
    int spi_host;
    int gpio_demo_pin;
} ohos_port_board_profile_t;

const ohos_port_board_profile_t *ohos_port_board_profile_get(void);
