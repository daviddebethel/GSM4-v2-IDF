# Repo Audit

**Date/Time:** 2026-02-15, 20:01 GMT  
**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`

---

## Summary

**Overall result:** ⏳ **CONDITIONAL PASS** (1 critical blocker, 2 high-priority items)

**Failure categories:**
- Missing required paths: **1** (.github/workflows/)
- Broken documentation references: **3 file types** (docs/modules → docs/contracts/v1, docs/refactor_analysis/agents → docs/analysis)
- Empty skeleton folders: **2** (firmware/, web/)
- Mismatch between docs and actual structure

**Blocking issues for rewrite:**
- ✅ Core module contracts exist (docs/contracts/v1/)
- ✅ Analysis artefacts **all present** (03_entry_points, 05_bus_access_union_fn_refined, 13_dependency_edges.csv, 13_system_recomposition.md, **A_calls_cluster_all_cpp.md**)
- ❌ **CRITICAL:** CI folder structure missing (.github/workflows/)—cannot start automated testing
- ⚠️ **HIGH:** Broken path references must be corrected (docs/modules → docs/contracts/v1, docs/refactor_analysis/agents → docs/analysis)
- ⚠️ **WARNING:** New engineers will encounter broken doc references if path corrections not applied

---

## Tree Snapshot

### Root level (top-level folders and files)

```
GSM4-v2-IDF-1/
├── .git/
├── README.md
├── GSM4-v2-IDF-1.code-workspace
├── ci/                          ← EMPTY (no workflows yet)
├── docs/
├── firmware/                    ← EMPTY (skeleton only)
├── tools/                       ← EMPTY
└── web/                         ← EMPTY (skeleton only)
```

### /docs structure (depth 1)

```
docs/
├── START_HERE.md               ✅ Present
├── analysis/                   ✅ Present (5/5 required files, 1 broken ref)
├── architecture/               ✅ Present (1 file)
├── contracts/                  ✅ Present (v1/ with 9 contract files)
├── interfaces/                 ✅ Present (1 file)
├── project/                    ✅ Present (4 files, 1 missing ref)
├── security/                   ⚠️ Present but EMPTY
└── tests/                      ✅ Present (2 files)
```

---

## Required Paths Check

| Path | Expected type | Status | Notes |
|------|---|---|---|
| README.md | file | ✅ PASS | Exists and references correct docs |
| docs/START_HERE.md | file | ✅ PASS | Exists and correctly references `A_calls_cluster_all_cpp.md` (now present) |
| docs/project/REWRITE_PROGRAMME.md | file | ✅ PASS | Exists, references `docs/modules/*` (should be `docs/contracts/v1/`) |
| docs/project/DECISIONS.md | file | ✅ PASS | Exists |
| docs/tests/ACCEPTANCE_INVARIANTS.md | file | ✅ PASS | Exists, references `docs/modules/*` (should be `docs/contracts/v1/`) |
| docs/tests/TRACEABILITY.md | file | ✅ PASS | Exists |
| docs/architecture/IDF_TARGET_ARCH.md | file | ✅ PASS | Exists, references both wrong paths: `docs/modules/*` and `docs/refactor_analysis/agents/*` |
| docs/interfaces/COMMANDS_EVENTS.md | file | ✅ PASS | Exists |
| docs/contracts/v1/ | directory | ✅ PASS | Exists with 9 contract files (timebase, serial_transport, modem_core, etc.) |
| docs/analysis/03_entry_points.md | file | ✅ PASS | Exists |
| docs/analysis/05_bus_access_union_fn_refined.md | file | ✅ PASS | Exists |
| docs/analysis/13_dependency_edges.csv | file | ✅ PASS | Exists |
| docs/analysis/13_system_recomposition.md | file | ✅ PASS | Exists |
| docs/analysis/A_calls_cluster_all_cpp.md | file | ✅ PASS | **FOUND** — call coupling data with 36 file-level relationships |
| firmware/ | directory | ⚠️ EXISTS (EMPTY) | Folder exists but contains no files |
| web/ | directory | ⚠️ EXISTS (EMPTY) | Folder exists but contains no files |
| .github/workflows/ | directory | ❌ FAIL | **DOES NOT EXIST** — required for CI/CD |

---

## Doc Reference Check

### Referrer: README.md

| Referenced path | Status | Notes |
|---|---|---|
| docs/START_HERE.md | ✅ PASS | Correct path, file exists |
| docs/contracts/v1/ | ✅ PASS | Correct path, directory exists |
| docs/tests/ACCEPTANCE_INVARIANTS.md | ✅ PASS | Correct path, file exists |
| docs/interfaces/COMMANDS_EVENTS.md | ✅ PASS | Correct path, file exists |
| docs/project/REWRITE_PROGRAMME.md | ✅ PASS | Correct path, file exists |
| docs/tests/TRACEABILITY.md | ✅ PASS | Correct path, file exists |
| docs/project/DECISIONS.md | ✅ PASS | Correct path, file exists |
| /.github/workflows/ | ❌ FAIL | Referenced but path does not exist |

### Referrer: docs/START_HERE.md

| Referenced path | Status | Notes |
|---|---|---|
| docs/project/REWRITE_PROGRAMME.md | ✅ PASS | Correct path, file exists |
| docs/tests/ACCEPTANCE_INVARIANTS.md | ✅ PASS | Correct path, file exists |
| docs/architecture/IDF_TARGET_ARCH.md | ✅ PASS | Correct path, file exists |
| docs/interfaces/COMMANDS_EVENTS.md | ✅ PASS | Correct path, file exists |
| docs/contracts/v1/ | ✅ PASS | Correct path, directory exists |
| docs/tests/TRACEABILITY.md | ✅ PASS | Correct path, file exists |
| docs/project/DECISIONS.md | ✅ PASS | Correct path, file exists |
| docs/analysis/03_entry_points.md | ✅ PASS | Correct path, file exists |
| docs/analysis/05_bus_access_union_fn_refined.md | ✅ PASS | Correct path, file exists |
| docs/analysis/13_dependency_edges.csv | ✅ PASS | Correct path, file exists |
| docs/analysis/13_system_recomposition.md | ✅ PASS | Correct path, file exists |
| docs/analysis/A_calls_cluster_all_cpp.md | ✅ PASS | **Correct path, file exists** — call coupling relationships documented |

### Referrer: docs/project/REWRITE_PROGRAMME.md

| Referenced path | Status | Notes |
|---|---|---|
| docs/tests/ACCEPTANCE_INVARIANTS.md | ✅ PASS | Correct path, file exists |
| docs/tests/TRACEABILITY.md | ✅ PASS | Correct path, file exists |
| docs/contracts/v1/ | ✅ PASS | Correct path, directory exists |
| docs/architecture/IDF_TARGET_ARCH.md | ✅ PASS | Correct path, file exists |
| docs/interfaces/COMMANDS_EVENTS.md | ✅ PASS | Correct path, file exists |
| docs/START_HERE.md | ✅ PASS | Correct path, file exists |
| docs/analysis/ | ✅ PASS | Correct path, directory exists |
| docs/tests/HIL_PLAN.md | ❌ FAIL | **Referenced as future deliverable but does not exist** |

### Referrer: docs/tests/ACCEPTANCE_INVARIANTS.md

| Referenced path | Status | Notes |
|---|---|---|
| docs/tests/ACCEPTANCE_INVARIANTS.md | ✅ PASS | Self-reference, file exists |
| docs/modules/timebase.md | ❌ FAIL | **Should be `docs/contracts/v1/timebase.md`** |
| docs/modules/serial_transport.md | ❌ FAIL | **Should be `docs/contracts/v1/serial_transport.md`** |
| docs/modules/modem_core.md | ❌ FAIL | **Should be `docs/contracts/v1/modem_core.md`** |
| docs/modules/wifi_web_control_plane.md | ❌ FAIL | **Should be `docs/contracts/v1/wifi_web_control_plane.md`** |
| docs/modules/config_store.md | ❌ FAIL | **Should be `docs/contracts/v1/config_store.md`** |
| docs/modules/audio_runtime.md | ❌ FAIL | **Should be `docs/contracts/v1/audio_runtime.md`** |
| docs/modules/power_and_physical_io.md | ❌ FAIL | **Should be `docs/contracts/v1/power_and_physical_io.md`** |
| docs/modules/sensor_and_visual_ui.md | ❌ FAIL | **Should be `docs/contracts/v1/sensor_and_visual_ui.md`** |
| docs/modules/in_call_dtmf_control.md | ❌ FAIL | **Should be `docs/contracts/v1/in_call_dtmf_control.md`** |

### Referrer: docs/architecture/IDF_TARGET_ARCH.md

| Referenced path | Status | Notes |
|---|---|---|
| docs/tests/ACCEPTANCE_INVARIANTS.md | ✅ PASS | Correct path, file exists |
| docs/modules/*.md (9 contracts) | ❌ FAIL | **Should all be `docs/contracts/v1/*.md`** |
| docs/refactor_analysis/agents/03_entry_points.md | ❌ FAIL | **Should be `docs/analysis/03_entry_points.md`** |
| docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md | ❌ FAIL | **Should be `docs/analysis/A_calls_cluster_all_cpp.md`** (which now exists at correct location) |
| docs/refactor_analysis/agents/05_bus_access_union_fn_refined.md | ❌ FAIL | **Should be `docs/analysis/05_bus_access_union_fn_refined.md`** |
| docs/analysis/13_dependency_edges.csv | ✅ PASS | Correct path, file exists |

### Referrer: docs/contracts/v1/*.md (module contracts)

Multiple contract files reference both wrong paths:

| Referenced wrong path | Should be | Files affected |
|---|---|---|
| docs/modules/modem_core.md | docs/contracts/v1/modem_core.md | modem_core.md, audio_runtime.md, serial_transport.md, others |
| docs/modules/serial_transport.md | docs/contracts/v1/serial_transport.md | serial_transport.md, modem_core.md |
| docs/modules/audio_runtime.md | docs/contracts/v1/audio_runtime.md | audio_runtime.md, timebase.md |
| docs/modules/timebase.md | docs/contracts/v1/timebase.md | timebase.md, power_and_physical_io.md, in_call_dtmf_control.md |
| docs/modules/wifi_web_control_plane.md | docs/contracts/v1/wifi_web_control_plane.md | wifi_web_control_plane.md, config_store.md |
| docs/modules/config_store.md | docs/contracts/v1/config_store.md | config_store.md, power_and_physical_io.md |
| docs/modules/power_and_physical_io.md | docs/contracts/v1/power_and_physical_io.md | power_and_physical_io.md, sensor_and_visual_ui.md |
| docs/modules/sensor_and_visual_ui.md | docs/contracts/v1/sensor_and_visual_ui.md | sensor_and_visual_ui.md |
| docs/modules/in_call_dtmf_control.md | docs/contracts/v1/in_call_dtmf_control.md | in_call_dtmf_control.md, serial_transport.md |
| docs/refactor_analysis/agents/03_entry_points.md | docs/analysis/03_entry_points.md | modem_core.md, serial_transport.md, timebase.md, wifi_web_control_plane.md, power_and_physical_io.md, sensor_and_visual_ui.md, in_call_dtmf_control.md |
| docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md | docs/analysis/A_calls_cluster_all_cpp.md | timebase.md, serial_transport.md, modem_core.md, in_call_dtmf_control.md |
| docs/refactor_analysis/agents/05_bus_access_union_fn_refined.md | docs/analysis/05_bus_access_union_fn_refined.md | modem_core.md, serial_transport.md, in_call_dtmf_control.md, wifi_web_control_plane.md |

---

## Findings

### Critical Issues (Blocking rewrite)

1. **Broken path references to module contracts:** All 9 module contracts at `docs/contracts/v1/*.md` are referenced as `docs/modules/*.md` in multiple documentation files
   - **Impact:** HIGH — New engineers will search for wrong paths; build tooling will fail to find references
   - **Affected files:** README.md, REWRITE_PROGRAMME.md, ACCEPTANCE_INVARIANTS.md, IDF_TARGET_ARCH.md, all contract files themselves
   - **Scope:** 9 contracts × ~15 bad references = ~135 instances

2. **Broken path references to analysis files:** References to `docs/refactor_analysis/agents/*` should be `docs/analysis/*`
   - **Impact:** MEDIUM — Build tooling and doc validators will fail; cross-references break
   - **Affected files:** IDF_TARGET_ARCH.md, contract files (modem_core, serial_transport, timebase, etc.)
   - **Scope:** ~20+ bad references across 8+ files

3. **Missing CI infrastructure:** `.github/workflows/` directory does not exist
   - **Impact:** CRITICAL — Cannot start automated testing per R3 of REWRITE_PROGRAMME.md
   - **Constraint:** Blocks Phase 1 exit criteria ("CI passes")
   - **Remedy:** Must be created before any PR automation is possible

### High-Priority Issues

4. **Empty skeleton folders:** firmware/, web/, tools/, ci/, docs/security/ exist but are empty
   - **Impact:** MEDIUM — Repo skeleton is incomplete; rewrite cannot proceed without implementation
   - **Status:** Expected (Phase 1 task R1), but docs claim skeleton is "in place"
   - **Note:** This is acceptable if R1 is not yet started; otherwise indicates incomplete work

### Warnings

5. **Missing future deliverable:** `docs/tests/HIL_PLAN.md` is referenced in REWRITE_PROGRAMME.md (R5) but does not exist
   - **Impact:** LOW — This is a Phase 5 (later) deliverable, not blocking Phase 1
   - **Note:** Acceptable to be missing during rewrite startup

6. **Empty docs/security/ folder:** Exists but contains no files
   - **Impact:** LOW — No security docs yet; acceptable for Phase 1
   - **Note:** Check REWRITE_PROGRAMME.md Phase 2 for security hooks requirements

---

## Fix Plan

### Fix Category 1: Path Reference Corrections (REQUIRED BEFORE MERGE)

**Priority:** CRITICAL (affects all downstream tooling and new engineer onboarding)

#### Fix 1.1: Replace `docs/modules/` with `docs/contracts/v1/` throughout codebase

**Scope:** Replace 9 module names in 8+ files

**Files to edit:**
- docs/tests/ACCEPTANCE_INVARIANTS.md
- docs/project/REWRITE_PROGRAMME.md
- docs/architecture/IDF_TARGET_ARCH.md
- docs/contracts/v1/modem_core.md
- docs/contracts/v1/serial_transport.md
- docs/contracts/v1/timebase.md
- docs/contracts/v1/audio_runtime.md
- docs/contracts/v1/wifi_web_control_plane.md
- docs/contracts/v1/config_store.md
- docs/contracts/v1/power_and_physical_io.md
- docs/contracts/v1/sensor_and_visual_ui.md
- docs/contracts/v1/in_call_dtmf_control.md

**Shell commands (do not execute):**

```bash
# ACCEPT_INVARIANTS
sed -i 's|docs/modules/timebase.md|docs/contracts/v1/timebase.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/serial_transport.md|docs/contracts/v1/serial_transport.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/modem_core.md|docs/contracts/v1/modem_core.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/wifi_web_control_plane.md|docs/contracts/v1/wifi_web_control_plane.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/config_store.md|docs/contracts/v1/config_store.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/audio_runtime.md|docs/contracts/v1/audio_runtime.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/power_and_physical_io.md|docs/contracts/v1/power_and_physical_io.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/sensor_and_visual_ui.md|docs/contracts/v1/sensor_and_visual_ui.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md
sed -i 's|docs/modules/in_call_dtmf_control.md|docs/contracts/v1/in_call_dtmf_control.md|g' docs/tests/ACCEPTANCE_INVARIANTS.md

# REWRITE_PROGRAMME
sed -i 's|docs/modules/|docs/contracts/v1/|g' docs/project/REWRITE_PROGRAMME.md

# IDF_TARGET_ARCH
sed -i 's|docs/modules/|docs/contracts/v1/|g' docs/architecture/IDF_TARGET_ARCH.md

# All contract files
for file in docs/contracts/v1/*.md; do
  sed -i 's|docs/modules/|docs/contracts/v1/|g' "$file"
done
```

#### Fix 1.2: Replace `docs/refactor_analysis/agents/` with `docs/analysis/` throughout codebase

**Scope:** Replace 3 analysis file paths in 8+ files

**Files to edit:**
- docs/architecture/IDF_TARGET_ARCH.md
- docs/contracts/v1/modem_core.md
- docs/contracts/v1/serial_transport.md
- docs/contracts/v1/timebase.md
- docs/contracts/v1/audio_runtime.md
- docs/contracts/v1/wifi_web_control_plane.md
- docs/contracts/v1/power_and_physical_io.md
- docs/contracts/v1/sensor_and_visual_ui.md
- docs/contracts/v1/in_call_dtmf_control.md

**Shell commands (do not execute):**

```bash
# Replace all instances
find docs -name "*.md" -type f -exec sed -i 's|docs/refactor_analysis/agents/03_entry_points.md|docs/analysis/03_entry_points.md|g' {} \;
find docs -name "*.md" -type f -exec sed -i 's|docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md|docs/analysis/A_calls_cluster_all_cpp.md|g' {} \;
find docs -name "*.md" -type f -exec sed -i 's|docs/refactor_analysis/agents/05_bus_access_union_fn_refined.md|docs/analysis/05_bus_access_union_fn_refined.md|g' {} \;
```

---

### Fix Category 2: Missing CI Infrastructure (REQUIRED FOR R3)

**Priority:** CRITICAL (blocks Phase 1 exit criteria)

#### Fix 3.1: Create `.github/workflows/` structure

**Scope:** Create CI pipeline directory and initial workflow files

**Files to create:**
- `.github/workflows/ci.yml` (main CI pipeline)

**Minimal `.github/workflows/ci.yml` template (pending actual IDF build config):**

```yaml
name: Firmware Build and Test

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build-firmware:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Set up ESP-IDF build environment
        run: |
          # TODO: Install ESP-IDF and toolchain
          echo "IDF environment setup placeholder"
      - name: Build firmware
        run: |
          # TODO: Run IDF build
          echo "firmware build placeholder"

  unit-tests:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Run unit tests
        run: |
          # TODO: Run Unity test runner
          echo "unit tests placeholder"

  mock-tests:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Run mock/host tests
        run: |
          # TODO: Run mock tests
          echo "mock tests placeholder"
```

**Shell command to create directory:**

```bash
mkdir -p .github/workflows
```

---

### Fix Category 3: Skeleton Completion (DEFERRED, tracked in R1)

**Priority:** LOW (expected as part of R1 – "Implement the rewrite in the IDF native framework")

**Status:** Acceptable to be empty at repo initialization; these folders should be populated during Phase 1 implementation.

**Note:** Once firmware and web implementations begin, these folders will be populated. Current empty state is consistent with R1 "Not started" status.

---

### Summary of Fixes

| Fix ID | Issue | Severity | Lines affected | Estimated effort |
|---|---|---|---|---|
| 1.1 | docs/modules/ → docs/contracts/v1/ | CRITICAL | ~135 | 10 min (sed) |
| 1.2 | docs/refactor_analysis/agents/ → docs/analysis/ | CRITICAL | ~20 | 5 min (find + sed) |
| 2.1 | Create .github/workflows/ | CRITICAL | 1–3 new files | 30 min (placeholder, pending IDF details) |
| 3.x | Populate firmware/, web/, tools/, ci/ | LOW | N/A | Tracked in R1 |

---

## Recommendations

1. **Immediate action (Pre-merge):**
   - Apply Fix 1.1 and 1.2 (path corrections) to all referencing files
   - Create `.github/workflows/ci.yml` structure with placeholder steps

2. **Next milestone (Phase 1):**
   - Populate `/firmware` with ESP-IDF project skeleton per R1
   - Populate `/web` with React/TypeScript UI stub per R1
   - Finalize CI pipeline steps in `.github/workflows/ci.yml` with actual IDF toolchain commands

3. **Documentation discipline:**
   - Add a pre-commit hook or CI lint step to verify all `docs/**/` paths exist before merge
   - Use a link checker tool (e.g., markdown-link-check) in CI to catch future broken references

4. **Onboarding:**
   - After fixes, update README.md to clarify that docs/START_HERE.md is the single entry point
   - Add a "Common paths" quick reference to START_HERE.md for new engineers

---

**Audit completed:** 2026-02-15, 20:01 GMT
