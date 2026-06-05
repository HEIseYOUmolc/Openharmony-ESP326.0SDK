set(OHOS_LITEOS_M_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../openharmony-v6.1-release/kernel/liteos_m"
    CACHE PATH "OpenHarmony LiteOS-M source directory")

if(NOT EXISTS "${OHOS_LITEOS_M_PATH}/kernel/include/los_task.h")
    message(FATAL_ERROR
        "OpenHarmony LiteOS-M source not found at ${OHOS_LITEOS_M_PATH}. "
        "Set OHOS_LITEOS_M_PATH or place OpenHarmony at ../openharmony-v6.1-release.")
endif()

set(LITEOS_KERNEL_SRCS
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_event.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_init.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_mux.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_queue.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_sched.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_sem.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_sortlink.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_swtmr.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_task.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/los_tick.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/mm/los_membox.c"
    "${OHOS_LITEOS_M_PATH}/kernel/src/mm/los_memory.c"
    "${OHOS_LITEOS_M_PATH}/utils/los_debug.c"
    "${OHOS_LITEOS_M_PATH}/utils/los_error.c"
    "${OHOS_LITEOS_M_PATH}/utils/los_hook.c"
)

idf_component_register(
    SRCS
        "src/ohos_liteos_m_start.c"
        "src/ohos_liteos_m_port.c"
        ${LITEOS_KERNEL_SRCS}
    INCLUDE_DIRS
        "include"
        "${OHOS_LITEOS_M_PATH}/kernel/include"
        "${OHOS_LITEOS_M_PATH}/arch/include"
        "${OHOS_LITEOS_M_PATH}/utils"
        "${OHOS_LITEOS_M_PATH}/utils/internal"
    REQUIRES
        ohos_port
        esp_timer
        esp_system
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE
    __LITEOS__
    __LITEOS_M__
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-error
    -Wno-unused-function
    -Wno-unused-variable
    -Wno-missing-field-initializers
)
