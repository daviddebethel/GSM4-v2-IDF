#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Secure smoke foundation:
# - Build with the ci-secure profile.
"${REPO_ROOT}/tools/build_firmware.sh" --profile ci-secure reconfigure build
