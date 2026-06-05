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

Project helper for the standard OpenHarmony source tree:

```bash
make setup-ohos
```

Install the GitCode repo launcher first if `repo` is unavailable:

```bash
bash tools/install_gitcode_repo.sh
export PATH="$HOME/bin:$PATH"
```

Download the project baseline OpenHarmony 6.0 source:

```bash
make setup-ohos
```

This downloads OpenHarmony `OpenHarmony-v6.0-Release` from `https://gitcode.com/openharmony/manifest` to:

```text
../openharmony-v6.0-release
```

The LiteOS-M kernel source will be available at:

```text
../openharmony-v6.0-release/kernel/liteos_m
```

To follow the OpenHarmony 6.1 release-note source retrieval method:

```bash
make setup-ohos OHOS_VERSION=6.1
```

Optional variables:

```bash
OHOS_DOWNLOAD_MODE=ssh    # use git@gitcode.com:openharmony/manifest.git
OHOS_DOWNLOAD_MODE=https  # use https://gitcode.com/openharmony/manifest
OHOS_REF_KIND=tag         # use refs/tags/OpenHarmony-vX.Y-Release
OHOS_REF_KIND=branch      # use OpenHarmony-X.Y-Release branch
```

## 中文说明

### 项目简介

本仓库是一个基于 ESP-IDF 6.0.1 的 OpenHarmony 6.0 LiteOS-M 轻量级移植工程，目标是在 ESP32 系列芯片上验证 OpenHarmony 风格的系统适配接口、目标芯片适配层和示例应用。

工程当前定位为移植框架和验证脚手架，包含端口抽象层、架构适配层、目标芯片适配层、板级配置模板、内核兼容层以及多组可选 Demo。

### 项目特性

- 支持 ESP32 Xtensa 架构目标。
- 支持 ESP32-S3 Xtensa LX7 架构目标。
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
| `esp32s3` | Xtensa LX7 | `sdkconfig.defaults.esp32s3` |
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
- ESP32、ESP32-S3 或 ESP32-C6 开发板

如果本机尚未安装 ESP-IDF，可直接在项目根目录执行：

```bash
make setup-idf
```

该命令会下载 ESP-IDF `v6.0.1` 到 `../esp-idf-6.0.1`，并将工具链安装到 `../.espressif`。安装完成后会生成 `../export_esp_idf.sh`，用于手动加载环境。

手动加载方式：

```bash
. ../export_esp_idf.sh
```

通常情况下，项目根目录的 `Makefile` 会自动通过 `tools/idf_env.sh` 加载上述 ESP-IDF 安装，因此可以直接使用 `make build`。

### 编译方法

选择目标芯片：

```bash
make menuconfig
```

该命令会在同一个配置入口里设置两件事：

- `Target Chip`：选择 ESP32、ESP32-S3 或 ESP32-C6。
- `Project To Run`：选择当前要运行的项目/Demo，例如 GPIO、UART、I2C、SPI、Wi-Fi、TCP/HTTP。

芯片选择会写入本地 `.ohos_target`，项目选择会写入本地 `.ohos_project`，并同步到 ESP-IDF 的 `sdkconfig`。加载 ESP-IDF 环境后，`make build` 会自动调用 `idf.py set-target <target>` 应用芯片目标，并按当前项目选择编译运行入口。

`make menuconfig` 参考 OpenHarmony LiteOS-M 的 Makefile/Kconfig 组织方式：如果系统中存在 `menuconfig` 工具，会优先读取根目录 `Kconfig` 并生成 `.ohos_target.config`；如果没有该工具，则回退到文本菜单。`.ohos_target` 和 `.ohos_target.config` 都是本地芯片选择文件，不会提交到仓库。

如果只想选择芯片，不进入 Demo 子项目配置菜单：

```bash
make target-menuconfig
```

如果想用命令直接设置要跑哪个项目：

```bash
make set-project PROJECT=gpio_blink
make set-project PROJECT=uart_echo
make set-project PROJECT=i2c_scan
make set-project PROJECT=spi_loop
make set-project PROJECT=wifi_sta
make set-project PROJECT=tcp_http
```

查看当前选择：

```bash
make print-target
make print-project
```

单独进入 ESP-IDF 项目配置菜单，继续选择 Demo、Wi-Fi、GPIO 等配置：

```bash
make idf-menuconfig
```

`make build` 会在切换芯片前检查 `build/`。如果 `build/` 不是 ESP-IDF CMake 构建目录，会先重命名为 `build.non-cmake.<timestamp>`，避免 `idf.py set-target` 拒绝处理未知目录。

编译 ESP32：

```bash
make menuconfig
make build
```

编译 ESP32-C6：

```bash
make menuconfig
make build
```

编译 ESP32-S3：

```bash
make menuconfig
make build
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

### LiteOS-M 接入计划

本工程后续移植目标应是标准 OpenHarmony LiteOS-M 内核源码，而不是长期停留在临时 `ohos_kernel_shim` 兼容层。内核基线优先来自：

```text
../openharmony-v6.0-release/kernel/liteos_m
../openharmony-v6.1-release/kernel/liteos_m
https://gitcode.com/openharmony/kernel_liteos_m
```

`kernel_liteos_m` 是面向 MCU 等小资源设备的轻量级 IoT 内核，源码结构包含架构层、可选组件、驱动、KAL 内核抽象层、最小内核功能集、测试集、工具和通用工具目录。ESP32 移植应分阶段推进：

1. 保留 `components/ohos_port` 作为 ESP32 板级和 SoC 适配边界。
2. 将 `components/ohos_kernel_shim` 从弱兼容层逐步替换为 `kernel/liteos_m` 的真实头文件和内核对象。
3. 将 ESP-IDF 能力映射到 LiteOS-M 底层需求：
   - 中断进入/退出和临界区
   - 系统 tick 源和调度 tick hook
   - 堆区域注册
   - 任务、信号量、互斥锁、队列、定时器
   - UART 控制台、panic 和日志输出
4. 优先复用或扩展上游 `arch/xtensa`、`arch/risc-v` 架构层，避免在本仓库重复维护架构代码。
5. 在 LiteOS-M 接入稳定前，ESP-IDF 仍负责启动、时钟、flash、Wi-Fi 和外设驱动所有权。
6. 按最小闭环验证：启动日志、tick、任务创建、堆分配、GPIO/UART Demo，再逐步打开网络 Demo。

在上述步骤完成前，`components/ohos_kernel_shim` 只能视为过渡层。

### 常见问题

构建失败、Wi-Fi 无法联网、Demo 输出异常、看门狗复位等问题可参考 [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)。

### 当前状态

当前版本是 OpenHarmony LiteOS-M 在 ESP32 系列芯片上的轻量移植框架和验证工程。后续可以继续将 `components/ohos_kernel_shim` 替换为真实 LiteOS-M 启动流程、调度器、系统 Tick、堆管理和中断绑定，从而逐步接近完整 OpenHarmony 运行时。

### Yango 工程目录约定

后续可运行工程统一放在：

```text
vendor/yango/<project_name>
```

`main/` 不再存放具体工程源码，只保留 ESP-IDF 启动入口和工程分发器。当前已有工程：

```text
vendor/yango/gpio_blink
vendor/yango/uart_echo
vendor/yango/i2c_scan
vendor/yango/spi_loop
vendor/yango/wifi_sta
vendor/yango/tcp_http
```

通过 `make menuconfig` 进入 `Yango Project To Run` 菜单即可选择当前要运行的工程。新增工程时，需要同步更新：

- `vendor/yango/CMakeLists.txt`
- `Kconfig`
- `Kconfig.projbuild`
- `tools/select_target.py`
- `README.md`
- `AGENTS.md`

## English

### Overview

This repository provides a lightweight OpenHarmony 6.0 LiteOS-M porting framework for ESP32-family chips, built on ESP-IDF 6.0.1. It validates OpenHarmony-style system adaptation APIs, target adapters, board profiles, and demo applications on ESP32-class hardware.

The current scope is a porting framework and validation scaffold. It includes a port abstraction layer, architecture glue, target-specific adapters, board profile templates, a lightweight kernel compatibility shim, and runnable projects under `vendor/yango`.

### Features

- Supports ESP32 Xtensa targets.
- Supports ESP32-S3 Xtensa LX7 targets.
- Supports ESP32-C6 RISC-V 32-bit targets.
- Uses a three-layer porting structure:
  - `components/ohos_port/src/common` for shared port APIs.
  - `components/ohos_port/src/arch` for architecture-specific glue.
  - `components/ohos_port/src/targets/<idf_target>` for chip-specific adapters.
- Provides board profile templates in `board_profile/`.
- Provides a lightweight LiteOS-M compatibility shim in `components/ohos_kernel_shim`.
- Supports `vendor/yango/<project>` selection through `make menuconfig`.
- Includes GitHub Actions build validation.

### Supported Targets

| Target | Architecture | Default Config |
| --- | --- | --- |
| `esp32` | Xtensa | `sdkconfig.defaults.esp32` |
| `esp32s3` | Xtensa LX7 | `sdkconfig.defaults.esp32s3` |
| `esp32c6` | RISC-V 32-bit | `sdkconfig.defaults.esp32c6` |

### Directory Layout

```text
.
|-- board_profile/                 Board profile templates
|-- components/
|   |-- ohos_kernel_shim/           Lightweight LiteOS-M compatibility shim
|   `-- ohos_port/                  OpenHarmony-to-ESP32 porting layer
|-- docs/                           Porting and troubleshooting notes
|-- main/                           ESP-IDF startup entry and project dispatcher
|-- vendor/yango/                   Runnable projects under the Yango namespace
|-- tools/                          Helper scripts
|-- CMakeLists.txt                  ESP-IDF project entry
`-- Kconfig.projbuild               Demo and port configuration menu
```

### Requirements

- ESP-IDF v6.0.1
- Python and cross-toolchain dependencies required by ESP-IDF
- ESP32, ESP32-S3, or ESP32-C6 development board

If ESP-IDF is not installed locally, run this from the project root:

```bash
make setup-idf
```

This downloads ESP-IDF `v6.0.1` to `../esp-idf-6.0.1` and installs tools to `../.espressif`. It also generates `../export_esp_idf.sh` for manual environment loading.

Manual loading:

```bash
. ../export_esp_idf.sh
```

In normal use, the root `Makefile` automatically loads this ESP-IDF installation through `tools/idf_env.sh`, so `make build` can be used directly.

To download the standard OpenHarmony source tree for LiteOS-M integration:

```bash
make setup-ohos
```

This uses `https://gitcode.com/openharmony/manifest` and checks out `refs/tags/OpenHarmony-v6.0-Release` into `../openharmony-v6.0-release`.

For the official OpenHarmony 6.1 release-note flow:

```bash
bash tools/install_gitcode_repo.sh
export PATH="$HOME/bin:$PATH"
make setup-ohos OHOS_VERSION=6.1
```

### Build

Select the chip target:

```bash
make menuconfig
```

This command configures two things in the same menu:

- `Target Chip`: ESP32, ESP32-S3, or ESP32-C6.
- `Yango Project To Run`: the active `vendor/yango/<project>` project, such as GPIO, UART, I2C, SPI, Wi-Fi, or TCP/HTTP.

The chip selection is written to `.ohos_target`; the project selection is written to `.ohos_project` and synchronized into ESP-IDF `sdkconfig`. After the ESP-IDF environment is loaded, `make build` automatically applies the chip target through `idf.py set-target <target>` and runs the selected `vendor/yango` project entry.

`make menuconfig` follows the OpenHarmony LiteOS-M Makefile/Kconfig pattern. If the `menuconfig` tool is available, it uses the top-level `Kconfig` and writes `.ohos_target.config`; otherwise it falls back to a text menu. Both `.ohos_target` and `.ohos_target.config` are local chip-selection files and are ignored by Git.

To select only the chip target without opening demo project configuration:

```bash
make target-menuconfig
```

To set the project directly from the command line:

```bash
make set-project PROJECT=gpio_blink
make set-project PROJECT=uart_echo
make set-project PROJECT=i2c_scan
make set-project PROJECT=spi_loop
make set-project PROJECT=wifi_sta
make set-project PROJECT=tcp_http
```

Show the current selection:

```bash
make print-target
make print-project
```

Open only the ESP-IDF project configuration menu for demos, Wi-Fi, GPIO, and other options:

```bash
make idf-menuconfig
```

`make build` checks `build/` before switching the chip target. If `build/` is not an ESP-IDF CMake build directory, it is renamed to `build.non-cmake.<timestamp>` so `idf.py set-target` will not fail while protecting unknown files.

Build for ESP32:

```bash
make menuconfig
make build
```

Build for ESP32-C6:

```bash
make menuconfig
make build
```

Build for ESP32-S3:

```bash
make menuconfig
make build
```

Flash and open the serial monitor:

```bash
idf.py -p <serial-port> flash monitor
```

Replace `<serial-port>` with the actual port, such as `/dev/ttyUSB0` on Linux/WSL or `COM3` on Windows.

### Yango Project Selection

Run:

```bash
make menuconfig
```

Open `Yango Project To Run` and select one `vendor/yango` project. Current projects include:

- `vendor/yango/gpio_blink`
- `vendor/yango/uart_echo`
- `vendor/yango/i2c_scan`
- `vendor/yango/spi_loop`
- `vendor/yango/wifi_sta`
- `vendor/yango/tcp_http`

New runnable projects must be added under `vendor/yango/<project_name>` and registered in the top-level `Kconfig`, `Kconfig.projbuild`, `tools/select_target.py`, and `vendor/yango/CMakeLists.txt`.

### Porting Layer

The project separates OpenHarmony-facing system APIs from ESP-IDF target details so common logic can be reused across chips.

- The common layer provides stable APIs and shared capabilities.
- The architecture layer handles Xtensa and RISC-V CPU differences.
- The target layer handles chip clocks, interrupts, heap, peripheral capabilities, and default configuration.

To add a new ESP32-family target, see [docs/PORTING_GUIDE.md](docs/PORTING_GUIDE.md).

### LiteOS-M Integration Plan

The long-term porting target is the standard OpenHarmony LiteOS-M kernel source, not the temporary compatibility shim alone. Use one of these upstream source locations as the kernel baseline:

```text
../openharmony-v6.0-release/kernel/liteos_m
../openharmony-v6.1-release/kernel/liteos_m
https://gitcode.com/openharmony/kernel_liteos_m
```

The `kernel_liteos_m` project defines LiteOS-M as a lightweight IoT kernel for resource-constrained MCU devices. Its source layout includes the architecture layer, optional components, drivers, kernel abstraction layer, minimum kernel core, testsuites, tools, and common utilities. The ESP32 port should therefore migrate in stages:

1. Keep `components/ohos_port` as the ESP32 board and SoC adaptation boundary.
2. Replace `components/ohos_kernel_shim` weak compatibility behavior with real LiteOS-M headers and kernel objects from `kernel/liteos_m`.
3. Map ESP-IDF primitives to LiteOS-M low-level requirements:
   - interrupt enter/exit and critical sections
   - system tick source and scheduler tick hook
   - heap region registration
   - task, semaphore, mutex, queue, and timer primitives
   - UART console and panic/log output
4. Reuse or extend the upstream `arch/xtensa` and `arch/risc-v` layers where possible instead of duplicating architecture code in this repository.
5. Keep ESP-IDF startup, clock, flash, Wi-Fi, and peripheral ownership in the ESP-IDF side until the LiteOS-M integration is proven stable.
6. Validate with the smallest target first: boot banner, tick, task creation, heap allocation, and one GPIO/UART demo on ESP32, ESP32-S3, and ESP32-C6 before enabling networking demos.

Until these steps are complete, `components/ohos_kernel_shim` should be treated as a transition layer only.

### Troubleshooting

For build failures, Wi-Fi issues, missing demo output, watchdog resets, and related problems, see [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md).

### Status

This is a lightweight OpenHarmony LiteOS-M porting framework and validation project for ESP32-family chips. The next step is to replace `components/ohos_kernel_shim` with the real LiteOS-M startup flow, scheduler, system tick, heap management, and interrupt bindings.
