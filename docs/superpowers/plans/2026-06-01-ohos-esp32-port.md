# OpenHarmony LiteOS-M on ESP32 Implementation Notes

This repository contains the initial implementation scaffold for the plan created on 2026-06-01.

Implemented in this phase:
- Unified port APIs and layered directory layout
- Xtensa and RISC-V architecture glue stubs
- Target templates for esp32 and esp32c6
- Multi-demo framework with GPIO/UART/I2C/SPI/Wi-Fi/TCP HTTP
- Build matrix and board profile templates

Pending for hardware bring-up:
- Replace shim startup with full LiteOS-M scheduler boot path
- Tune per-board pins and partition tables
- Validate and close all hardware acceptance tests
