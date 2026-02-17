# Start here (GSM4 v2 IDF rewrite)

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`

This repo is a full rewrite of the GSM4 firmware onto **ESP-IDF**, preserving legacy behaviour via evidence-based module contracts and acceptance gates.

---

## What to read first (in order)

1) **Programme board (tasks and phases)**  
- `docs/project/REWRITE_PROGRAMME.md`

2) **Acceptance gates (definition of done for parity)**  
- `docs/tests/ACCEPTANCE_INVARIANTS.md`

3) **Target architecture (tasks, ownership, IPC, boot order, phased security + OTA hooks)**  
- `docs/architecture/IDF_TARGET_ARCH.md`

4) **Interfaces (commands, events, status snapshot, mapping from legacy bus fields)**  
- `docs/interfaces/COMMANDS_EVENTS.md`

5) **Module contracts (v1 rewrite contracts, one per module)**  
- `docs/contracts/v1/`

6) **ESP-IDF local setup (activation, build wrappers, VS Code defaults)**
- `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`

---

## Key rules (to keep the rewrite correct)

- **No behaviour changes by accident**: any behaviour change must update:
  - the relevant module contract in `docs/contracts/v1/`
  - the acceptance gate(s) in `docs/tests/ACCEPTANCE_INVARIANTS.md`
  - traceability in `docs/tests/TRACEABILITY.md`

- **HTTP handlers must not directly mutate hardware state**: web routes translate to commands; owning tasks apply side effects.

- **Single-writer ownership** is the target model. If compatibility requires multi-writer semantics in Phase 1, it must be explicit and logged.

- **Do not leak secrets** (now or later): never log keys, certs, passwords, tokens, or raw credential material.

---

## Repo structure (recommended)

- `/firmware/` — ESP-IDF firmware project
- `/web/` — React + TypeScript UI (format preserved, served as static assets)
- `/docs/` — contracts, architecture, interfaces, tests, and project docs
- `/tools/` — scripts, parsers, harness utilities
- `/.github/workflows/` — CI pipelines

---

## Where the legacy evidence lives

These artefacts are references that support the contracts and the architecture:

- Execution topology:
  - `docs/analysis/03_entry_points.md`

- Bus/shared-state coupling (merge-ready union):
  - `docs/analysis/05_bus_access_union_fn_refined.md`

- System recomposition and dependencies:
  - `docs/analysis/13_dependency_edges.csv`
  - `docs/analysis/13_system_recomposition.md`

- Call interaction evidence:
  - `docs/analysis/A_calls_cluster_all_cpp.md`

---

## If you are using GPT Codex

When asking Codex to implement a change, always include:
- the relevant module contract(s) from `docs/contracts/v1/`
- the specific acceptance gates that must pass
- the relevant command/event interface entry from `docs/interfaces/COMMANDS_EVENTS.md`
- a reminder: “preserve behaviour, do not redesign”

---

## Quick start checklist (Phase 1 bring-up)

- [ ] Firmware builds in CI from a clean checkout
- [ ] `app_main` initialises NVS, mounts FS (if used), brings up WiFi
- [ ] IDF native web server running (`esp_http_server`)
- [ ] `/api/status` returns a valid snapshot payload
- [ ] Unit and mock tests run in CI and produce JUnit artefacts
