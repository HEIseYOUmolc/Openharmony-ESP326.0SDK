#pragma once

#include <stddef.h>
#include <stdint.h>
#include "ohos_port_caps.h"

#ifdef __cplusplus
extern "C" {
#endif

int ohos_port_init(void);
int ohos_port_clock_init(void);
int ohos_port_tick_start(void);
void ohos_port_irq_enable(void);
void ohos_port_irq_disable(void);
int ohos_port_uart_console_init(void);
int ohos_port_heap_region_get(void **start, size_t *size);
const ohos_port_soc_caps_t *ohos_port_soc_caps_get(void);

void ohos_port_arch_init(void);
void ohos_port_target_init(void);

#ifdef __cplusplus
}
#endif
