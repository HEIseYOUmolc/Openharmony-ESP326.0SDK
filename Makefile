PYTHON ?= python3
ESP_IDF_VERSION ?= v6.0.1
IDF ?= ESP_IDF_VERSION=$(ESP_IDF_VERSION) bash tools/idf_env.sh
KCONFIG_CONFIG ?= .ohos_target.config
KCONFIG ?= Kconfig
KCONFIG_MENUCONFIG := $(shell command -v menuconfig 2>/dev/null)

.PHONY: help setup-idf setup-ohos target-menuconfig menuconfig print-target print-project set-target set-project idf-menuconfig build flash monitor clean fullclean

help:
	@echo "OpenHarmony ESP32 port make targets"
	@echo
	@echo "  make setup-idf       Install ESP-IDF v6.0.1 for this project"
	@echo "  make setup-ohos      Download OpenHarmony v6.0 Release source"
	@echo "  make setup-ohos OHOS_VERSION=6.1"
	@echo "  make target-menuconfig"
	@echo "                       Select ESP-IDF chip target only"
	@echo "  make menuconfig      Select chip target and vendor/yango project"
	@echo "  make print-target    Show selected chip target"
	@echo "  make print-project   Show selected vendor/yango project"
	@echo "  make set-target      Apply selected target with idf.py set-target"
	@echo "  make set-project PROJECT=<name>"
	@echo "                       Set vendor/yango project directly"
	@echo "  make idf-menuconfig  Open ESP-IDF project menuconfig"
	@echo "  make build           Apply selected target and build"
	@echo "  make flash           Flash with idf.py"
	@echo "  make monitor         Monitor with idf.py"
	@echo "  make clean           Clean build output"
	@echo "  make fullclean       Full clean build output"

setup-idf:
	ESP_IDF_VERSION=$(ESP_IDF_VERSION) bash tools/setup_esp_idf.sh

setup-ohos:
	bash tools/setup_openharmony.sh

target-menuconfig:
ifneq ($(KCONFIG_MENUCONFIG),)
	KCONFIG_CONFIG=$(KCONFIG_CONFIG) menuconfig $(KCONFIG)
	$(PYTHON) tools/select_target.py --sync-from-kconfig
else
	$(PYTHON) tools/select_target.py --menuconfig
endif

menuconfig: target-menuconfig

print-target:
	@$(PYTHON) tools/select_target.py --print-target-name

print-project:
	@$(PYTHON) tools/select_target.py --print-project-name

set-project:
	@if [ -z "$(PROJECT)" ]; then \
		echo "Usage: make set-project PROJECT=<name>"; \
		echo "Valid projects: gpio_blink uart_echo i2c_scan spi_loop wifi_sta tcp_http"; \
		exit 1; \
	fi
	$(PYTHON) tools/select_target.py --set-project "$(PROJECT)"

prepare-build-dir:
	@if [ -d build ] && [ ! -f build/CMakeCache.txt ]; then \
		backup="build.non-cmake.$$(date +%Y%m%d-%H%M%S)"; \
		echo "build/ is not an ESP-IDF CMake build directory."; \
		echo "Moving it to $$backup before idf.py set-target."; \
		mv build "$$backup"; \
	fi

set-target: prepare-build-dir
	$(IDF) set-target $$($(PYTHON) tools/select_target.py --print-target)

idf-menuconfig: set-target
	$(PYTHON) tools/select_target.py --apply-project
	$(IDF) menuconfig

build: set-target
	$(PYTHON) tools/select_target.py --apply-project
	$(IDF) build

flash:
	$(IDF) flash

monitor:
	$(IDF) monitor

clean:
	$(IDF) clean

fullclean:
	$(IDF) fullclean
