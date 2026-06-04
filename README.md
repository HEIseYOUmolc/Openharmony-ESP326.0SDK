# OpenHarmony移植ESP32 6.0 SDK

This repository is a lightweight OpenHarmony 6.0 LiteOS-M port framework for ESP32-family chips, built on ESP-IDF 6.0.1. It provides the project skeleton, porting layers, target adapters, board profiles, and demo applications needed to validate OpenHarmony-style APIs on ESP32-class hardware.

## Features

- Supports both Xtensa and RISC-V ESP32 targets.
- Uses a three-layer porting model:
  - `components/ohos_port/src/common` for shared port APIs.
  - `components/ohos_port/src/arch` for architecture-specific glue.
  - `components/ohos_port/src/targets/<idf_target>` for chip-specific setup.
- Provides board profile templates in `board_profile/`.
- Includes a lightweight kernel shim in `components/ohos_kernel_shim`.
- Includes selectable demo applications through Kconfig.
- Includes GitHub Actions build validation for ESP-IDF based builds.

## Supported Targets

| Target | Architecture | Default Config |
| --- | --- | --- |
| `esp32` | Xtensa | `sdkconfig.defaults.esp32` |
| `esp32c6` | RISC-V 32-bit | `sdkconfig.defaults.esp32c6` |

## Directory Layout

```text
.
|-- board_profile/                 Board description templates
|-- components/
|   |-- ohos_kernel_shim/           Lightweight LiteOS-M compatibility shim
|   `-- ohos_port/                  OpenHarmony-to-ESP32 porting layer
|-- docs/                           Porting and troubleshooting notes
|-- main/                           Demo runner and demo applications
|-- tools/                          Helper scripts
|-- CMakeLists.txt                  ESP-IDF project entry
`-- Kconfig.projbuild               Demo and port configuration menu
```

## Requirements

- ESP-IDF v6.0.1
- Python and toolchain dependencies required by ESP-IDF
- An ESP32 or ESP32-C6 development board for hardware validation

Make sure the ESP-IDF environment has been activated before building:

```bash
. $IDF_PATH/export.sh
```

On Windows PowerShell with ESP-IDF installed, use the ESP-IDF PowerShell environment before running the commands below.

## Build

Build for ESP32:

```bash
idf.py set-target esp32
idf.py build
```

Build for ESP32-C6:

```bash
idf.py set-target esp32c6
idf.py build
```

Flash and monitor:

```bash
idf.py -p <serial-port> flash monitor
```

## Demo Selection

Run:

```bash
idf.py menuconfig
```

Then open `OpenHarmony ESP32 Port` and select exactly one demo. Current demos include:

- GPIO blink
- UART echo
- I2C scan
- SPI loopback
- Wi-Fi station
- TCP/HTTP networking

## Porting Notes

The project is designed to keep OpenHarmony-facing APIs stable while allowing ESP-IDF target details to live in isolated target adapters. To add a new ESP32-family target, see [docs/PORTING_GUIDE.md](docs/PORTING_GUIDE.md).

Current baseline:

- OpenHarmony: `OpenHarmony-v6.0-Release` LiteOS-M
- ESP-IDF: `v6.0.1`

## Troubleshooting

Common build, Wi-Fi, demo selection, and boot issues are documented in [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md).

## Status

This is a lightweight porting framework and validation scaffold. The next integration step is replacing the shim layer with the real LiteOS-M startup, scheduler, tick, heap, and interrupt bindings for production-grade OpenHarmony runtime behavior.
