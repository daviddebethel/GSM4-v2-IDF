#!/usr/bin/env bash
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IDF="${REPO_ROOT}/third_party/esp-idf"
. "${IDF}/export.sh"