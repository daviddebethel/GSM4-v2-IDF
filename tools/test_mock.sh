#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Foundation behavior:
# - Build with the ci-test profile (mock tests execute in this profile).
# - Emit a JUnit artifact so CI/QASE plumbing is wired before mock suites are added.
"${REPO_ROOT}/tools/build_firmware.sh" --profile ci-test reconfigure build

ARTIFACTS_DIR="${ARTIFACTS_DIR:-${REPO_ROOT}/artifacts}"
mkdir -p "${ARTIFACTS_DIR}"
JUNIT_FILE="${ARTIFACTS_DIR}/mock-tests.junit.xml"

cat > "${JUNIT_FILE}" <<'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<testsuites tests="1" failures="0" errors="0" skipped="0">
  <testsuite name="mock-tests" tests="1" failures="0" errors="0" skipped="0">
    <testcase classname="foundation" name="mock_tests_foundation_profile_ci_test"/>
  </testsuite>
</testsuites>
EOF

echo "JUnit artifact written to ${JUNIT_FILE}"
