idf_component_register(
    SRCS
        "src/common/ohos_port_common.c"
        "src/common/ohos_port_heap.c"
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp_timer
        esp_system
        freertos
)

if(IDF_TARGET STREQUAL "esp32" OR IDF_TARGET STREQUAL "esp32s2" OR IDF_TARGET STREQUAL "esp32s3")
    target_sources(${COMPONENT_LIB} PRIVATE "src/arch/xtensa/ohos_port_arch.c")
else()
    target_sources(${COMPONENT_LIB} PRIVATE "src/arch/riscv32/ohos_port_arch.c")
endif()

if(IDF_TARGET STREQUAL "esp32")
    target_sources(${COMPONENT_LIB} PRIVATE "src/targets/esp32/target_port.c")
elseif(IDF_TARGET STREQUAL "esp32s3")
    target_sources(${COMPONENT_LIB} PRIVATE "src/targets/esp32s3/target_port.c")
elseif(IDF_TARGET STREQUAL "esp32c6")
    target_sources(${COMPONENT_LIB} PRIVATE "src/targets/esp32c6/target_port.c")
else()
    target_sources(${COMPONENT_LIB} PRIVATE "src/targets/generic/target_port.c")
endif()
