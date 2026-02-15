# GSM4 v2 IDF

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`  
**Last updated:** 2026-02-15

This repository is a rebuild of the GSM4 firmware onto **ESP-IDF**, with a modern web UI stack and a strong test and documentation discipline.

## Start here

- **Read first:** `docs/START_HERE.md`  
  (links to programme board, acceptance gates, target architecture, interfaces, and module contracts)

## Repository layout

- `/firmware/` — ESP-IDF firmware project
- `/web/` — React + TypeScript UI (served by the device as static assets)
- `/docs/` — contracts, architecture, interfaces, tests, and project docs
- `/tools/` — scripts, parsers, harness utilities
- `/.github/workflows/` — CI pipelines

## Workflow (high level)

1) Use **module contracts** as rewrite contracts: `docs/contracts/v1/`  
2) Use **acceptance gates** as definition of done: `docs/tests/ACCEPTANCE_INVARIANTS.md`  
3) Use **commands and events** as the system interface: `docs/interfaces/COMMANDS_EVENTS.md`  
4) Track progress in the programme board: `docs/project/REWRITE_PROGRAMME.md`

## Testing and traceability

- Trace mapping (acceptance → QASE → CI): `docs/tests/TRACEABILITY.md`
- Decisions log (why we chose what we chose): `docs/project/DECISIONS.md`

## CI

CI runs on every pull request and on main (see `/.github/workflows/`).  
The expected minimum jobs are:
- firmware build
- unit tests
- mock tests (where applicable)

## Contributing guidelines (important)

- Do not change behaviour unintentionally.
- If behaviour changes legitimately, update:
  - the relevant module contract
  - acceptance gates
  - traceability mappings

## Quick commands (placeholders)

These are placeholders until `/firmware` and CI scripts are finalised. Update this section once the project skeleton is in place.

- Firmware build (example):
  - `idf.py build`
- Firmware unit tests (example):
  - `idf.py test` or `idf.py -T all build` (depending on your chosen IDF test setup)

