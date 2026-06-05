#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
SDK_ROOT="$(cd "${PROJECT_ROOT}/.." && pwd)"

OHOS_VERSION="${OHOS_VERSION:-6.0}"
OHOS_DOWNLOAD_MODE="${OHOS_DOWNLOAD_MODE:-https}"
OHOS_REF_KIND="${OHOS_REF_KIND:-tag}"
OHOS_MANIFEST_URL="${OHOS_MANIFEST_URL:-}"
OHOS_MANIFEST_BRANCH="${OHOS_MANIFEST_BRANCH:-}"
OHOS_DIR="${OHOS_DIR:-}"
OHOS_JOBS="${OHOS_JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"
OHOS_DEPTH="${OHOS_DEPTH:-0}"

case "${OHOS_VERSION}" in
    6.0)
        DEFAULT_BRANCH="OpenHarmony-6.0-Release"
        DEFAULT_TAG="refs/tags/OpenHarmony-v6.0-Release"
        DEFAULT_DIR="${SDK_ROOT}/openharmony-v6.0-release"
        ;;
    6.1)
        DEFAULT_BRANCH="OpenHarmony-6.1-Release"
        DEFAULT_TAG="refs/tags/OpenHarmony-v6.1-Release"
        DEFAULT_DIR="${SDK_ROOT}/openharmony-v6.1-release"
        ;;
    *)
        echo "Unsupported OHOS_VERSION: ${OHOS_VERSION}"
        echo "Supported values: 6.0, 6.1"
        exit 2
        ;;
esac

if [ -z "${OHOS_MANIFEST_URL}" ]; then
    if [ "${OHOS_DOWNLOAD_MODE}" = "ssh" ]; then
        OHOS_MANIFEST_URL="git@gitcode.com:openharmony/manifest.git"
    else
        OHOS_MANIFEST_URL="https://gitcode.com/openharmony/manifest"
    fi
fi

if [ -z "${OHOS_MANIFEST_BRANCH}" ]; then
    if [ "${OHOS_REF_KIND}" = "branch" ]; then
        OHOS_MANIFEST_BRANCH="${DEFAULT_BRANCH}"
    else
        OHOS_MANIFEST_BRANCH="${DEFAULT_TAG}"
    fi
fi

if [ -z "${OHOS_DIR}" ]; then
    OHOS_DIR="${DEFAULT_DIR}"
fi

echo "OpenHarmony manifest: ${OHOS_MANIFEST_URL}"
echo "OpenHarmony branch:   ${OHOS_MANIFEST_BRANCH}"
echo "OpenHarmony dir:      ${OHOS_DIR}"
echo "Sync jobs:            ${OHOS_JOBS}"
echo "Clone depth:          ${OHOS_DEPTH} (0 means full history)"
echo

if ! command -v git >/dev/null 2>&1; then
    echo "git is required. Install it first:"
    echo "  sudo apt update && sudo apt install -y git"
    exit 127
fi

if ! command -v repo >/dev/null 2>&1; then
    echo "repo command is required."
    echo "Install GitCode's official repo launcher first:"
    echo "  bash tools/install_gitcode_repo.sh"
    echo "  export PATH=\"\$HOME/bin:\$PATH\""
    echo
    echo "This follows the OpenHarmony release-note source retrieval instructions."
    exit 127
fi

if ! command -v git-lfs >/dev/null 2>&1; then
    echo "git-lfs is recommended for OpenHarmony binary assets. Install it first:"
    echo "  sudo apt update && sudo apt install -y git-lfs"
    exit 127
fi

mkdir -p "${OHOS_DIR}"
cd "${OHOS_DIR}"

repo_init_args=(
    init
    -u "${OHOS_MANIFEST_URL}"
    -b "${OHOS_MANIFEST_BRANCH}"
    --no-repo-verify
)

if [ "${OHOS_DEPTH}" != "0" ]; then
    repo_init_args+=(--depth="${OHOS_DEPTH}")
fi

if [ ! -d ".repo" ]; then
    echo "Initializing OpenHarmony repo workspace..."
    repo "${repo_init_args[@]}"
else
    echo "OpenHarmony repo workspace already initialized."
    echo "Refreshing manifest..."
    repo "${repo_init_args[@]}"
fi

echo
echo "Synchronizing OpenHarmony source..."
repo sync -c -j"${OHOS_JOBS}"

echo
echo "Pulling Git LFS assets..."
repo forall -c 'git lfs pull || true'

echo
echo "OpenHarmony source is ready:"
echo "  ${OHOS_DIR}"
echo
echo "LiteOS-M kernel path:"
echo "  ${OHOS_DIR}/kernel/liteos_m"
