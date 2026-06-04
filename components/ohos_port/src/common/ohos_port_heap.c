#include "ohos_port.h"

#include <stddef.h>
#include <stdint.h>

#include "sdkconfig.h"

static uint8_t s_heap_fallback[CONFIG_OHOS_PORT_HEAP_FALLBACK_SIZE];

int ohos_port_heap_region_get(void **start, size_t *size)
{
    if (start == NULL || size == NULL) {
        return -1;
    }

    *start = (void *)s_heap_fallback;
    *size = sizeof(s_heap_fallback);
    return 0;
}
