#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
SDK_ROOT="$(cd "${PROJECT_ROOT}/.." && pwd)"

ESP_IDF_VERSION="${ESP_IDF_VERSION:-v6.0.1}"
ESP_IDF_DIR="${ESP_IDF_DIR:-${SDK_ROOT}/esp-idf-${ESP_IDF_VERSION#v}}"
IDF_TOOLS_PATH="${IDF_TOOLS_PATH:-${SDK_ROOT}/.espressif}"

if command -v idf.py >/dev/null 2>&1; then
    exec idf.py "$@"
fi

if [ ! -f "${ESP_IDF_DIR}/export.sh" ]; then
    echo "ESP-IDF is not configured." >&2
    echo "Run:" >&2
    echo "  make setup-idf" >&2
    echo "or load an existing ESP-IDF environment manually." >&2
    exit 127
fi

export IDF_TOOLS_PATH
# shellcheck disable=SC1090
. "${ESP_IDF_DIR}/export.sh" >/dev/null

exec idf.py "$@"
