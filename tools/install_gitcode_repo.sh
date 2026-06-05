#!/usr/bin/env bash
set -euo pipefail

REPO_BIN_DIR="${REPO_BIN_DIR:-${HOME}/bin}"
REPO_BIN="${REPO_BIN_DIR}/repo"
REPO_URL="${REPO_URL:-https://raw.gitcode.com/gitcode-dev/repo/raw/main/repo-py3}"

mkdir -p "${REPO_BIN_DIR}"

echo "Installing GitCode repo launcher:"
echo "  ${REPO_BIN}"
echo

curl "${REPO_URL}" -o "${REPO_BIN}"
chmod a+x "${REPO_BIN}"

if command -v pip3 >/dev/null 2>&1; then
    pip3 install -i https://repo.huaweicloud.com/repository/pypi/simple requests
else
    echo "pip3 was not found. Install python3-pip if repo later reports missing requests."
fi

case ":${PATH}:" in
    *":${REPO_BIN_DIR}:"*) ;;
    *)
        echo
        echo "Add repo to PATH:"
        echo "  export PATH=${REPO_BIN_DIR}:\$PATH"
        ;;
esac

echo
"${REPO_BIN}" --version || true
