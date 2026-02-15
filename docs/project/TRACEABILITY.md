# Traceability

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`

This file links:
- Requirements (R1–R6 in `docs/project/REWRITE_PROGRAMME.md`)
- Acceptance gates (from `docs/tests/ACCEPTANCE_INVARIANTS.md`)
- QASE test cases (owned by test engineers)
- CI jobs and artefacts (GitHub Actions)

This enables: consistent QA planning, regression confidence, and audit-ready documentation.

---

## How to use this file

1) When you add or change behaviour:
- Update the relevant module contract (if behaviour changes legitimately).
- Update acceptance gates in `docs/tests/ACCEPTANCE_INVARIANTS.md`.
- Update this traceability mapping with QASE and CI references.

2) When you add tests:
- Add a QASE test case ID.
- Ensure CI produces a result artefact (JUnit XML preferred).
- Reference the CI job name that produces/executes the test.

---

## Conventions

### IDs
- **REQ:** R1…R6 (from `docs/project/REWRITE_PROGRAMME.md`)
- **GATE:** Use a stable identifier `GATE-<MODULE>-NN`.
- **QASE:** Use the QASE Case ID (example `QASE-123`).
- **CI Job:** Use the workflow and job name, for example:
  - `ci.yml / build-firmware`
  - `ci.yml / unit-tests`
  - `ci.yml / mock-tests`

### Module tags
Use these module codes:
- TMB (Timebase)
- SER (Serial Transport)
- MOD (Modem Core)
- WEB (WiFi/Web Control Plane)
- CFG (Config Store)
- AUD (Audio Runtime)
- PWR (Power and Physical IO)
- UI  (Sensor and Visual UI)
- DTMF (In-call DTMF Control)

---

## Traceability matrix (Phase 1)

| Req | Module | Gate ID | Acceptance gate (WHEN/THEN) | QASE Case ID(s) | CI workflow/job | Test type | Evidence artefact |
|---|---|---|---|---|---|---|---|
| R1 | TMB | GATE-TMB-01 | WHEN <trigger>, THEN <outcome>. | QASE- | ci.yml / unit-tests | unit | artifacts/junit.xml |
| R1 | CFG | GATE-CFG-01 | WHEN <trigger>, THEN <outcome>. | QASE- | ci.yml / unit-tests | unit | artifacts/junit.xml |
| R1 | WEB | GATE-WEB-01 | WHEN <trigger>, THEN <outcome>. | QASE- | ci.yml / integration-tests | integration | artifacts/junit.xml |
| R2 | ALL | GATE-TEST-01 | WHEN tests run, THEN results are produced as JUnit. | QASE- | ci.yml / unit-tests | unit | artifacts/junit.xml |
| R3 | ALL | GATE-CI-01 | WHEN PR opened, THEN build+tests run automatically. | (n/a) | GitHub branch rules | policy | GitHub settings |

Notes:
- “Acceptance gate” should be copied verbatim from `docs/tests/ACCEPTANCE_INVARIANTS.md`.
- Add multiple QASE cases per gate if needed (unit + mock + HIL).

---

## QASE structure (recommended)

Create suites in QASE:
- CFG, WEB, MOD, AUD, PWR, UI, DTMF, SER, TMB
Sub-suites in each:
- Unit
- Mock
- Integration
- HIL
- Regression

Tag test cases:
- `phase1`, `phase2-security`, `phase3-ota`, `phase4-cloud`, `phase5-debug`
- plus module tag (CFG/WEB/etc)

---

## CI artefacts (recommended)

Standardise on:
- `artifacts/junit.xml` for unit + mock tests
- `artifacts/build.log` for build output
- Optional: `artifacts/firmware.bin` for tagged releases

---

## Later phases traceability stubs

### Phase 2 — Security hardening
Add rows for:
- secure boot enabled (verification method)
- flash encryption enabled (verification method)
- key/cert storage verified (no leakage)
- TLS enablement regression tests

### Phase 3 — OTA dual image
Add rows for:
- successful OTA install
- rollback on boot failure
- power-loss tolerance during OTA steps

### Phase 5 — Field debug UX
Add rows for:
- Bluetooth serial enabled under feature flag only
- web terminal page auth + rate limit
- no secrets in captured logs

---

## Change log
- YYYY-MM-DD: Created traceability file for GSM4-v2-IDF rewrite.
