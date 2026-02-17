#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

"${REPO_ROOT}/tools/build_firmware.sh" --profile ci-test fullclean reconfigure build

ARTIFACTS_DIR="${ARTIFACTS_DIR:-${REPO_ROOT}/artifacts}"
mkdir -p "${ARTIFACTS_DIR}"
HOST_LOG_FILE="${ARTIFACTS_DIR}/unit-tests.host.log"
JUNIT_FILE="${ARTIFACTS_DIR}/unit-tests.junit.xml"

. "${REPO_ROOT}/tools/idf_env.sh"

UNIT_TEST_PROJECT_DIR="${REPO_ROOT}/firmware/tests/host/unit"
UNIT_TEST_BINARY="${UNIT_TEST_PROJECT_DIR}/build/gsm4_unit_host_test.elf"

cd "${UNIT_TEST_PROJECT_DIR}"
idf.py --preview set-target linux
idf.py build

set +e
"${UNIT_TEST_BINARY}" 2>&1 | tee "${HOST_LOG_FILE}"
test_exit="${PIPESTATUS[0]}"
set -e

if [ "${test_exit}" -eq 0 ]; then
    failures=0
    failure_block=""
else
    failures=1
    failure_block="<failure message=\"unit host test binary failed\">See ${HOST_LOG_FILE}</failure>"
fi

cat > "${JUNIT_FILE}" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<testsuites tests="1" failures="${failures}" errors="0" skipped="0">
  <testsuite name="unit-tests" tests="1" failures="${failures}" errors="0" skipped="0">
    <testcase classname="host.unity" name="gsm4_unit_host_test.elf">${failure_block}</testcase>
  </testsuite>
</testsuites>
EOF

echo "Unit host test log written to ${HOST_LOG_FILE}"
echo "JUnit artifact written to ${JUNIT_FILE}"

if [ "${test_exit}" -ne 0 ]; then
    exit "${test_exit}"
fi
