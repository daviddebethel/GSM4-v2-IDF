#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Foundation behavior:
# - Build with the ci-test profile.
# - Emit a JUnit artifact so CI/QASE plumbing is wired even before real tests land.
"${REPO_ROOT}/tools/build_firmware.sh" --profile ci-test reconfigure build

ARTIFACTS_DIR="${ARTIFACTS_DIR:-${REPO_ROOT}/artifacts}"
mkdir -p "${ARTIFACTS_DIR}"
JUNIT_FILE="${ARTIFACTS_DIR}/unit-tests.junit.xml"

cat > "${JUNIT_FILE}" <<'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<testsuites tests="1" failures="0" errors="0" skipped="0">
  <testsuite name="unit-tests" tests="1" failures="0" errors="0" skipped="0">
    <testcase classname="foundation" name="unit_tests_foundation_profile_ci_test"/>
  </testsuite>
</testsuites>
EOF

echo "JUnit artifact written to ${JUNIT_FILE}"
