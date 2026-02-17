#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

assert_has() {
    local pattern="$1"
    local file="$2"
    if ! rg -q "^${pattern}$" "${file}"; then
        echo "ERROR: Missing required setting '${pattern}' in ${file}" >&2
        exit 1
    fi
}

assert_absent() {
    local pattern="$1"
    local file="$2"
    if rg -q "^${pattern}$" "${file}"; then
        echo "ERROR: Forbidden release setting '${pattern}' found in ${file}" >&2
        exit 1
    fi
}

release_sdkconfig="${REPO_ROOT}/firmware/build/release/sdkconfig"

if [ ! -f "${release_sdkconfig}" ]; then
    echo "Release sdkconfig not found, generating it first..."
    "${REPO_ROOT}/tools/build_firmware.sh" --profile release reconfigure
fi

assert_has "CONFIG_GSM4_BUILD_PROFILE_RELEASE=y" "${release_sdkconfig}"
assert_has "CONFIG_GSM4_RELEASE_SAFE_DEFAULTS=y" "${release_sdkconfig}"
assert_has "CONFIG_GSM4_SERIAL_DEBUG_LOCKDOWN=y" "${release_sdkconfig}"
assert_has "CONFIG_GSM4_REQUIRE_RELEASE_SECURITY_CONTROLS=y" "${release_sdkconfig}"

assert_absent "CONFIG_GSM4_TEST_HARNESS_MODE=y" "${release_sdkconfig}"
assert_absent "CONFIG_GSM4_SERIAL_DEBUG_ENABLED=y" "${release_sdkconfig}"
assert_absent "CONFIG_GSM4_RUNTIME_DEBUG_WEB_DEFAULT_ON=y" "${release_sdkconfig}"

echo "Release policy checks passed for ${release_sdkconfig}"
