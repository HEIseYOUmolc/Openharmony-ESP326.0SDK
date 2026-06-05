#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
SDK_ROOT="$(cd "${PROJECT_ROOT}/.." && pwd)"

ESP_IDF_VERSION="${ESP_IDF_VERSION:-v6.0.1}"
ESP_IDF_DIR="${ESP_IDF_DIR:-${SDK_ROOT}/esp-idf-${ESP_IDF_VERSION#v}}"
IDF_TOOLS_PATH="${IDF_TOOLS_PATH:-${SDK_ROOT}/.espressif}"

echo "ESP-IDF version: ${ESP_IDF_VERSION}"
echo "ESP-IDF dir:     ${ESP_IDF_DIR}"
echo "IDF tools path:  ${IDF_TOOLS_PATH}"
echo

if ! command -v git >/dev/null 2>&1; then
    echo "git is required. Install it first, for example:"
    echo "  sudo apt update && sudo apt install -y git"
    exit 127
fi

if ! command -v python3 >/dev/null 2>&1; then
    echo "python3 is required. Install it first, for example:"
    echo "  sudo apt update && sudo apt install -y python3 python3-pip python3-venv"
    exit 127
fi

if [ ! -d "${ESP_IDF_DIR}/.git" ]; then
    if [ -e "${ESP_IDF_DIR}" ]; then
        echo "Found an incomplete ESP-IDF directory without .git:"
        echo "  ${ESP_IDF_DIR}"
        echo
        echo "Move it away and retry, for example:"
        echo "  mv \"${ESP_IDF_DIR}\" \"${ESP_IDF_DIR}.broken-\$(date +%Y%m%d-%H%M%S)\""
        echo "  make setup-idf"
        exit 1
    fi

    echo "Cloning ESP-IDF..."
    git clone --depth 1 -b "${ESP_IDF_VERSION}" https://github.com/espressif/esp-idf.git "${ESP_IDF_DIR}"
    git -C "${ESP_IDF_DIR}" submodule update --init --recursive --depth 1
else
    echo "ESP-IDF directory already exists. Updating submodules..."
    git -C "${ESP_IDF_DIR}" fetch --tags
    git -C "${ESP_IDF_DIR}" checkout "${ESP_IDF_VERSION}"
    git -C "${ESP_IDF_DIR}" submodule update --init --recursive --depth 1
fi

echo
echo "Installing ESP-IDF tools..."
export IDF_TOOLS_PATH
"${ESP_IDF_DIR}/install.sh" esp32,esp32c6

cat > "${SDK_ROOT}/export_esp_idf.sh" <<EOF
#!/usr/bin/env bash
export IDF_TOOLS_PATH="${IDF_TOOLS_PATH}"
. "${ESP_IDF_DIR}/export.sh"
EOF
chmod +x "${SDK_ROOT}/export_esp_idf.sh"

echo
echo "ESP-IDF setup complete."
echo "To load it manually:"
echo "  . ${SDK_ROOT}/export_esp_idf.sh"
echo
echo "This project Makefile also uses this installation automatically."
