#include "ohos_port.h"

#include <stdint.h>

#include "esp_timer.h"
#include "los_task.h"
#include "los_timer.h"

UINT8 m_aucSysMem0[CONFIG_OHOS_PORT_HEAP_FALLBACK_SIZE];

static ArchTickTimer s_tick_timer;

VOID ArchInit(VOID)
{
}

UINT32 ArchStartSchedule(VOID)
{
    return LOS_OK;
}

VOID ArchSysExit(VOID)
{
    while (1) {
    }
}

UINT32 LOS_IntLock(VOID)
{
    ohos_port_irq_disable();
    return 0;
}

VOID LOS_IntRestore(UINT32 intSave)
{
    (void)intSave;
    ohos_port_irq_enable();
}

VOID LOS_IntUnLock(VOID)
{
    ohos_port_irq_enable();
}

static UINT32 tick_init(HWI_PROC_FUNC tickHandler)
{
    (void)tickHandler;
    return LOS_OK;
}

static UINT64 tick_get_cycle(UINT32 *period)
{
    if (period != NULL) {
        *period = (UINT32)(1000000U / CONFIG_OHOS_TICK_HZ);
    }
    return (UINT64)esp_timer_get_time();
}

static UINT64 tick_reload(UINT64 time)
{
    return time;
}

static VOID tick_lock(VOID)
{
}

static VOID tick_unlock(VOID)
{
}

ArchTickTimer *ArchSysTickTimerGet(VOID)
{
    s_tick_timer.freq = 1000000U;
    s_tick_timer.irqNum = 0;
    s_tick_timer.periodMax = UINT64_MAX;
    s_tick_timer.init = tick_init;
    s_tick_timer.getCycle = tick_get_cycle;
    s_tick_timer.reload = tick_reload;
    s_tick_timer.lock = tick_lock;
    s_tick_timer.unlock = tick_unlock;
    return &s_tick_timer;
}

VOID ohos_port_tick_hook(void)
{
    OsTickHandler();
}
