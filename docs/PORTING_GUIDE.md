# Porting Guide

## 1. Architecture

- Common layer implements stable APIs consumed by kernel shim and demos.
- Arch layer handles Xtensa or RISC-V specific glue.
- Target layer handles per-chip setup (clock, pin defaults, capabilities).

## 2. Add a New ESP32 Target

1. Create `components/ohos_port/src/targets/<idf_target>/target_port.c`.
2. Create `components/ohos_port/src/targets/<idf_target>/Kconfig.target`.
3. Create `components/ohos_port/src/targets/<idf_target>/sdkconfig.defaults`.
4. Add a board profile yaml in `board_profile/`.
5. Update `components/ohos_port/CMakeLists.txt` for target source selection.

## 3. Integrate Real LiteOS-M

- Replace `components/ohos_kernel_shim` weak hooks with actual LiteOS-M startup.
- Bind tick interrupt callback to LiteOS-M tick handler.
- Wire real heap regions from linker symbols.

## 4. Validation

- Compile at least one Xtensa target and one RISC-V target.
- Boot and verify console logs.
- Execute each demo and confirm expected logs.
