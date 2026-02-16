# Decisions (GSM4 v2 IDF rewrite)

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`  
**Last updated:** 2026-02-15

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

---

## Superseded decisions

(None yet)
