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
