# ESP32-WROOM Serial Deploy Guide (GSM4-v2-IDF)

Date: 2026-02-22
Scope: Build, flash, and monitor GSM4 firmware on ESP32-WROOM over serial (UART0).

## 1) Where things are in this repo

- Repo root: `GSM4-v2-IDF-1/`
- Firmware project: `firmware/`
- ESP-IDF (pinned): `third_party/esp-idf/`
- Build helper: `tools/build_firmware.sh`
- ESP-IDF env helper: `tools/idf_env.sh`

Build output location (by profile):
- `firmware/build/<profile>/`

Important generated files:
- App binary: `firmware/build/<profile>/gsm4_v2_idf.bin`
- Bootloader: `firmware/build/<profile>/bootloader/bootloader.bin`
- Partition table: `firmware/build/<profile>/partition_table/partition-table.bin`
- OTA data: `firmware/build/<profile>/ota_data_initial.bin`
- Web FS image: `firmware/build/<profile>/webfs.bin`
- Flash argument file: `firmware/build/<profile>/flash_args`

## 2) Hardware path used for flashing

Programming/logging is via UART0:
- TXD0: GPIO1
- RXD0: GPIO3

Use the board USB serial interface connected to UART0.

## 3) One-time shell setup

From repo root:

```sh
source tools/idf_env.sh
```

## 4) Build firmware

Recommended (profile-aware wrapper):

```sh
tools/build_firmware.sh --profile dev build
```

Other valid profiles:
- `dev`
- `ci-test`
- `ci-secure`
- `release`

Example:

```sh
tools/build_firmware.sh --profile ci-test build
```

## 5) Find your serial port

Common macOS commands:

```sh
ls /dev/cu.*
```

Typical device names:
- `/dev/cu.usbserial-*`
- `/dev/cu.SLAB_USBtoUART`
- `/dev/cu.usbmodem*`

## 6) Flash over serial

From repo root (wrapper already built `firmware/build/dev`):

```sh
cd firmware
idf.py -B build/dev -p <PORT> flash
```

Example:

```sh
idf.py -B build/dev -p /dev/cu.usbserial-0001 flash
```

Optional flash speed:

```sh
idf.py -B build/dev -p <PORT> -b 460800 flash
```

If auto-download fails:
- Hold BOOT,
- Press/release RESET,
- Release BOOT when flashing starts.

## 7) Memory map for ESP32 programming tools

When using a manual flash tool, each generated `.bin` must be written to the correct flash offset.

Primary source of truth:
- `firmware/build/<profile>/flash_args`

Partition-layout references:
- `firmware/partitions_16mb.csv`
- `firmware/build/<profile>/partition_table/partition-table.bin`

### 7.1 Required image-to-offset mapping

For this project (default partition layout), use:

| Image | File path (under `firmware/build/<profile>/`) | Flash offset |
|---|---|---|
| Bootloader | `bootloader/bootloader.bin` | `0x1000` |
| Partition table | `partition_table/partition-table.bin` | `0x8000` |
| OTA data | `ota_data_initial.bin` | `0x14000` |
| App (OTA slot 0) | `gsm4_v2_idf.bin` | `0x20000` |
| Web FS (FAT image) | `webfs.bin` | `0x620000` |

If you change the partition table, regenerate and re-check `flash_args` before flashing.

### 7.2 Example `esptool` manual command

From `firmware/`:

```sh
python -m esptool --chip esp32 -b 460800 --before default_reset --after hard_reset \
  write_flash --flash_mode dio --flash_freq 40m --flash_size 16MB \
  0x1000 build/dev/bootloader/bootloader.bin \
  0x8000 build/dev/partition_table/partition-table.bin \
  0x14000 build/dev/ota_data_initial.bin \
  0x20000 build/dev/gsm4_v2_idf.bin \
  0x620000 build/dev/webfs.bin
```

### 7.3 Espressif Flash Download Tool (GUI) mapping

If using Espressif’s Flash Download Tool, add one row per image with these addresses:

- `bootloader.bin` -> `0x1000`
- `partition-table.bin` -> `0x8000`
- `ota_data_initial.bin` -> `0x14000`
- `gsm4_v2_idf.bin` -> `0x20000`
- `webfs.bin` -> `0x620000`

And set flash parameters to match build output:
- Flash mode: `dio`
- Flash frequency: `40m`
- Flash size: `16MB`

## 8) Open serial monitor

```sh
cd firmware
idf.py -B build/dev -p <PORT> monitor
```

Combined flash + monitor:

```sh
idf.py -B build/dev -p <PORT> flash monitor
```

Monitor exit shortcut:
- `Ctrl+]`

## 9) What success looks like in logs (Phase 0)

Look for these signatures:
- `BOOT: phase0 boot start board=gsm4_rev4 uart0=ok`
- `I2C: init ok port=0 scl=26 sda=27 hz=100000`
- `KTS: 0x20 init ok`
- `KTS: 0x21 init ok`
- `BQ: init ok addr=0x6B`
- `BOOT: phase0 bringup complete`

Full bench checklist:
- `docs/bringup/phase0_bench_checklist.md`

## 10) Alternative manual flash command via generated args file

After a successful build, you can use the generated `flash_args` directly:

```sh
cd firmware/build/dev
python -m esptool --chip esp32 write_flash @flash_args
```

## 11) Quick troubleshooting

- IDF not active:
  - `source tools/idf_env.sh`
- Wrong port:
  - re-check `ls /dev/cu.*`
- Port busy:
  - close any other serial monitor apps
- Build profile mismatch:
  - ensure `-B build/<profile>` matches what you built
- Flash offset mismatch:
  - verify against `firmware/build/<profile>/flash_args`

## 12) Useful references

- Setup baseline: `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`
- Main repo readme: `README.md`
- Phase 0 proposal: `docs/bringup/phase0_proposal.md`
