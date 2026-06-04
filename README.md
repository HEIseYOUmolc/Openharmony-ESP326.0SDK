# OpenHarmony ESP32 6.0 SDK Port

**Language / 语言**: [中文](#中文说明) | [English](#english)

## 上游源码 / Upstream Sources

This project is organized from the ESP-IDF SDK and OpenHarmony 6.0 LiteOS-M baseline. Please confirm these upstream sources first when reproducing, auditing, or extending the port.

本工程是在 ESP-IDF SDK 与 OpenHarmony 6.0 LiteOS-M 基线之上整理的 ESP32 轻量移植框架。复现、审查或继续扩展本项目时，请优先确认以下上游源码来源。

| Project / 项目 | Repository / 仓库 | Baseline / 基线 |
| --- | --- | --- |
| ESP-IDF SDK | [https://github.com/espressif/esp-idf](https://github.com/espressif/esp-idf) | `v6.0.1` |
| OpenHarmony manifest | [https://gitcode.com/openharmony/manifest](https://gitcode.com/openharmony/manifest) | `refs/tags/OpenHarmony-v6.0-Release` |
| OpenHarmony 6.0 release notes | [https://github.com/openharmony/docs/blob/master/en/release-notes/OpenHarmony-v6.0-release.md](https://github.com/openharmony/docs/blob/master/en/release-notes/OpenHarmony-v6.0-release.md) | OpenHarmony 6.0 Release |

Get ESP-IDF:

```bash
git clone -b v6.0.1 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
. ./export.sh
```

Get OpenHarmony 6.0 Release:

```bash
repo init -u https://gitcode.com/openharmony/manifest -b refs/tags/OpenHarmony-v6.0-Release --no-repo-verify
repo sync -c
repo forall -c 'git lfs pull'
```

## 中文说明

### 项目简介

本仓库是一个基于 ESP-IDF 6.0.1 的 OpenHarmony 6.0 LiteOS-M 轻量级移植工程，目标是在 ESP32 系列芯片上验证 OpenHarmony 风格的系统适配接口、目标芯片适配层和示例应用。

工程当前定位为移植框架和验证脚手架，包含端口抽象层、架构适配层、目标芯片适配层、板级配置模板、内核兼容层以及多组可选 Demo。

### 项目特性

- 支持 ESP32 Xtensa 架构目标。
- 支持 ESP32-C6 RISC-V 32 位架构目标。
- 使用三层移植结构，便于扩展到更多 ESP32 系列芯片：
  - `components/ohos_port/src/common`：公共端口 API。
  - `components/ohos_port/src/arch`：架构相关适配。
  - `components/ohos_port/src/targets/<idf_target>`：具体芯片适配。
- 提供 `board_profile/` 板级配置模板。
- 提供 `components/ohos_kernel_shim` 轻量级 LiteOS-M 兼容层。
- 支持通过 Kconfig 选择不同 Demo。
- 已包含 GitHub Actions 构建验证配置。

### 支持目标

| 目标芯片 | 架构 | 默认配置 |
| --- | --- | --- |
| `esp32` | Xtensa | `sdkconfig.defaults.esp32` |
| `esp32c6` | RISC-V 32 位 | `sdkconfig.defaults.esp32c6` |

### 目录结构

```text
.
|-- board_profile/                 板级描述模板
|-- components/
|   |-- ohos_kernel_shim/           LiteOS-M 轻量兼容层
|   `-- ohos_port/                  OpenHarmony 到 ESP32 的移植层
|-- docs/                           移植说明和问题排查文档
|-- main/                           Demo 入口和示例应用
|-- tools/                          辅助脚本
|-- CMakeLists.txt                  ESP-IDF 工程入口
`-- Kconfig.projbuild               Demo 和移植配置菜单
```

### 环境要求

- ESP-IDF v6.0.1
- ESP-IDF 所需 Python 和交叉编译工具链
- ESP32 或 ESP32-C6 开发板

在 Linux 或 WSL 环境中，构建前请先加载 ESP-IDF 环境：

```bash
. $IDF_PATH/export.sh
```

如果使用 Windows PowerShell，请先进入 ESP-IDF PowerShell 环境，再执行后续构建命令。

### 编译方法

编译 ESP32：

```bash
idf.py set-target esp32
idf.py build
```

编译 ESP32-C6：

```bash
idf.py set-target esp32c6
idf.py build
```

烧录并打开串口监视器：

```bash
idf.py -p <serial-port> flash monitor
```

其中 `<serial-port>` 请替换为实际串口，例如 Linux/WSL 下的 `/dev/ttyUSB0`，或 Windows 下的 `COM3`。

### Demo 选择

运行：

```bash
idf.py menuconfig
```

进入 `OpenHarmony ESP32 Port` 菜单，并选择一个 Demo。当前包含：

- GPIO 闪灯
- UART 回显
- I2C 扫描
- SPI 回环
- Wi-Fi Station
- TCP/HTTP 网络示例

建议每次只选择一个 Demo，方便定位构建和运行问题。

### 移植层说明

本工程将 OpenHarmony 面向系统层的接口与 ESP-IDF 目标平台细节拆开，方便在不同芯片之间复用公共逻辑。

- 公共层负责稳定 API 和通用能力封装。
- 架构层负责 Xtensa、RISC-V 等 CPU 架构差异。
- 目标层负责具体芯片的时钟、中断、堆、外设能力和默认配置。

新增 ESP32 系列目标芯片时，可参考 [docs/PORTING_GUIDE.md](docs/PORTING_GUIDE.md)。

### 常见问题

构建失败、Wi-Fi 无法联网、Demo 输出异常、看门狗复位等问题可参考 [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)。

### 当前状态

当前版本是 OpenHarmony LiteOS-M 在 ESP32 系列芯片上的轻量移植框架和验证工程。后续可以继续将 `components/ohos_kernel_shim` 替换为真实 LiteOS-M 启动流程、调度器、系统 Tick、堆管理和中断绑定，从而逐步接近完整 OpenHarmony 运行时。

## English

### Overview

This repository provides a lightweight OpenHarmony 6.0 LiteOS-M porting framework for ESP32-family chips, built on ESP-IDF 6.0.1. It validates OpenHarmony-style system adaptation APIs, target adapters, board profiles, and demo applications on ESP32-class hardware.

The current scope is a porting framework and validation scaffold. It includes a port abstraction layer, architecture glue, target-specific adapters, board profile templates, a lightweight kernel compatibility shim, and selectable demos.

### Features

- Supports ESP32 Xtensa targets.
- Supports ESP32-C6 RISC-V 32-bit targets.
- Uses a three-layer porting structure:
  - `components/ohos_port/src/common` for shared port APIs.
  - `components/ohos_port/src/arch` for architecture-specific glue.
  - `components/ohos_port/src/targets/<idf_target>` for chip-specific adapters.
- Provides board profile templates in `board_profile/`.
- Provides a lightweight LiteOS-M compatibility shim in `components/ohos_kernel_shim`.
- Supports demo selection through Kconfig.
- Includes GitHub Actions build validation.

### Supported Targets

| Target | Architecture | Default Config |
| --- | --- | --- |
| `esp32` | Xtensa | `sdkconfig.defaults.esp32` |
| `esp32c6` | RISC-V 32-bit | `sdkconfig.defaults.esp32c6` |

### Directory Layout

```text
.
|-- board_profile/                 Board profile templates
|-- components/
|   |-- ohos_kernel_shim/           Lightweight LiteOS-M compatibility shim
|   `-- ohos_port/                  OpenHarmony-to-ESP32 porting layer
|-- docs/                           Porting and troubleshooting notes
|-- main/                           Demo entry and sample applications
|-- tools/                          Helper scripts
|-- CMakeLists.txt                  ESP-IDF project entry
`-- Kconfig.projbuild               Demo and port configuration menu
```

### Requirements

- ESP-IDF v6.0.1
- Python and cross-toolchain dependencies required by ESP-IDF
- ESP32 or ESP32-C6 development board

Before building on Linux or WSL, activate the ESP-IDF environment:

```bash
. $IDF_PATH/export.sh
```

On Windows PowerShell, enter the ESP-IDF PowerShell environment before running the build commands.

### Build

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

Flash and open the serial monitor:

```bash
idf.py -p <serial-port> flash monitor
```

Replace `<serial-port>` with the actual port, such as `/dev/ttyUSB0` on Linux/WSL or `COM3` on Windows.

### Demo Selection

Run:

```bash
idf.py menuconfig
```

Open `OpenHarmony ESP32 Port` and select one demo. Current demos include:

- GPIO blink
- UART echo
- I2C scan
- SPI loopback
- Wi-Fi Station
- TCP/HTTP networking sample

Select one demo at a time to keep build and runtime validation easy to diagnose.

### Porting Layer

The project separates OpenHarmony-facing system APIs from ESP-IDF target details so common logic can be reused across chips.

- The common layer provides stable APIs and shared capabilities.
- The architecture layer handles Xtensa and RISC-V CPU differences.
- The target layer handles chip clocks, interrupts, heap, peripheral capabilities, and default configuration.

To add a new ESP32-family target, see [docs/PORTING_GUIDE.md](docs/PORTING_GUIDE.md).

### Troubleshooting

For build failures, Wi-Fi issues, missing demo output, watchdog resets, and related problems, see [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md).

### Status

This is a lightweight OpenHarmony LiteOS-M porting framework and validation project for ESP32-family chips. The next step is to replace `components/ohos_kernel_shim` with the real LiteOS-M startup flow, scheduler, system tick, heap management, and interrupt bindings.
