#!/usr/bin/env python3
"""Select the ESP-IDF target and demo project used by the Makefile."""

from __future__ import annotations

import argparse
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]
CONFIG_PATH = PROJECT_ROOT / ".ohos_target"
PROJECT_CONFIG_PATH = PROJECT_ROOT / ".ohos_project"
KCONFIG_CONFIG_PATH = PROJECT_ROOT / ".ohos_target.config"
SDKCONFIG_PATH = PROJECT_ROOT / "sdkconfig"

TARGETS = {
    "esp32": {
        "label": "ESP32",
        "arch": "Xtensa",
        "defaults": "sdkconfig.defaults.esp32",
    },
    "esp32s3": {
        "label": "ESP32-S3",
        "arch": "Xtensa LX7",
        "defaults": "sdkconfig.defaults.esp32s3",
    },
    "esp32c6": {
        "label": "ESP32-C6",
        "arch": "RISC-V 32-bit",
        "defaults": "sdkconfig.defaults.esp32c6",
    },
}

DEFAULT_TARGET = "esp32"

PROJECTS = {
    "gpio_blink": {
        "label": "vendor/yango/gpio_blink - GPIO blink",
        "symbol": "CONFIG_OHOS_DEMO_GPIO_BLINK",
    },
    "uart_echo": {
        "label": "vendor/yango/uart_echo - UART echo",
        "symbol": "CONFIG_OHOS_DEMO_UART_ECHO",
    },
    "i2c_scan": {
        "label": "vendor/yango/i2c_scan - I2C scan",
        "symbol": "CONFIG_OHOS_DEMO_I2C_SCAN",
    },
    "spi_loop": {
        "label": "vendor/yango/spi_loop - SPI loopback",
        "symbol": "CONFIG_OHOS_DEMO_SPI_LOOP",
    },
    "wifi_sta": {
        "label": "vendor/yango/wifi_sta - Wi-Fi station",
        "symbol": "CONFIG_OHOS_DEMO_WIFI_STA",
    },
    "tcp_http": {
        "label": "vendor/yango/tcp_http - TCP and HTTP client",
        "symbol": "CONFIG_OHOS_DEMO_TCP_HTTP",
    },
}

DEFAULT_PROJECT = "gpio_blink"


def normalize_target(value: str | None) -> str:
    target = (value or DEFAULT_TARGET).strip().lower()
    if target not in TARGETS:
        valid = ", ".join(TARGETS)
        raise SystemExit(f"Unsupported target '{target}'. Valid targets: {valid}")
    return target


def normalize_project(value: str | None) -> str:
    project = (value or DEFAULT_PROJECT).strip().lower().replace("-", "_")
    if project not in PROJECTS:
        valid = ", ".join(PROJECTS)
        raise SystemExit(f"Unsupported project '{project}'. Valid projects: {valid}")
    return project


def read_target() -> str:
    kconfig_target = read_kconfig_target()
    if kconfig_target:
        return kconfig_target

    if not CONFIG_PATH.exists():
        return DEFAULT_TARGET

    for line in CONFIG_PATH.read_text(encoding="utf-8").splitlines():
        if line.startswith("IDF_TARGET="):
            return normalize_target(line.split("=", 1)[1])

    return DEFAULT_TARGET


def read_project() -> str:
    kconfig_project = read_kconfig_project()
    if kconfig_project:
        return kconfig_project

    if not PROJECT_CONFIG_PATH.exists():
        return DEFAULT_PROJECT

    for line in PROJECT_CONFIG_PATH.read_text(encoding="utf-8").splitlines():
        if line.startswith("OHOS_PROJECT="):
            return normalize_project(line.split("=", 1)[1])

    return DEFAULT_PROJECT


def read_kconfig_target() -> str | None:
    if not KCONFIG_CONFIG_PATH.exists():
        return None

    config = KCONFIG_CONFIG_PATH.read_text(encoding="utf-8")
    if "CONFIG_OHOS_TARGET_ESP32S3=y" in config:
        return "esp32s3"
    if "CONFIG_OHOS_TARGET_ESP32C6=y" in config:
        return "esp32c6"
    if "CONFIG_OHOS_TARGET_ESP32=y" in config:
        return "esp32"

    return None


def read_kconfig_project() -> str | None:
    if not KCONFIG_CONFIG_PATH.exists():
        return None

    config = KCONFIG_CONFIG_PATH.read_text(encoding="utf-8")
    for project, meta in PROJECTS.items():
        if f"{meta['symbol']}=y" in config:
            return project

    return None


def write_target(target: str) -> None:
    target = normalize_target(target)
    CONFIG_PATH.write_text(f"IDF_TARGET={target}\n", encoding="utf-8", newline="\n")
    write_kconfig_config(target, read_project())


def write_project(project: str) -> None:
    project = normalize_project(project)
    PROJECT_CONFIG_PATH.write_text(f"OHOS_PROJECT={project}\n", encoding="utf-8", newline="\n")
    write_kconfig_config(read_target(), project)


def write_kconfig_target(target: str) -> None:
    write_kconfig_config(target, read_project())


def write_kconfig_config(target: str, project: str) -> None:
    target = normalize_target(target)
    project = normalize_project(project)

    lines = []
    for candidate in TARGETS:
        symbol = f"CONFIG_OHOS_TARGET_{candidate.upper()}"
        if candidate == target:
            lines.append(f"{symbol}=y")
        else:
            lines.append(f"# {symbol} is not set")
    lines.append(f'CONFIG_OHOS_IDF_TARGET="{target}"')
    lines.append("")
    for candidate, meta in PROJECTS.items():
        symbol = meta["symbol"]
        if candidate == project:
            lines.append(f"{symbol}=y")
        else:
            lines.append(f"# {symbol} is not set")
    lines.extend([f'CONFIG_OHOS_SELECTED_PROJECT="{project}"', ""])
    content = "\n".join(lines)

    KCONFIG_CONFIG_PATH.write_text(content, encoding="utf-8", newline="\n")


def sync_from_kconfig() -> None:
    target = read_kconfig_target()
    if not target:
        target = read_target()
    project = read_kconfig_project()
    if not project:
        project = read_project()
    write_kconfig_config(target, project)
    write_target(target)
    write_project(project)
    apply_project_to_sdkconfig(project)
    print_target_name(target)
    print_project_name(project)


def print_target_name(target: str) -> None:
    meta = TARGETS[target]
    print(f"{meta['label']} ({target}, {meta['arch']}, {meta['defaults']})")


def print_project_name(project: str) -> None:
    meta = PROJECTS[project]
    print(f"{meta['label']} ({project}, {meta['symbol']})")


def apply_project_to_sdkconfig(project: str | None = None) -> None:
    project = normalize_project(project or read_project())
    existing_lines = []
    if SDKCONFIG_PATH.exists():
        existing_lines = SDKCONFIG_PATH.read_text(encoding="utf-8").splitlines()

    project_symbols = {meta["symbol"] for meta in PROJECTS.values()}
    filtered_lines = []
    for line in existing_lines:
        if any(line == f"{symbol}=y" or line == f"# {symbol} is not set" for symbol in project_symbols):
            continue
        filtered_lines.append(line)

    project_lines = []
    for candidate, meta in PROJECTS.items():
        symbol = meta["symbol"]
        if candidate == project:
            project_lines.append(f"{symbol}=y")
        else:
            project_lines.append(f"# {symbol} is not set")

    content = "\n".join(filtered_lines + project_lines + [""])
    SDKCONFIG_PATH.write_text(content, encoding="utf-8", newline="\n")
    print("Applied project to sdkconfig:")
    print_project_name(project)


def menuconfig() -> None:
    current = read_target()
    current_project = read_project()
    choices = list(TARGETS)
    project_choices = list(PROJECTS)

    print("OpenHarmony ESP32 configuration")
    print("--------------------------------")
    print("Target chip:")
    for index, target in enumerate(choices, start=1):
        marker = "*" if target == current else " "
        meta = TARGETS[target]
        print(f"{index}. [{marker}] {meta['label']} - {target} / {meta['arch']}")

    print()
    raw = input(f"Select target [1-{len(choices)}] (Enter keeps {current}): ").strip()
    if not raw:
        selected = current
    else:
        try:
            selected = choices[int(raw) - 1]
        except (ValueError, IndexError) as exc:
            raise SystemExit(f"Invalid selection: {raw}") from exc

    print()
    print("vendor/yango project to run:")
    for index, project in enumerate(project_choices, start=1):
        marker = "*" if project == current_project else " "
        meta = PROJECTS[project]
        print(f"{index}. [{marker}] {meta['label']} - {project}")

    print()
    raw = input(f"Select project [1-{len(project_choices)}] (Enter keeps {current_project}): ").strip()
    if not raw:
        selected_project = current_project
    else:
        try:
            selected_project = project_choices[int(raw) - 1]
        except (ValueError, IndexError) as exc:
            raise SystemExit(f"Invalid selection: {raw}") from exc

    write_target(selected)
    write_project(selected_project)
    apply_project_to_sdkconfig(selected_project)
    print("Selected target:")
    print_target_name(selected)
    print("Selected project:")
    print_project_name(selected_project)
    print(f"Wrote {CONFIG_PATH.name}, {PROJECT_CONFIG_PATH.name}, and {SDKCONFIG_PATH.name}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--menuconfig", action="store_true", help="open the target selection menu")
    group.add_argument("--print-target", action="store_true", help="print selected ESP-IDF target")
    group.add_argument("--print-target-name", action="store_true", help="print selected target details")
    group.add_argument("--print-project", action="store_true", help="print selected project")
    group.add_argument("--print-project-name", action="store_true", help="print selected project details")
    group.add_argument("--set-target", choices=TARGETS.keys(), help="write selected ESP-IDF target")
    group.add_argument("--set-project", choices=PROJECTS.keys(), help="write selected project")
    group.add_argument("--apply-project", action="store_true", help="apply selected project to sdkconfig")
    group.add_argument("--sync-from-kconfig", action="store_true", help="sync .ohos_target from Kconfig output")
    args = parser.parse_args()

    if args.menuconfig:
        menuconfig()
    elif args.print_target:
        print(read_target())
    elif args.print_target_name:
        print_target_name(read_target())
    elif args.print_project:
        print(read_project())
    elif args.print_project_name:
        print_project_name(read_project())
    elif args.set_target:
        write_target(args.set_target)
        print_target_name(args.set_target)
    elif args.set_project:
        write_project(args.set_project)
        apply_project_to_sdkconfig(args.set_project)
    elif args.apply_project:
        apply_project_to_sdkconfig()
    elif args.sync_from_kconfig:
        sync_from_kconfig()
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
