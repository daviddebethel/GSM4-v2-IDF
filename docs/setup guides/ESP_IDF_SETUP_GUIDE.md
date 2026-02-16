# ESP-IDF setup guide for GSM4-v2-IDF (ESP32-WROOM-32)

Date: 2026-02-16

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
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IDF="${REPO_ROOT}/third_party/esp-idf"
. "${IDF}/export.sh"
```

Create `tools/build_firmware.sh`:

```sh
#!/usr/bin/env bash
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
. "${REPO_ROOT}/tools/idf_env.sh"
cd "${REPO_ROOT}/firmware"
idf.py build
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

## 4) Additional steps you should consider including (recommended)

These are worth including, but I need your decisions to make them concrete.

1) CI pinning and reproducibility
- Do you want CI to run on **GitHub Actions** only, or also a local runner pattern?
- Do you want CI to use the submodule checkout, or a pinned Docker image?

2) Debugging
- Will you use **JTAG** (ESP-Prog or similar), or is serial-only fine initially?

3) Web build and packaging strategy
You said you plan to use React + TypeScript and serve from the device.
- Should the web build output be embedded into flash as a filesystem image, or compiled into firmware as binary blobs?

4) Testing baseline (unit + mocks) for IDF
- Do you want to adopt IDF's built-in Unity test runner first, or start with host-based tests as well?

5) Codex guardrails
- Do you want a dedicated “Codex rules” file (for example `docs/CODEX_RULES.md`) to enforce contracts and invariants first?

If you answer those, we can extend this guide with the exact follow-on steps and ready-to-commit files (CI workflow, devcontainer if desired, test harness skeleton, and Codex rules).
