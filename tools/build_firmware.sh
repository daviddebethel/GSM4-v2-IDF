#!/usr/bin/env bash
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
. "${REPO_ROOT}/tools/idf_env.sh"
cd "${REPO_ROOT}/firmware"
idf.py build