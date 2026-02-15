# Config Store
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: persistence/defaults backend for `configReg` values using SPIFFS-backed config file operations in `src/src/file_handler.cpp`.
- Model: Config Store persists `configReg` to SPIFFS; other modules consume `configReg` as readers (pull model).
- Non goals: None stated in code.

## Owner execution context
- Primary execution contexts: `loop` (`config_init`, `config_read`, `config_default`, and periodic `config_write` call path from `src/main.cpp:loop()`).
- Primary execution contexts: `http route handler` (`handleConfigUpload(...)` calls `config_write()`, and `/config` command path can call `config_default()`).
- Primary execution contexts: `task: modem` (`sms_apply_esim()` calls `config_write()` in modem thread flow).
- Callers: System Orchestrator (`src/main.cpp`), WiFi/Web Control Plane (`src/src/wifi_handler.cpp`), Power and Physical IO (`src/src/button_handler.cpp`), Modem Core (`src/src/modem_handler.cpp`).

## Entry points
- `bool config_init(void)`: called from `src/main.cpp` PREINIT_GO branch.
- `void config_read(void)`: called from `src/main.cpp` PREINIT_GO branch (non-debug path).
- `void config_default(void)`: called from `src/main.cpp` PREINIT_GO debug path, `src/src/button_handler.cpp:button_task`, `src/src/wifi_handler.cpp:handle_command_requests` (`config_default` action), and internal recovery path in `config_read()`.
- `bool config_write(void)`: called from periodic config-change branch in `src/main.cpp:loop()`, PREINIT_GO debug path in `src/main.cpp`, `button_task`, `handleConfigUpload`, `sms_apply_esim`, and internal recovery path in `config_read()`.

## Inputs
- `config_read()` input source: file content from `SPIFFS.open(CONFIG_FILE_PATH, "r")` where `CONFIG_FILE_PATH` is `"/config.ini"`.
- `config_write()` input source: current in-memory `configReg.*` fields serialized in fixed write order.
- `config_default()` input source: compile-time default constants (`DEFAULT_*`) and runtime MAC-derived UIN (`WiFi.macAddress`).
- `generalFlags.config_changed` is consumed by `src/main.cpp:loop()` to trigger deferred persistence (`config_write()`).

## Outputs and side effects
- Reads and writes `"/config.ini"` via SPIFFS (`SPIFFS.open`, `SPIFFS.exists`, file read/write/close).
- On missing/outdated file in `config_read()`, executes recovery path `config_default(); config_write();`.
- Persists auth, whitelist, logs, FTP, audio, modem, UI, and power-related configuration fields from `configReg`.
- Sets `generalFlags.config_changed = true` in migration/repair branches inside `config_read()` when legacy/auth/whitelist defaults are applied.

## Owned state
- Persistent artifact: `"/config.ini"` (`CONFIG_FILE_PATH` in `src/src/file_handler.cpp`).
- Persistence format: ordered newline-delimited values (not key/value INI pairs), plus terminator marker `5846325`; legacy detection uses marker `5846324`.
- Persisted config categories in `config_write()`/`config_read()` include:
- Core identity/config: `configReg.PIN`, `configReg.UIN`, `configReg.flags.Val`, `configReg.telephone_operating_mode`.
- Audio and call behavior: `configReg.announcement_volume`, `configReg.ringer_volume`, `configReg.mimic_*`, `configReg.audo_path_test_period`, `configReg.auto_*`.
- Power/UI/IMU: `configReg.battery_chargerate`, `configReg.battery_low_level`, `configReg.status_led_brightness`, `configReg.door_led_brightness`, `configReg.imu_tip_threshold`.
- Credentials/roles/salts: `configReg.wifi_web_user*`, `configReg.wifi_web_password*`, `configReg.wifi_web_salt*`, `configReg.wifi_web_enabled*`, `configReg.wifi_web_role*`.
- Whitelist/logs/FTP: `configReg.whitelist_*`, `configReg.logs[*]`, `configReg.no_sms_logs`, `configReg.no_web_logs`, `configReg.ftp_*`.
- Config-change flag ownership note: persistence trigger flag `generalFlags.config_changed` is set by multiple modules and cleared in `src/main.cpp` immediately before periodic `config_write()`.

## Timing and triggering
- Startup triggering: PREINIT_GO path in `src/main.cpp:loop()` calls `config_init()` and then `config_read()` (or debug path `config_default()+config_write()`).
- Periodic loop triggering: `src/main.cpp:loop()` calls `config_write()` when `generalFlags.config_changed` is set and `TickGet() - config_tick >= (TICK_SECOND * CONFIG_CHECK_PERIOD)`.
- HTTP-triggered save: `src/src/wifi_handler.cpp:handleConfigUpload(...)` calls `config_write()` on final upload chunk.
- Button-triggered save: `src/src/button_handler.cpp:button_task()` long-hold path calls `config_default(); config_write();` before restart.
- Modem-triggered save: `src/src/modem_handler.cpp:sms_apply_esim()` calls `config_write()` when SIM mode changes.

## Concurrency and ordering rules
- Proven multi-context writes/calls into Config Store:
- `loop` context calls `config_write()` periodically and during PREINIT_GO debug path.
- `http route handler` context calls `config_write()` (`handleConfigUpload`) and `config_default()` (`handle_command_requests`, `config_default` action).
- `task: modem` context calls `config_write()` in `sms_apply_esim()` from modem thread flow.
- Proven protection evidence: no `xSemaphore`, mutex, `portENTER_CRITICAL`, `atomic`, or `__atomic` usage around `config_init/config_read/config_default/config_write` call paths in inspected files.
- Ordering behavior: loop clears `generalFlags.config_changed` immediately before periodic `config_write()`; other contexts can set/trigger writes independently.

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| WiFi/Web Control Plane | shared-state | `src/src/wifi_handler.cpp`: `wifi_start_ap()` reads `configReg.flags.location_text_setup` and `configReg.phone_loc`; `handleSetupXML`/`handleConfigXML`/`handleValuesXML` read and emit persisted `configReg.*` values. |
| Audio Runtime | shared-state | `src/src/audio_handler.cpp`: audio test flow checks `configReg.telephone_operating_mode` in `audio_test_task`; `src/main.cpp` uses persisted audio config (`configReg.announcement_volume`, `configReg.ringer_volume`, `configReg.mimic_volume`, `configReg.flags.audio_playback_enable`) before `audio_set_volume(...)`/`audio_playback(...)`. |
| Power and Physical IO | shared-state | `src/src/power_handler.cpp`: `power_task()` reads `configReg.battery_low_level`; `power_set_config()` calls `power_set_chargerate(configReg.battery_chargerate)` and checks `configReg.flags.charger_temperature_enable`. |
| Sensor and Visual UI | shared-state | `src/src/leddisplay_handler.cpp`: `leddisplay_init()` reads `configReg.status_led_brightness`, `leddisplay_task()` reads `configReg.flags.led_board_status_enabled`; `src/src/imu_handler.cpp` reads `configReg.flags.imu_enabled` and `configReg.imu_tip_threshold`. |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| Power and Physical IO | call | `src/src/button_handler.cpp:button_task()` long-hold path calls `config_default()` then `config_write()`. |
| WiFi/Web Control Plane | call | `src/src/wifi_handler.cpp:handle_command_requests()` action `config_default` calls `config_default()`; `handleConfigUpload(...)` calls `config_write()`. |
| Modem Core | shared-state | `src/src/modem_handler.cpp:sms_decode()` writes caller-owned fields such as `configReg.voice_no*`, `configReg.sms_user_no*`, `configReg.PIN`; `src/src/file_handler.cpp:config_write()` persists those fields. |
| System Orchestrator | call | `src/main.cpp:loop()` PREINIT_GO branch calls `config_init()` then `config_read()` (or debug `config_default()+config_write()`); loop periodic branch calls `config_write()` on config-change gate. |
| Modem Core | call | `src/src/modem_handler.cpp:sms_apply_esim()` calls `config_write()`; this path is reached from `sms_decode()` in `modem_task()` modem thread flow. |

## Evidence sources
- `src/src/file_handler.cpp`: `config_init`, `config_read`, `config_write`, `config_default`, `migrate_legacy_passwords`, `CONFIG_FILE_PATH`.
- `src/main.cpp`: `loop()` PREINIT_GO config init/read/default path; periodic config-change `config_write` gate.
- `src/src/wifi_handler.cpp`: `handle_command_requests` (`config_default` action), `handleConfigUpload` (`config_write` call).
- `src/src/button_handler.cpp`: long-hold default/reset path calling `config_default` + `config_write`.
- `src/src/modem_handler.cpp`: `sms_apply_esim` direct `config_write` call, `sms_decode` config mutation paths.
- `src/src/power_handler.cpp`, `src/src/leddisplay_handler.cpp`, `src/src/imu_handler.cpp`, `src/src/audio_handler.cpp`: downstream consumers of persisted `configReg` fields.
- `docs/analysis/13_system_recomposition.md` and `docs/analysis/13_dependency_edges.csv`.

## Uncertainties to verify
- Exact runtime thread used by AsyncWebServer callback execution is not shown in repository code, so precise interleaving with loop-based writes remains UNCERTAIN.
- No repository-local synchronization primitive is shown around concurrent `config_write()` call sites (`loop`, HTTP upload handler, modem task), so cross-context ordering at runtime is UNCERTAIN.
