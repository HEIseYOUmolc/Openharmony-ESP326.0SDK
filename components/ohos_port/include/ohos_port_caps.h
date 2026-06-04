#pragma once

#include <stdint.h>

#define OHOS_PORT_CAP_WIFI      (1u << 0)
#define OHOS_PORT_CAP_BT        (1u << 1)
#define OHOS_PORT_CAP_GPIO      (1u << 2)
#define OHOS_PORT_CAP_I2C       (1u << 3)
#define OHOS_PORT_CAP_SPI       (1u << 4)
#define OHOS_PORT_CAP_ETH       (1u << 5)

#define OHOS_PORT_ARCH_XTENSA   1u
#define OHOS_PORT_ARCH_RISCV32  2u

typedef struct {
    uint8_t cores;
    uint8_t arch;
    uint32_t feature_bits;
    const char *idf_target;
} ohos_port_soc_caps_t;
