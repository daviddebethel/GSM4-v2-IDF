# GSM4 v2 IDF Rewrite Programme

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`

This document is the canonical programme board for the GSM4 v2 rewrite onto **ESP-IDF**.  
It exists to keep engineering, test, and documentation aligned, and to make it easy for GPT Codex to stay “on rails”.

---

## How to use this document

- Treat this file as the **single source of truth** for scope and sequencing.
- Each work item must link to:
  - A PR (or commit range)
  - Updated acceptance gates in `docs/tests/ACCEPTANCE_INVARIANTS.md`
  - Updated traceability in `docs/tests/TRACEABILITY.md`
  - QASE coverage where applicable
- A requirement is only “Done” when:
  - CI passes
  - Acceptance gates are met
  - QASE mapping is present for relevant tests
  - Documentation for the change is updated

---

## Phase model

### Phase 1 — Core functional parity (IDF rewrite baseline)
Goal: match legacy behaviour per module contracts and acceptance gates.

Exit criteria:
- All **Phase 1** acceptance gates pass (see `docs/tests/ACCEPTANCE_INVARIANTS.md`)
- `docs/architecture/IDF_TARGET_ARCH.md` implemented at least for baseline tasks and IPC scaffolding
- `/api/status` exists and reports core snapshot fields
- Unit and mock test suite runs in CI

### Phase 2 — Security hooks and hardening foundations
Goal: add secure boot / flash encryption hooks, key storage abstractions, and safe logging constraints.

Exit criteria:
- Partitions and config are reserved for later security features
- Secrets never appear in logs or web responses
- Key/cert handling is abstracted behind interfaces (no hard-coded paths)

### Phase 3 — OTA dual image support (A/B)
Goal: implement dual image OTA with rollback strategy and power-loss tolerance.

Exit criteria:
- OTA update flow implemented and tested
- Rollback and “known good” logic validated
- QASE suite includes OTA regression set

### Phase 4 — Cloud comms roadmap (TLS 1.3 capable, SNMPv3 capable)
Goal: design and prepare interfaces, implement when required.

Exit criteria:
- Transport and credential interfaces support TLS upgrades and SNMPv3 integration cleanly
- No debug interfaces bypass authentication or policy

### Phase 5 — Field debug UX
Goal: Bluetooth serial and web serial terminal page, heavily gated and rate-limited.

Exit criteria:
- Feature flags and role gating implemented
- Log capture is non-blocking and does not expose secrets

---

# Requirements 1–6

## R1. Implement the rewrite in the IDF native framework

**Definition:** A new ESP-IDF firmware in `/firmware` that preserves behaviour defined by v1 module contracts and acceptance gates.

### Deliverables
- [ ] `/firmware` ESP-IDF project builds locally (documented steps)
- [ ] Task model and IPC scaffolding created per `docs/architecture/IDF_TARGET_ARCH.md`
- [ ] IDF native web server (esp_http_server) running with:
  - [ ] static file serving
  - [ ] `/api/status` endpoint
- [ ] Module-by-module implementation plan (port order) defined and tracked

### Acceptance criteria
- [ ] Phase 1 global gates met
- [ ] Each ported module meets its module acceptance gates
- [ ] CI builds firmware in a clean environment

### Work breakdown (ordered)
1. **Repo skeleton**
   - [ ] Create `/firmware`, `/web`, `/docs`, `/tools`, `/ci` structure
2. **IDF baseline boot**
   - [ ] `app_main`: NVS init, filesystem mount (if used), WiFi init
3. **Core tasks scaffolding**
   - [ ] Create stubs for: modem_task, audio_task, io_task, ui_task (or loop runner), web_task, config_task (or config service)
4. **IPC primitives**
   - [ ] Command queue(s) defined
   - [ ] Event groups defined
   - [ ] State snapshot struct defined and served via `/api/status`
5. **Bring-up milestones**
   - [ ] “Hello” build + logs + status endpoint
   - [ ] First safe command endpoint (read-only)
   - [ ] First state-changing command (controlled)

**Owner:** Firmware Lead  
**Status:** Not started  
**Links:**  
- Contracts: `docs/contracts/v1/`  
- Architecture: `docs/architecture/IDF_TARGET_ARCH.md`  
- Interfaces: `docs/interfaces/COMMANDS_EVENTS.md`

---

## R2. Bring in relevant dependencies for unit and mock testing

**Definition:** A repeatable test setup that supports:
- unit tests (logic, state machines, parsers)
- mock tests (HAL boundary, deterministic simulation)
- later HIL integration without rewriting test harnesses

### Deliverables
- [ ] Unity test runner configured (IDF-native unit tests)
- [ ] HAL boundaries introduced for:
  - [ ] GPIO, ADC, I2C, UART, Time, Storage
- [ ] Mock implementations for HAL for test builds
- [ ] Host-based test option (optional but recommended) for fast logic tests

### Acceptance criteria
- [ ] Unit tests run locally in a single command
- [ ] Mock tests validate non-hardware logic deterministically
- [ ] Test runs produce machine-readable artefacts (JUnit XML preferred)

**Owner:** Firmware + QA Automation  
**Status:** Not started  
**Links:**  
- Acceptance gates: `docs/tests/ACCEPTANCE_INVARIANTS.md`

---

## R3. Set up CI/CD and GitHub to automatically run unit and mock tests

**Definition:** Every PR must run builds and tests automatically, with required checks before merge.

### Deliverables
- [ ] GitHub Actions workflows:
  - [ ] build (IDF firmware build)
  - [ ] unit tests (Unity)
  - [ ] mock/host tests (if present)
  - [ ] lint/format checks (optional but recommended)
- [ ] Branch protection:
  - [ ] required status checks
  - [ ] required review(s)
- [ ] CI artefacts:
  - [ ] build logs
  - [ ] test results (JUnit)
  - [ ] optional firmware binaries for tagged releases

### Acceptance criteria
- [ ] A clean checkout in CI can build and test without manual steps
- [ ] PR cannot merge if tests fail

**Owner:** DevOps / Firmware Lead  
**Status:** Not started  
**Links:**  
- Traceability: `docs/tests/TRACEABILITY.md`

---

## R4. Use the generated docs as references and keep them in the workspace for Codex

**Definition:** The “Codex reference pack” lives in the repo, with a single canonical entry point.

### Deliverables
- [ ] Place reference docs in stable locations:
  - [ ] `docs/contracts/v1/` (module v1 contracts + micro passes)
  - [ ] `docs/analysis/` (entry points, bus union, dep edges, calls)
  - [ ] `docs/architecture/` (IDF_TARGET_ARCH)
  - [ ] `docs/interfaces/` (COMMANDS_EVENTS)
  - [ ] `docs/tests/` (ACCEPTANCE_INVARIANTS + TRACEABILITY)
- [ ] Create `docs/START_HERE.md` with:
  - [ ] “What to read first”
  - [ ] “How to use contracts”
  - [ ] “Do not change behaviour without updating acceptance gates”
- [ ] Update README to point to `docs/START_HERE.md`

### Acceptance criteria
- [ ] New engineer can find contracts, gates, and architecture in under 5 minutes
- [ ] Codex prompts can reference stable paths

**Owner:** Firmware Lead  
**Status:** In progress  
**Links:**  
- Repo: `https://github.com/daviddebethel/GSM4-v2-IDF.git`

---

## R5. Set up for later Hardware-in-the-Loop (HIL) testing

**Definition:** Build in the hooks and interfaces now so HIL can be added later without refactoring core code.

### Deliverables
- [ ] `CONFIG_TEST_HARNESS_MODE` build option
- [ ] Deterministic injection points for:
  - [ ] modem responses (simulated)
  - [ ] button presses / GPIO inputs
  - [ ] ADC readings
  - [ ] I2C peripheral responses
- [ ] Non-blocking log capture buffer suitable for field retrieval and later web-terminal support
- [ ] Initial `docs/tests/HIL_PLAN.md` describing required signals and expected observations

### Acceptance criteria
- [ ] Test harness mode runs without hardware and can drive key flows
- [ ] HIL plan exists and is reviewable by test engineers

**Owner:** Firmware + Test Engineering  
**Status:** Not started

---

## R6. Track test requirements for test engineers using QASE

**Definition:** QASE is the canonical test case store; this repo provides traceability to requirements and CI results.

### Deliverables
- [ ] QASE project structure defined:
  - suites per module (CFG, WEB, MODEM, AUD, PWR, UI, DTMF)
  - sub-suites: Unit, Mock, Integration, HIL, Regression
- [ ] `docs/tests/TRACEABILITY.md` maps:
  - acceptance gate → QASE test case ID(s) → CI job(s)
- [ ] CI produces test result artefacts suitable for QASE ingestion (JUnit XML)

### Acceptance criteria
- [ ] Every Phase 1 acceptance gate has at least one QASE case mapped
- [ ] CI results can be attached to a QASE run (manual at first, automated later)

**Owner:** QA / Test Engineering  
**Status:** Not started

---

# Cross-cutting constraints and “do not forget” list

## Security and OTA (planned, not required for Phase 1 parity)
- [ ] Partition layout reserves space for OTA A/B and storage needs
- [ ] Logging policy prohibits secrets (keys, certs, passwords, tokens)
- [ ] Any future debug channels must be:
  - authenticated
  - rate limited
  - feature-flagged
  - non-blocking

## Web stack change
- [ ] Legacy web routes and response shapes must be preserved, even if the server changes to `esp_http_server`

---

# Meeting notes / change log
Append dated notes here as decisions change.

- YYYY-MM-DD: (example) Chose compatibility persistence for Phase 1.
