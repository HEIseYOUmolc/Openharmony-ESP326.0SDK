#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
VENDOR_DIR="$ROOT_DIR/vendor"

mkdir -p "$VENDOR_DIR"

if [ ! -d "$VENDOR_DIR/esp-idf" ]; then
  git clone --branch v6.0.1 --depth 1 https://github.com/espressif/esp-idf.git "$VENDOR_DIR/esp-idf"
fi

if [ ! -d "$VENDOR_DIR/openharmony-kernel_liteos_m" ]; then
  git clone --branch OpenHarmony-v6.0-Release --depth 1 https://github.com/openharmony/kernel_liteos_m.git "$VENDOR_DIR/openharmony-kernel_liteos_m"
fi

echo "Upstream repositories are ready in $VENDOR_DIR"
echo
echo "Note: this script only fetches lightweight reference repositories."
echo "For the standard OpenHarmony source tree, run:"
echo "  make setup-ohos"
