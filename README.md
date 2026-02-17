# GSM4 v2 IDF

**Monorepo:** `https://github.com/daviddebethel/GSM4-v2-IDF.git`
**Last updated:** 2026-02-17

This repository is a rebuild of the GSM4 firmware onto **ESP-IDF**, with a modern web UI stack and a strong test and documentation discipline.

## Start here

- **Read first:** `docs/START_HERE.md`
  (links to programme board, acceptance gates, target architecture, interfaces, and module contracts)
- **ESP-IDF setup + troubleshooting:** `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`

## ESP-IDF troubleshooting (quick)

If ESP-IDF activation/configuration fails, run:

```sh
source tools/idf_env.sh
cd firmware
idf.py reconfigure
```

If **ESP-IDF: Explorer** shows no command entries, register this repo with the VS Code ESP-IDF extension:

```sh
./tools/register_idf_for_vscode.sh
```

Then in VS Code:
1) `ESP-IDF: Select Current ESP-IDF Version`
2) Select this repo's `third_party/esp-idf` entry
3) `Developer: Reload Window`

Common symptoms and fixes (zsh/bash wrapper compatibility, shell option side effects, and VS Code workspace IDF path) are documented in:
- `docs/setup guides/ESP_IDF_SETUP_GUIDE.md` (section `3.3.1 Common activation failures (and fixes now applied)`)

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

Phase 1 CI decision:
- GitHub Actions only.
- Build environment uses repository checkout with pinned `third_party/esp-idf` submodule (no pinned Docker image in Phase 1).
- Build-profile contract uses Option 2: `dev`, `ci-test`, `ci-secure`, `release`.

Scaffolding status:
- CI foundation is implemented in `.github/workflows/ci.yml` with submodule checkout and profile-aware job commands.
- Unit tests run a real Unity host test binary (`firmware/tests/host/unit`) and publish JUnit + host logs.
- Mock tests run a real Unity+CMock host test binary (`firmware/tests/host/mock`) and publish JUnit + host logs.

Current CI jobs:
- firmware build
- unit tests
- mock tests
- `ci-secure` release-equivalent security validation build
- `release-policy-check`

Required PR merge checks (Option 2 Balanced Gate):
- `build-firmware`
- `unit-tests`
- `mock-tests`
- `lint-format`
- `ci-secure`
- `release-policy-check` (`tools/check_release_policy.sh`)

Release debug policy:
- `release` profile defaults to serial debug locked down and runtime debug channels off.
- Runtime debug is enabled only via authenticated, role-gated, feature-flagged web controls.

Partition/webfs scaffolding:
- Firmware uses custom 16MB partition table: `firmware/partitions_16mb.csv` (OTA A/B + `webfs` FATFS partition).
- Firmware build packages web assets into `webfs` via `fatfs_create_spiflash_image`.
- Source path is `web/dist`; if absent, fallback scaffold content in `firmware/web_fallback` is packaged.

Phase 1 testing strategy:
- ESP-IDF Unity on-target tests are the baseline test runner.
- Mock-based unit tests are the unit-testing layer for logic modules.
- HiL test automation is planned as a later layer and reassessed at each development stage.
- Test stack/layout contract: Unity + CMock (`idf_component_mock`) with module-local `test`/`test/mocks` structure and shared `tools/test_*.sh` entrypoints for local/CI parity.

Codex governance:
- Codex-delivered changes must follow `docs/CODEX_RULES.md`.
- This includes mandatory unit+mock testing for behavior changes, HiL-ready abstraction seams, coding standards/lint/layout discipline, and serial debug stream gating by build profile (dev/CI-test vs release).

## Contributing guidelines (important)

- Do not change behaviour unintentionally.
- If behaviour changes legitimately, update:
  - the relevant module contract
  - acceptance gates
  - traceability mappings

## Quick commands

Build profiles:
- `tools/build_firmware.sh --profile dev build`
- `tools/build_firmware.sh --profile ci-test build`
- `tools/build_firmware.sh --profile ci-secure build`
- `tools/build_firmware.sh --profile release build`

Release policy validation:
- `tools/check_release_policy.sh`

Firmware unit tests (example):
- `tools/test_unit.sh`
- `tools/test_mock.sh`
- Note: host mock test generation uses CMock and requires `ruby` on the host.
