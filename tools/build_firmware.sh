#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

usage() {
    cat <<'EOF'
Usage:
  tools/build_firmware.sh [--profile <dev|ci-test|ci-secure|release>] [idf.py args...]

Examples:
  tools/build_firmware.sh
  tools/build_firmware.sh --profile ci-test build
  tools/build_firmware.sh --profile release reconfigure build
EOF
}

profile="dev"
if [ "${1-}" = "--profile" ]; then
    if [ $# -lt 2 ]; then
        usage
        exit 1
    fi
    profile="$2"
    shift 2
elif [ "${1-}" = "-h" ] || [ "${1-}" = "--help" ]; then
    usage
    exit 0
fi

case "${profile}" in
    dev|ci-test|ci-secure|release) ;;
    *)
        echo "ERROR: Unknown profile '${profile}'" >&2
        usage
        exit 1
        ;;
esac

defaults_base="${REPO_ROOT}/firmware/sdkconfig.defaults"
defaults_profile="${REPO_ROOT}/firmware/sdkconfig.defaults.${profile}"
[ -f "${defaults_base}" ] || { echo "ERROR: Missing ${defaults_base}" >&2; exit 1; }
[ -f "${defaults_profile}" ] || { echo "ERROR: Missing ${defaults_profile}" >&2; exit 1; }

build_dir="build/${profile}"
sdkconfig_path="${build_dir}/sdkconfig"
export SDKCONFIG_DEFAULTS="${defaults_base};${defaults_profile}"

if [ $# -eq 0 ]; then
    set -- build
fi

echo "Using profile: ${profile}"
echo "SDKCONFIG_DEFAULTS=${SDKCONFIG_DEFAULTS}"
echo "Build directory: ${build_dir}"

. "${REPO_ROOT}/tools/idf_env.sh"
cd "${REPO_ROOT}/firmware"
idf.py -B "${build_dir}" -DSDKCONFIG="${sdkconfig_path}" "$@"
