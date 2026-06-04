#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 2 ]; then
  echo "usage: $0 <idf_target> <default_gpio_pin>"
  exit 1
fi

TARGET="$1"
GPIO_PIN="$2"
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
TARGET_DIR="$ROOT_DIR/components/ohos_port/src/targets/$TARGET"

mkdir -p "$TARGET_DIR"

cat >"$TARGET_DIR/target_port.c" <<EOF
#include "ohos_port.h"
#include "ohos_port_board.h"

#include "esp_log.h"

static const char *TAG = "ohos_target_${TARGET}";

static const ohos_port_board_profile_t s_profile = {
    .board_name = "${TARGET}_default",
    .uart_port = 0,
    .i2c_port = 0,
    .spi_host = 2,
    .gpio_demo_pin = ${GPIO_PIN},
};

void ohos_port_target_init(void)
{
    ESP_LOGI(TAG, "target init done");
}

const ohos_port_board_profile_t *ohos_port_board_profile_get(void)
{
    return &s_profile;
}
EOF

cat >"$TARGET_DIR/Kconfig.target" <<EOF
config OHOS_TARGET_${TARGET^^}
    bool
    default y if IDF_TARGET_${TARGET^^}
EOF

cat >"$TARGET_DIR/sdkconfig.defaults" <<EOF
CONFIG_OHOS_DEMO_GPIO_PIN=${GPIO_PIN}
EOF

echo "Created target skeleton: $TARGET_DIR"
