#pragma once

#include <errno.h>
#include <stddef.h>
#include <string.h>

typedef int errno_t;

#ifndef EOK
#define EOK 0
#endif

static inline errno_t memcpy_s(void *dest, size_t destMax, const void *src, size_t count)
{
    if (dest == NULL || src == NULL || count > destMax) {
        return EINVAL;
    }
    (void)memcpy(dest, src, count);
    return EOK;
}

static inline errno_t memset_s(void *dest, size_t destMax, int ch, size_t count)
{
    if (dest == NULL || count > destMax) {
        return EINVAL;
    }
    (void)memset(dest, ch, count);
    return EOK;
}
