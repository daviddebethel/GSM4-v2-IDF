# Decisions (GSM4 v2 IDF rewrite)

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`
**Last updated:** 2026-02-17

This file is a lightweight decision log (ADR-style).
It exists to capture the “why” behind choices so we do not re-litigate them later, and so GPT Codex stays aligned.

---

## How to add a decision

Append a new section using this template:

### ADR-YYYYMMDD-<short-title>
- **Status:** Accepted | Superseded | Draft
- **Context:** (what problem we are solving)
- **Decision:** (what we decided)
- **Options considered:** (A, B, C)
- **Consequences:** (what this enables, what it costs)
- **Links:** (PRs, issues, docs)

---

## Decisions

### ADR-20260215-monorepo
- **Status:** Accepted
- **Context:** We need tight coupling between firmware, web UI assets, CI, and documentation/traceability.
- **Decision:** Use a **single monorepo** with `/firmware` and `/web` folders.
- **Options considered:** Monorepo vs split repos (firmware repo + UI repo).
- **Consequences:** Simplifies CI and traceability; requires clear folder boundaries and tooling.
- **Links:** `docs/project/REWRITE_PROGRAMME.md`

### ADR-20260215-idf-native-http-server
- **Status:** Accepted
- **Context:** Legacy uses an Arduino or Async server style; rewrite target is ESP-IDF native architecture.
- **Decision:** Use **ESP-IDF native HTTP server (`esp_http_server`)** for the web control plane.
- **Options considered:** Keep ESPAsyncWebServer vs migrate to `esp_http_server`.
- **Consequences:** Better alignment with IDF; requires route handler rewrite; behaviour must match existing routes and payloads.
- **Links:** `docs/contracts/v1/wifi_web_control_plane.md`, `docs/architecture/IDF_TARGET_ARCH.md`

### ADR-20260215-react-typescript-ui-format-preserved
- **Status:** Accepted
- **Context:** UI must remain familiar and preserve page formatting, while enabling modern development workflows.
- **Decision:** Use **React + TypeScript** for the UI, preserving existing formatting as a visual contract.
- **Options considered:** Keep static HTML; full redesign; React enhancement of existing pages.
- **Consequences:** Requires a packaging strategy (FS image or embedded assets); introduces a web build pipeline.
- **Links:** `docs/architecture/IDF_TARGET_ARCH.md`

### ADR-20260215-phase-gated-security-and-ota
- **Status:** Accepted
- **Context:** Security (X.509, secure boot, flash encryption) and OTA dual-image are required, but should not block Phase 1 parity.
- **Decision:** Implement as a **phase-gated roadmap**:
  - Phase 1: functional parity
  - Phase 2: security hooks and hardening foundations
  - Phase 3: OTA dual image
  - Phase 4: cloud comms roadmap (TLS 1.3 capable, SNMPv3 capable)
  - Phase 5: field debug UX (Bluetooth serial, web serial terminal)
- **Options considered:** Implement security and OTA from day one vs postpone entirely vs hooks-first.
- **Consequences:** Early partitioning and interface hooks are mandatory; full implementation can come later without rework.
- **Links:** `docs/architecture/IDF_TARGET_ARCH.md`, `docs/project/REWRITE_PROGRAMME.md`

### ADR-20260215-acceptance-gates-as-definition-of-done
- **Status:** Accepted
- **Context:** AI-assisted rewrites risk behavioural drift, especially across concurrency and timing boundaries.
- **Decision:** Treat `docs/tests/ACCEPTANCE_INVARIANTS.md` as the **definition of done** for Phase 1 parity.
- **Options considered:** Informal manual testing; ad-hoc test notes; acceptance gates plus traceability.
- **Consequences:** Requires ongoing maintenance of gates and trace mapping; yields much higher rewrite confidence.
- **Links:** `docs/tests/ACCEPTANCE_INVARIANTS.md`, `docs/tests/TRACEABILITY.md`

### ADR-20260215-commands-events-interface
- **Status:** Accepted
- **Context:** Legacy coupling via shared “bus” globals must be made explicit for IDF task ownership and IPC.
- **Decision:** Use a documented **Commands and Events** interface as the boundary between web handlers and owning tasks.
- **Options considered:** Keep shared globals; event-only; queue-only; mixed queue plus event groups plus snapshot.
- **Consequences:** Clear ownership and testability; requires careful mapping from legacy bus semantics.
- **Links:** `docs/interfaces/COMMANDS_EVENTS.md`, `docs/analysis/05_bus_access_union_fn_refined.md`

### ADR-20260216-idf-version-and-upgrade-policy
- **Status:** Accepted
- **Context:** We need deterministic tooling and controlled change risk during Phase 1 parity.
- **Decision:** ESP-IDF pinned to v5.5.2.
- **Options considered:** Floating latest patch releases; immediate minor upgrades.
- **Consequences:** Upgrade policy: bugfix updates only during Phase 1 parity, planned minor upgrades at phase boundaries.
- **Links:** `docs/project/REWRITE_PROGRAMME.md`

### ADR-20260217-idf-env-shell-and-workspace-hardening
- **Status:** Accepted
- **Context:** Local setup failed for contributors using `zsh` because `tools/idf_env.sh` was Bash-specific (`BASH_SOURCE`) and changed caller shell options (`set -euo pipefail`) when sourced. The workspace file also lacked ESP-IDF settings, so extension behavior depended on machine-global state.
- **Decision:** Make `tools/idf_env.sh` source-safe for `bash` and `zsh` and keep it side-effect free for shell options; keep `tools/build_firmware.sh` as an executable wrapper; commit workspace defaults for `idf.espIdfPath`, `idf.currentSetup`, `idf.toolsPath`, and `idf.customExtraVars.IDF_TOOLS_PATH`.
- **Options considered:** Keep bash-only helpers and require direct `third_party/esp-idf/export.sh` usage; rely on each developer to run extension setup wizard manually; harden scripts plus commit workspace defaults.
- **Consequences:** Consistent activation/configuration across shells and machines, fewer onboarding regressions, less dependence on undocumented local VS Code state.
- **Links:** `tools/idf_env.sh`, `tools/build_firmware.sh`, `GSM4-v2-IDF-1.code-workspace`, `.vscode/settings.json`, `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`

### ADR-20260217-vscode-idf-extension-v2-registration
- **Status:** Accepted
- **Context:** ESP-IDF extension `v2.x` discovers available setups from installation metadata files (`esp_idf.json` / `eim_idf.json`). On clean machines this metadata may be missing even when `idf.py` works in shell, causing an empty `ESP-IDF: Explorer` command view.
- **Decision:** Add and use a repo script `tools/register_idf_for_vscode.sh` to register the current checkout in the extension catalog (`$HOME/.espressif/esp_idf.json`), then select current IDF version in VS Code.
- **Options considered:** Depend only on extension installation manager UI; keep undocumented per-machine manual fixes; provide deterministic script plus documented workflow.
- **Consequences:** Reproducible extension onboarding, reduced IDE-only failure modes, and explicit bridge between shell-valid setup and extension metadata.
- **Links:** `tools/register_idf_for_vscode.sh`, `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `README.md`

### ADR-20260217-serial-first-debugging
- **Status:** Accepted
- **Context:** Setup guide section 4 required a debugging strategy decision for bring-up and Phase 1 parity work.
- **Decision:** Use serial-only programming and debugging initially; no JTAG dependency in baseline setup.
- **Options considered:** Serial-only first; JTAG from day one; serial default with optional JTAG.
- **Consequences:** Faster onboarding and less hardware setup friction. Limitation: no hardware breakpoints/watchpoints until a JTAG workflow is introduced.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `README.md`

### ADR-20260217-ci-actions-only-submodule-phase1
- **Status:** Accepted
- **Context:** Setup guide section 4 required a CI/reproducibility decision for Phase 1 parity work.
- **Decision:** Use GitHub Actions only in Phase 1, building from repository checkout with pinned `third_party/esp-idf` submodule; no pinned Docker image for Phase 1.
- **Options considered:** GitHub Actions only vs Actions plus local runner pattern; submodule checkout vs pinned Docker image.
- **Consequences:** Lower setup/maintenance overhead and faster adoption now; less hermetic than Docker, so revisit if environment drift causes repeated local-vs-CI mismatches.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `README.md`

### ADR-20260217-web-assets-filesystem-image
- **Status:** Accepted
- **Context:** Setup guide section 4 required a packaging decision for the React + TypeScript web UI served from the device.
- **Decision:** Package web build artifacts as a flash filesystem image in a dedicated data partition, served by `esp_http_server`.
- **Options considered:** Flash filesystem image; compile assets into firmware blobs.
- **Consequences:** Better scaling for larger web bundles and cleaner separation between app binary and static assets; requires partition/mount/image-generation plumbing and release traceability between firmware and web artifact versions.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `docs/contracts/v1/wifi_web_control_plane.md`, `README.md`

### ADR-20260217-flash-layout-fatfs-and-release-security
- **Status:** Accepted
- **Context:** Partitioning and storage/security decisions were required before scaffolding OTA A/B, web assets packaging, and release-profile controls.
- **Decision:** For ESP32-WROOM-32E (16MB flash), use a scalable custom partition strategy (8MB allocation profile initially, scalable to full 16MB usage), use FATFS with wear levelling + VFS for web/data storage, and require release profile secure boot + flash encryption + NVS encryption.
- **Options considered:** SPIFFS vs FATFS for web/data partition; fixed small partitioning vs scalable custom layout; NVS encryption only vs full release security profile (secure boot + flash encryption + NVS encryption).
- **Consequences:** Better long-term storage flexibility and OTA headroom with stronger release security posture; requires profile-aware build wiring, provisioning workflow, and compatibility checks because data/web partitions are not automatically rollback-coupled with OTA app rollback.
- **Testing impact:** Add FATFS/WL mount-read-write-recovery tests, OTA A/B compatibility tests against single-copy data partition behavior, and CI/security validation tests proving release profile enables secure boot + flash encryption + NVS encryption.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `docs/CODEX_RULES.md`, `docs/tests/ACCEPTANCE_INVARIANTS.md`, `README.md`

### ADR-20260217-build-profile-contract-option2
- **Status:** Accepted
- **Context:** Scaffolding requires a stable build-profile contract that balances fast feedback with release-equivalent security validation.
- **Decision:** Use build-profile contract Option 2: `dev`, `ci-test`, `ci-secure`, `release`.
- **Profile intent:**
  - `dev`: fastest local iteration, diagnostics enabled.
  - `ci-test`: deterministic unit/mock test profile for rapid CI feedback.
  - `ci-secure`: release-equivalent security profile validation (secure boot + flash encryption + NVS encryption).
  - `release`: production artifact profile with hardened defaults and security requirements enabled.
- **Options considered:** Option 1 (dev/ci-test/release); Option 2 (dev/ci-test/ci-secure/release); full matrix profile strategy.
- **Consequences:** Better confidence that release security settings remain valid without slowing fast test CI loop; adds one extra CI profile/job and associated maintenance.
- **Testing impact:** CI must include both `ci-test` and `ci-secure` gates, plus artifact policy checks verifying release outputs do not include development test/debug flags.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `README.md`, `docs/CODEX_RULES.md`

### ADR-20260217-ci-merge-gate-policy-option2
- **Status:** Accepted
- **Context:** Before scaffolding, PR merge policy needed a fixed required-check set so quality/security gates are predictable and aligned with Option 2 build profiles.
- **Decision:** Adopt Option 2 (Balanced Gate) as required merge policy with mandatory checks: `build-firmware`, `unit-tests`, `mock-tests`, `lint-format`, `ci-secure`, and `release-policy-check`; require at least one approval, resolved conversations, and up-to-date branch before merge.
- **Options considered:** Fast Gate (minimum checks), Option 2 Balanced Gate, Security-First hard gate on every PR, two-tier branch-only hardening.
- **Consequences:** Stronger default quality/security assurance with moderate PR runtime cost; requires stable job naming and branch-protection maintenance.
- **Testing impact:** Required checks must publish test/security artifacts (including JUnit for unit/mock and release-policy/security validation evidence) and block merge on failure.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `README.md`, `docs/tests/TRACEABILITY.md`

### ADR-20260217-release-log-and-debug-channel-policy
- **Status:** Accepted
- **Context:** Before scaffolding, release logging/debug behavior needed a fixed policy covering serial lockdown, runtime web debug controls, and release defaults.
- **Decision:** In `release`, serial debug is locked down and all debug channels are default-off. Runtime debug is allowed only through authenticated, role-gated, feature-flagged web controls with category-level runtime switches. Web debug supports terminal-style viewing and local capture/export, and optional secure upstream streaming remains default-off unless explicitly enabled.
- **Options considered:** Keep debug mostly available in release for support convenience; compile-time-only debug controls; policy-gated runtime controls with hardened release defaults.
- **Consequences:** Stronger production hardening with controlled field diagnostics; requires explicit profile wiring, auth/rate-limit controls, and release artifact policy validation.
- **Testing impact:** Add release-policy checks for serial lockdown and default-off debug state, plus integration/negative tests for auth gating, category controls, and no-secret logging.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `docs/architecture/IDF_TARGET_ARCH.md`, `docs/project/REWRITE_PROGRAMME.md`, `docs/CODEX_RULES.md`

### ADR-20260217-mock-test-stack-and-layout-contract
- **Status:** Accepted
- **Context:** Before scaffolding, the project needed a fixed mock/unit testing stack and directory/naming contract so local and CI execution remain aligned.
- **Decision:** Use Unity for on-target unit tests and CMock via ESP-IDF `idf_component_mock` for mock tests; standardize test layout under `firmware/components/<module>/test` and `firmware/components/<module>/test/mocks` with shared helpers under `firmware/tests/common`; standardize test/mocks naming and execute via shared scripts (`tools/test_unit.sh`, `tools/test_mock.sh`, `tools/test_secure_smoke.sh`).
- **Options considered:** Unity only; Unity + CMock; multi-framework strategy.
- **Consequences:** Predictable test structure and CI/local parity with lower tooling fragmentation; requires discipline to maintain module-level test boundaries and shared script entrypoints.
- **Testing impact:** `ci-test` must run deterministic unit+mock suites and publish JUnit artifacts; changes touching behavior must include matching unit/mock updates or explicit waiver metadata.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `docs/CODEX_RULES.md`, `docs/tests/TRACEABILITY.md`, `README.md`

### ADR-20260217-testing-baseline-unity-plus-mocks
- **Status:** Accepted
- **Context:** Setup guide section 4 required a testing baseline decision for Phase 1 parity and near-term development flow.
- **Decision:** Use ESP-IDF Unity on-target tests as baseline and implement mock-based unit tests for logic modules; treat HiL as a planned next layer rather than a current delivery gate.
- **Options considered:** Unity-only baseline; Unity plus mock unit tests; full HiL-first strategy.
- **Consequences:** Good short-term delivery speed with practical coverage growth; requires explicit hardware abstraction seams (HAL/interfaces) and staged planning for future HiL automation.
- **Links:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `docs/tests/ACCEPTANCE_INVARIANTS.md`, `README.md`

### ADR-20260217-codex-rules-governance
- **Status:** Accepted
- **Context:** Setup guide section 4 required a codex-guardrails decision to keep rewrite work aligned with architecture modernization, security, and testability goals without behavior drift.
- **Decision:** Add `docs/CODEX_RULES.md` as mandatory governance for Codex-delivered changes, including: hard-vs-migration boundaries, mandatory unit+mock testing for new/changed behavior, HiL-ready abstraction requirements, coding standards/lint/layout expectations, serial debug stream gating by build profile (development/CI-test vs release), and traceability/documentation obligations.
- **Options considered:** No explicit codex rules file; advisory-only guidance; mandatory enforceable rules file.
- **Consequences:** More consistent and auditable AI-assisted changes; stricter delivery discipline; occasional short-term overhead from required tests/documentation updates.
- **Links:** `docs/CODEX_RULES.md`, `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`, `docs/tests/ACCEPTANCE_INVARIANTS.md`, `README.md`

---

## Superseded decisions

(None yet)
