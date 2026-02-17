# ESP-IDF setup guide for GSM4-v2-IDF (ESP32-WROOM-32)

Date: 2026-02-17

This guide assumes:
- You are developing on **macOS**.
- Target hardware is **ESP32-WROOM-32**.
- Repository: `https://github.com/daviddebethel/GSM4-v2-IDF.git`
- You want to **pin ESP-IDF v5.5.2** in-repo for reproducible builds. citeturn0search1
- You are using **VS Code** with the official **ESP-IDF Extension**, plus GPT Codex. citeturn0search0turn0search14

> Why v5.5.2: The official releases page identifies `v5.5.2` as the latest stable release (at time of writing), and Espressif’s version guidance is to use stable releases for new projects. citeturn0search1turn0search5

---

## 1) Get the ESP-IDF files into the repo (pinned)

### 1.1 Choose the pinning method (recommended: git submodule)

**Recommended**: add ESP-IDF as a git submodule under `third_party/esp-idf` pinned to tag `v5.5.2`.

Pros:
- Deterministic, CI friendly, Codex can read the same headers and build system.
- Easy upgrades: update one pointer later.

Cons:
- Submodule workflow adds one extra clone step.

### 1.2 Add ESP-IDF as a submodule

From the repo root:

```sh
mkdir -p third_party

git submodule add https://github.com/espressif/esp-idf.git third_party/esp-idf
cd third_party/esp-idf
git checkout v5.5.2
cd ../..

git add .gitmodules third_party/esp-idf
git commit -m "Pin ESP-IDF as submodule at v5.5.2"
```

Then initialise all nested submodules (ESP-IDF uses submodules):

```sh
git submodule update --init --recursive
```

### 1.3 Record the pin in docs (so it never drifts)

Create `firmware/IDF_VERSION`:

```text
v5.5.2
```

Add an ADR entry in `docs/project/DECISIONS.md`:
- ESP-IDF pinned to `v5.5.2`
- Upgrade policy: bugfix updates only during Phase 1 parity, planned minor upgrades at phase boundaries

---

## 2) Set up the repo folder structure for development

Your documentation already assumes a monorepo layout. Make it real now.

### 2.1 Create the skeleton

From repo root:

```sh
mkdir -p firmware/main
mkdir -p web
mkdir -p tools
mkdir -p docs/behaviour
mkdir -p docs/contracts/v1
mkdir -p docs/analysis
mkdir -p .github/workflows
```

Add placeholders so Git tracks folders:

```sh
touch firmware/.gitkeep web/.gitkeep tools/.gitkeep
```

### 2.2 Add a minimal ESP-IDF project under `/firmware`

Create these files:

**`firmware/CMakeLists.txt`**
```cmake
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(gsm4_v2_idf)
```

**`firmware/main/CMakeLists.txt`**
```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
)
```

**`firmware/main/main.c`**
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    while (1) {
        printf("gsm4-v2-idf bringup\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

Commit the skeleton:

```sh
git add firmware
git commit -m "Add minimal ESP-IDF firmware skeleton"
```

### 2.3 Add Codex friendly “context anchors” in the repo

To keep Codex grounded, store your analysis artefacts under stable paths (if not already):

- `docs/analysis/03_entry_points.md`
- `docs/analysis/A_calls_cluster_all_cpp.md`
- `docs/analysis/05_bus_access_union_fn_refined.md`
- `docs/analysis/07_interaction_scores_v2_fanout.csv`
- `docs/analysis/08_interaction_clusters_v2_fanout.md`

Add a short `docs/analysis/README.md` explaining:
- what each file is
- which are canonical inputs for refactor prompts

---

## 3) Install dependencies for ESP-IDF on macOS (and for better Codex workflows)

There are two layers:
- **ESP-IDF tools** (toolchains, Python env, CMake/Ninja)
- **Developer QoL tools** (ripgrep, clang-format, pre-commit) that make changes safer and help Codex produce consistent output

### 3.1 Install OS level prerequisites (recommended baseline)

Using Homebrew:

```sh
brew update
brew install git cmake ninja python3
brew install ccache
brew install ripgrep
brew install clang-format
brew install pre-commit
```

Notes:
- `ripgrep` is used heavily in the analysis workflow (entry points, call wiring, etc).
- `clang-format` + `pre-commit` keeps diffs clean and reduces “AI churn”.

### 3.2 Install ESP-IDF tools for the pinned version (from the submodule)

ESP-IDF includes scripts to install its required tools and set up environment variables.

From repo root:

```sh
cd third_party/esp-idf
./install.sh
. ./export.sh
```

Espressif’s version guidance is to use stable releases and follow official installation workflows. citeturn0search5

#### Make it repeatable: add repo wrapper scripts

Create `tools/idf_env.sh`:

```sh
#!/usr/bin/env bash
# Source-safe helper: works from bash and zsh without changing shell options.
_idf_env_die() {
    echo "ERROR: $*" >&2
    return 1 2>/dev/null || exit 1
}

_idf_env_repo_root() {
    local root=""
    if command -v git >/dev/null 2>&1; then
        root="$(git rev-parse --show-toplevel 2>/dev/null || true)"
    fi
    if [ -z "${root}" ] && [ -n "${BASH_SOURCE[0]-}" ]; then
        root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." >/dev/null 2>&1 && pwd || true)"
    fi
    if [ -z "${root}" ] && [ -n "${ZSH_VERSION-}" ]; then
        zsh_path="$(eval 'printf "%s" "${(%):-%x}"' 2>/dev/null || true)"
        [ -n "${zsh_path}" ] && root="$(cd "$(dirname "${zsh_path}")/.." >/dev/null 2>&1 && pwd || true)"
    fi
    [ -n "${root}" ] || return 1
    printf '%s\n' "${root}"
}

REPO_ROOT="$(_idf_env_repo_root)" || _idf_env_die "Unable to determine repository root."
IDF="${REPO_ROOT}/third_party/esp-idf"
[ -f "${IDF}/export.sh" ] || _idf_env_die "Missing ${IDF}/export.sh"
. "${IDF}/export.sh"
```

Create `tools/build_firmware.sh` (profile-aware):

```sh
#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
. "${REPO_ROOT}/tools/idf_env.sh"
cd "${REPO_ROOT}/firmware"
profile="dev"
if [ "${1-}" = "--profile" ]; then
  profile="$2"
  shift 2
fi
export SDKCONFIG_DEFAULTS="${REPO_ROOT}/firmware/sdkconfig.defaults;${REPO_ROOT}/firmware/sdkconfig.defaults.${profile}"
if [ $# -eq 0 ]; then
  set -- build
fi
idf.py -B "build/${profile}" -DSDKCONFIG="build/${profile}/sdkconfig" "$@"
```

Use the wrappers from any shell:

```sh
source tools/idf_env.sh
tools/build_firmware.sh --profile dev build
tools/build_firmware.sh --profile release reconfigure build
tools/check_release_policy.sh
```

Make them executable and commit:

```sh
chmod +x tools/*.sh
git add tools
git commit -m "Add ESP-IDF environment and build wrapper scripts"
```

### 3.3 VS Code setup using the official ESP-IDF Extension

Install the **ESP-IDF** extension (`espressif.esp-idf-extension`) and run its configuration wizard. citeturn0search14turn0search0

Recommended approach with a pinned in-repo IDF:
- Use the extension for build, flash, monitor UX.
- Configure the extension to use the **existing ESP-IDF directory** at `third_party/esp-idf` (rather than downloading a separate copy).

Practical steps:
1. VS Code → Extensions → install “ESP-IDF”.
2. Command Palette → `ESP-IDF: Configure ESP-IDF extension` (setup wizard). citeturn0search0
3. Point **IDF_PATH** to: `<repo>/third_party/esp-idf`
4. Let it detect or install required tools (or keep using the tools installed by `install.sh` above).
5. Commit workspace defaults so new contributors inherit the same in-repo IDF path:

```json
{
  "folders": [{ "path": "." }],
  "settings": {
    "idf.espIdfPath": "${workspaceFolder}/third_party/esp-idf",
    "idf.currentSetup": "${workspaceFolder}/third_party/esp-idf",
    "idf.toolsPath": "${env:HOME}/.espressif",
    "idf.customExtraVars": {
      "IDF_TOOLS_PATH": "${env:HOME}/.espressif"
    }
  }
}
```

If contributors open the repository as a plain folder (not the `.code-workspace` file), keep the same settings in `.vscode/settings.json`:

```json
{
  "idf.espIdfPath": "${workspaceFolder}/third_party/esp-idf",
  "idf.currentSetup": "${workspaceFolder}/third_party/esp-idf",
  "idf.toolsPath": "${env:HOME}/.espressif",
  "idf.customExtraVars": {
    "IDF_TOOLS_PATH": "${env:HOME}/.espressif"
  }
}
```

6. Register this checkout in the ESP-IDF extension catalog used by extension v2:

```sh
./tools/register_idf_for_vscode.sh
```

Then in VS Code:
1) `ESP-IDF: Select Current ESP-IDF Version`
2) Select this repo's `third_party/esp-idf` entry
3) `Developer: Reload Window`

#### 3.3.1 Common activation failures (and fixes now applied)

1) Symptom:
`tools/idf_env.sh: BASH_SOURCE[0]: parameter not set`
- Cause: Bash-only path resolution sourced from `zsh`.
- Fix: use the updated `tools/idf_env.sh` in this repo (source-safe for `bash` and `zsh`).

2) Symptom:
Shell starts failing unexpectedly after sourcing the env script (for example `set -u` behavior in unrelated commands).
- Cause: `set -euo pipefail` was being applied by a sourced helper script.
- Fix: the updated `tools/idf_env.sh` no longer mutates caller shell options.

3) Symptom:
ESP-IDF extension opens but this workspace does not point at the in-repo IDF.
- Cause: workspace settings file had no ESP-IDF keys.
- Fix: workspace now includes `idf.espIdfPath` and `idf.toolsPath` defaults.

4) Symptom:
`ESP-IDF: Explorer` appears, but command entries are missing/empty.
- Cause: ESP-IDF extension v2 did not find a registered setup in its install catalog (`esp_idf.json` / `eim_idf.json`).
- Fix: run `./tools/register_idf_for_vscode.sh`, then select the current ESP-IDF version in VS Code and reload the window.

### 3.4 Make Codex outputs more reliable (recommended repo hygiene)

Add a `.editorconfig` at repo root:

```ini
root = true

[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
indent_style = space
indent_size = 4

[*.md]
trim_trailing_whitespace = false
```

Add a minimal `.pre-commit-config.yaml` (formatting and whitespace only to start):

```yaml
repos:
  - repo: https://github.com/pre-commit/pre-commit-hooks
    rev: v4.6.0
    hooks:
      - id: end-of-file-fixer
      - id: trailing-whitespace
      - id: check-yaml
```

Enable it:

```sh
pre-commit install
git add .editorconfig .pre-commit-config.yaml
git commit -m "Add editorconfig and pre-commit baseline"
```

Why this matters:
- Keeps Markdown and code files consistent.
- Minimises noisy diffs, which helps Codex focus on real changes.

---

## 4) Additional setup decisions (decision tracker)

This section tracks the remaining setup decisions and turns them into concrete follow-on files.

### 4.1 CI pinning and reproducibility — decided on 2026-02-17

Decision:
- Use **GitHub Actions only** for Phase 1 parity CI.
- Use repository checkout with pinned `third_party/esp-idf` submodule in CI.
- Do **not** adopt a pinned Docker image in Phase 1.
- Adopt build-profile contract **Option 2**: `dev`, `ci-test`, `ci-secure`, `release`.
- `ci-secure` must build with release-equivalent security settings (secure boot + flash encryption + NVS encryption) while `ci-test` remains the fast test profile.

Rationale:
- Small contributor set and pinned IDF keep short-term drift risk low.
- This keeps CI onboarding and maintenance overhead minimal during parity work.

Revisit triggers:
- Repeated local-vs-CI mismatches attributable to environment drift.
- Need for stricter hermetic/reproducible builds.

### 4.1.1 CI required checks and merge gate policy — decided on 2026-02-17

Decision:
- Adopt **Option 2 (Balanced Gate)** as mandatory PR merge policy.
- Required PR checks:
  - `build-firmware`
  - `unit-tests`
  - `mock-tests`
  - `lint-format`
  - `ci-secure`
  - `release-policy-check` (fails on development test/debug flag leakage in release profile artifacts)
- Branch protection policy:
  - Require at least 1 approval.
  - Require all conversations resolved.
  - Require branch to be up to date before merge.

Implementation notes:
- Apply required checks to protected branches used for integration/release.
- Keep longer-running integration and future HiL suites as non-blocking/nightly until explicitly promoted.
- Keep CI job names stable to avoid branch-protection drift.

Testing impact:
- CI must publish unit/mock JUnit artifacts and security/profile validation evidence for required checks.

### 4.2 Debugging — decided on 2026-02-17

Decision:
- Use **serial-only** programming and debugging for initial bring-up and Phase 1 parity.
- Do **not** require JTAG hardware in baseline setup.
- Require serial debug output to be controlled by build profile flags (enabled in dev/CI-test profiles, restricted/disabled in release profile unless explicitly approved).

Default workflow:

```sh
source tools/idf_env.sh
cd firmware
idf.py -p /dev/cu.usbserial-XXXX flash monitor
```

Follow-up:
- Reconsider JTAG later if deep timing/concurrency issues require hardware breakpoints or watchpoints.

### 4.3 Web build and packaging strategy — decided on 2026-02-17

Decision:
- Package web build output as a **flash filesystem image** in a dedicated data partition.
- Serve assets from the mounted filesystem via `esp_http_server`.
- Target hardware baseline is **ESP32-WROOM-32E (16MB flash)** with a **scalable custom partition layout** (start with an 8MB allocation profile now, while keeping layout/contracts scalable to full 16MB usage).
- Use **FATFS + wear levelling + VFS** for web/data filesystem storage.
- Security requirement: use **NVS encryption**, and require **secure boot + flash encryption + NVS encryption** in the release profile.

Implementation notes:
- Add/confirm a web assets FATFS partition in the custom partition table.
- Generate the filesystem image from the web build output as part of the firmware build/release flow.
- Keep UI assets versioned and traceable to firmware releases.
- OTA A/B rollback applies to app partitions; web/data partition is single-copy by default, so add app-assets compatibility/version checks.
- Keep build-profile separation explicit so development convenience does not relax release security settings.
- Foundation scaffolding implemented:
  - custom partition table at `firmware/partitions_16mb.csv` (OTA A/B + `webfs` FATFS partition),
  - profile defaults point to 16MB/custom partition in `firmware/sdkconfig.defaults`,
  - FATFS image generation integrated in `firmware/main/CMakeLists.txt` via `fatfs_create_spiflash_image(webfs ...)`,
  - source directory is `web/dist` with fallback scaffold content in `firmware/web_fallback`.

Testing impact:
- Add FATFS + wear levelling mount/read/write tests and recovery-path tests.
- Add OTA A/B tests validating app update behavior with single-copy data partition compatibility checks.
- Add release-profile validation tests for secure boot, flash encryption, and NVS encryption enablement.
- Add CI checks that release artifacts are produced with required security flags and without development debug/test leakage.

### 4.4 Testing baseline (unit + mocks) — decided on 2026-02-17

Decision:
- Use **ESP-IDF Unity on-target tests** as the baseline test runner.
- Implement **mock-based unit tests** as the unit-testing layer for logic modules.
- Treat **HiL** testing as a planned next layer, not a current delivery gate.

Implementation guidance:
- Define hardware seams (HAL/interfaces) so modules can be tested with host-side mocks.
- Add notes in setup and phase plans to reassess HiL automation at each development stage.

### 4.4.1 Mock test stack and layout contract — decided on 2026-02-17

Decision:
- Use **Unity** for on-target unit tests and **CMock** (via ESP-IDF `idf_component_mock`) for mock-based tests.
- Do not add a second test framework in Phase 1.
- Apply Option 2 build-profile execution intent:
  - `dev`: targeted local unit/mock runs.
  - `ci-test`: deterministic unit+mock test gate.
  - `ci-secure`: release-equivalent secure-profile validation gate.
  - `release`: no test/mock harness enabled by default.
- Use test directory layout contract:
  - `firmware/components/<module>/include`
  - `firmware/components/<module>/src`
  - `firmware/components/<module>/test`
  - `firmware/components/<module>/test/mocks`
  - `firmware/tests/common`
- Use naming contract:
  - Test source files: `test_<module>_<feature>.c`
  - Test case names: `test_<module>__<condition>__<expected>`
  - Mock config files: `mock_<module>.yaml`
  - Script entrypoints: `tools/test_unit.sh`, `tools/test_mock.sh`, `tools/test_secure_smoke.sh`
- Mock boundary contract: mock hardware/OS boundary dependencies (GPIO, UART, I2C, ADC, time, storage, modem transport edges) while keeping core logic under test real.

Implementation notes:
- Ensure local and CI both execute through the same test scripts to prevent workflow drift.
- Produce stable JUnit artifacts under `artifacts/` for unit and mock jobs and keep traceability mappings aligned.
- Require per-change unit+mock coverage updates (or waiver metadata) per `docs/CODEX_RULES.md`.

### 4.5 Codex guardrails — decided on 2026-02-17

Decision:
- Add `docs/CODEX_RULES.md` as the active Codex policy file.
- Enforce behavior-preserving migration rules (hard invariants vs migration-allowed internals).
- Enforce mandatory unit + mock testing for new/changed behavior and HiL-ready abstraction seams.
- Enforce coding standards, lint/format consistency, and layout discipline for touched code.

Implementation notes:
- `docs/CODEX_RULES.md` now defines mandatory execution, testing, security, and traceability rules for Codex changes.
- Any temporary gap in tests requires explicit waiver metadata (reason, owner, due date, tracking item).

### 4.6 Release log and debug channel policy — decided on 2026-02-17

Decision:
- Factory-programmed `release` devices must have serial debug locked down (no operational debug console/stream over serial).
- Runtime debug channels are default-off in the `release` profile.
- Runtime debug enablement must be policy-gated through authenticated web server controls (role-gated + feature-flagged), not open by default.
- Debug output controls must be runtime-switchable by functional domain/category with safe defaults set to off.
- Web debug UX must support terminal-style viewing and local capture/export.
- Optional secure upstream log streaming (for example to an AWS endpoint) is allowed only as a feature-flagged capability and remains default-off in `release`.
- All debug paths must enforce no-secret logging/redaction policy.

Implementation notes:
- This is a policy-level decision; implementation wiring remains a scaffolding task.
- `release-policy-check` must fail artifacts that leak development debug/test settings or violate release default-off requirements.
- Keep Phase 5 debug UX implementation aligned with auth, rate-limit, and non-blocking constraints.

Testing impact:
- Add CI checks proving `release` artifacts keep runtime debug channels default-off and serial debug locked down.
- Add integration tests for authenticated/role-gated web debug enablement and category-level filtering.
- Add negative tests for unauthenticated debug access and secret leakage in debug outputs.

### 4.7 Technical scaffolding plan — ready to execute

All Section 4 clarifications are now decided.

Implement technical scaffolding for 4.1-4.4 and 4.6 (including 4.4.1) in one pass:
- DONE: 4.1 GitHub Actions workflow scaffolding is in place with submodule checkout in `.github/workflows/ci.yml`.
- 4.2: add serial-first workflow scaffolding (documented commands/scripts for flash + monitor).
- DONE: 4.3 partition and filesystem-image scaffolding is in place (`firmware/partitions_16mb.csv`, profile defaults + `fatfs_create_spiflash_image(webfs ...)` integration).
- DONE: 4.4 initial CI test execution wiring exists via `unit-tests` and `mock-tests` jobs (foundation JUnit output + profile build validation).
- DONE: 4.4.1 script entrypoints are wired (`tools/test_unit.sh`, `tools/test_mock.sh`, `tools/test_secure_smoke.sh`) with local/CI parity.
- DONE: Option 2 build-profile foundation is in place (`dev`, `ci-test`, `ci-secure`, `release`) via `main/Kconfig.projbuild`, `firmware/sdkconfig.defaults*`, and profile-aware `tools/build_firmware.sh`.
- DONE: test harness default contract is wired (`ci-test` enabled by defaults; `ci-secure`/`release` disabled by defaults).
- DONE: serial debug flag gating foundation is wired (`dev`/`ci-test` enabled by defaults; `ci-secure`/`release` default-off) with baseline release validation in `tools/check_release_policy.sh`.
- DONE: release profile guard job is wired (`release-policy-check` runs `tools/check_release_policy.sh`).
- TODO: implement release debug policy scaffolding (serial locked down in `release`; web runtime debug controls role-gated/authenticated/feature-flagged; default-off release config; optional secure upstream streaming kept disabled unless explicitly enabled).
- TODO: add `lint-format` CI job and then enforce full Option 2 branch protection required checks (`build-firmware`, `unit-tests`, `mock-tests`, `lint-format`, `ci-secure`, `release-policy-check`) with stable job naming.
- TODO: replace foundation unit/mock JUnit placeholders with real Unity/CMock suite execution and artifacts.
