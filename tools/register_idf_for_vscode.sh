#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
IDF_PATH="${REPO_ROOT}/third_party/esp-idf"

if [ ! -d "${IDF_PATH}" ]; then
    echo "ERROR: ESP-IDF not found at ${IDF_PATH}" >&2
    exit 1
fi

IDF_TOOLS_PATH="${IDF_TOOLS_PATH:-${HOME}/.espressif}"
ESP_IDF_JSON="${IDF_TOOLS_PATH}/esp_idf.json"
mkdir -p "${IDF_TOOLS_PATH}"

VERSION_RAW="$(git -C "${IDF_PATH}" describe --tags --exact-match 2>/dev/null || true)"
if [ -z "${VERSION_RAW}" ]; then
    VERSION_RAW="$(git -C "${IDF_PATH}" describe --tags --always 2>/dev/null || true)"
fi
if [ -z "${VERSION_RAW}" ]; then
    VERSION_RAW="vunknown"
fi
VERSION="${VERSION_RAW#v}"
MAJOR_MINOR="$(printf '%s' "${VERSION}" | awk -F. '{print $1 "." $2}')"

PYTHON_BIN="${IDF_PYTHON_BIN:-}"
if [ -z "${PYTHON_BIN}" ]; then
    if [ -n "${IDF_PYTHON_ENV_PATH:-}" ] && [ -x "${IDF_PYTHON_ENV_PATH}/bin/python" ]; then
        PYTHON_BIN="${IDF_PYTHON_ENV_PATH}/bin/python"
    elif [ -d "${IDF_TOOLS_PATH}/python_env" ]; then
        preferred=""
        while IFS= read -r -d '' candidate; do
            if [ -z "${preferred}" ] && [[ "${candidate}" == *"idf${MAJOR_MINOR}"*"/bin/"* ]]; then
                preferred="${candidate}"
            fi
            if [ -z "${PYTHON_BIN}" ]; then
                PYTHON_BIN="${candidate}"
            fi
        done < <(find "${IDF_TOOLS_PATH}/python_env" -maxdepth 5 -path "*/bin/*" \( -name "python" -o -name "python3" \) -print0 2>/dev/null | sort -z)
        if [ -n "${preferred}" ]; then
            PYTHON_BIN="${preferred}"
        fi
    fi
fi
if [ -z "${PYTHON_BIN}" ]; then
    PYTHON_BIN="$(command -v python3 || true)"
fi

if [ -z "${PYTHON_BIN}" ]; then
    echo "ERROR: Could not locate Python for ESP-IDF in ${IDF_TOOLS_PATH}/python_env or PATH." >&2
    exit 1
fi

GIT_BIN="$(command -v git || true)"
if [ -z "${GIT_BIN}" ]; then
    GIT_BIN="/usr/bin/git"
fi

SETUP_ID="${IDF_PATH}-v${VERSION}"

python3 - "${ESP_IDF_JSON}" "${IDF_TOOLS_PATH}" "${SETUP_ID}" "${IDF_PATH}" "${VERSION}" "${PYTHON_BIN}" "${GIT_BIN}" <<'PY'
import json
import os
import sys

json_path, tools_path, setup_id, idf_path, version, python_bin, git_bin = sys.argv[1:]

base = {
    "$schema": "http://json-schema.org/schema#",
    "$id": "http://dl.espressif.com/dl/schemas/esp_idf",
    "_comment": "Configuration file for ESP-IDF IDEs.",
    "_warning": "Use / or \\ when specifying path. Single backslash is not allowed by JSON format.",
    "gitPath": git_bin,
    "idfToolsPath": tools_path,
    "idfSelectedId": setup_id,
    "idfInstalled": {},
}

data = base
if os.path.exists(json_path):
    try:
        with open(json_path, "r", encoding="utf-8") as f:
            loaded = json.load(f)
        if isinstance(loaded, dict):
            data = loaded
    except Exception:
        data = base

data.setdefault("$schema", base["$schema"])
data.setdefault("$id", base["$id"])
data.setdefault("_comment", base["_comment"])
data.setdefault("_warning", base["_warning"])
data["gitPath"] = git_bin
data["idfToolsPath"] = tools_path
data["idfSelectedId"] = setup_id
installed = data.setdefault("idfInstalled", {})
if not isinstance(installed, dict):
    installed = {}
    data["idfInstalled"] = installed

installed[setup_id] = {
    "path": idf_path,
    "version": version,
    "python": python_bin,
}

with open(json_path, "w", encoding="utf-8") as f:
    json.dump(data, f, indent=2)
    f.write("\n")
PY

echo "Registered ESP-IDF setup for VS Code:"
echo "  esp_idf.json: ${ESP_IDF_JSON}"
echo "  setup id:     ${SETUP_ID}"
echo "  IDF_PATH:     ${IDF_PATH}"
echo "  version:      ${VERSION}"
echo "  python:       ${PYTHON_BIN}"
echo
echo "Next in VS Code:"
echo "  1) Command Palette -> ESP-IDF: Select Current ESP-IDF Version"
echo "  2) Pick: ${IDF_PATH}"
echo "  3) Developer: Reload Window"
