# Project Context

## ESP-IDF Setup Baseline (2026-02-17)

- ESP-IDF is pinned in-repo at `third_party/esp-idf` (tag `v5.5.2`).
- Preferred local activation command:
  - `source tools/idf_env.sh`
- Preferred firmware build command:
  - `./tools/build_firmware.sh`

## Setup Pitfalls Addressed

- `tools/idf_env.sh` is now compatible with both `bash` and `zsh`.
- `tools/idf_env.sh` no longer applies `set -euo pipefail` to the caller shell.
- `GSM4-v2-IDF-1.code-workspace` and `.vscode/settings.json` include `idf.espIdfPath` / `idf.currentSetup` / `idf.toolsPath` defaults plus `idf.customExtraVars.IDF_TOOLS_PATH`.
- `tools/register_idf_for_vscode.sh` registers this checkout in the ESP-IDF extension v2 install catalog to avoid an empty command list in `ESP-IDF: Explorer`.

## Canonical Setup Reference

- `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`

## Phase 0 Bring-up Progress (2026-02-22)

- Branch delivered: `feature/phase0-bringup-implementation`.
- Implementation commit: `8b3e805` (`feat(phase0): implement deterministic BSP bring-up, drivers, and tests`).
- `main` is now thin and calls `bsp_init()` only (`firmware/main/main.c`).
- New Phase 0 components implemented:
  - `firmware/components/i2c_bus/` mockable I2C abstraction + IDF backend.
  - `firmware/components/drv_kts1622/` minimal KTS1622 driver via `i2c_bus`.
  - `firmware/components/drv_bq25622/` minimal BQ25622 driver via `i2c_bus`.
  - `firmware/components/bsp_gsm4_rev4/` extended bring-up sequence (`bsp_init.c`, `bsp_gpio.c`, `bsp_i2c.c`, `bsp_interrupts.c`).
- Deterministic boot logging/tags implemented per proposal intent: `BOOT`, `BSP`, `I2C`, `KTS`, `BQ`, `INT`, `SEC`.
- `CONFIG_GSM4_PHASE0_TELEMETRY` added to `firmware/main/Kconfig.projbuild`.
- Phase 0 security scope enforced in code/logging:
  - no network bring-up,
  - no AT passthrough shell,
  - no debug endpoints enabled by default,
  - no Secure Boot/Flash Encryption/NVS Encryption enablement in Phase 0.
- Bench validation artifact added: `docs/bringup/phase0_bench_checklist.md`.
- Host test coverage expanded with fake bus and required logic tests:
  - `firmware/components/*/test/` for BSP/KTS/BQ logic,
  - `firmware/tests/host/common/fake_i2c_bus.*`,
  - host unit/mock runners now execute Phase 0 test suites.
- Current verification status:
  - `../tools/test_unit.sh` passed (10 tests, 0 failures),
  - `../tools/test_mock.sh` passed (10 tests, 0 failures).
