# Sensor and Visual UI
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: `CR17` IMU orientation/tilt processing, `CR18` LED status board compositing, and `CR19` TFT bring-up/message/logo rendering.
- Purpose: consume shared status/config fields and render UI/sensor outputs to IMU, LED strip, and TFT hardware paths.
- Non goals: None stated in code.

## Owner execution context
- Primary execution context:
  - `loop`: `src/main.cpp:loop()` calls `imu_task()` and `leddisplay_task()` every iteration; loop state-machine branches call `tft_init()`, `tft_logo()`, `tft_message()`, and `tft_deinit()`.
- External invocation contexts:
  - None proven for this module’s exported APIs outside loop call paths.
- Context caveat:
  - `tft_task()` is defined in `src/src/tft_handler.cpp` but no call site is present in `src/`.

## Entry points
- IMU path:
  - `imu_initialise()` called from `src/main.cpp` `INITIAL_GO` branch.
  - `imu_task()` called from `src/main.cpp:loop()`.
- LED path:
  - `leddisplay_init()` called from `src/main.cpp` `INITIAL_GO` branch.
  - `leddisplay_task()` called from `src/main.cpp:loop()`.
  - `leddisplay_clear()` called from `src/main.cpp` mode transitions (for example `IDLE_SLEEP` path).
- TFT path:
  - `tft_init()` called from `src/main.cpp` `INITIAL_GO` and multiple run-mode branches when power is re-enabled.
  - `tft_logo()` called from `src/main.cpp` after successful `tft_init()` in `INITIAL_GO`.
  - `tft_message(const char*)` called from `src/main.cpp` mode transitions (`WEBSERVER`, `ONHOOK`, `HANDSET`, `HANDSFREE`, `ANSWER`, `AUDIO TEST`, `FTP`).
  - `tft_deinit()` called from `src/main.cpp` `IDLE_SLEEP` path.
  - `tft_task()` exported but no repository call sites found.

## Inputs
- IMU config/status inputs:
  - `configReg.flags.imu_enabled`, `configReg.flags.imu_vertical_swap_enabled`, `configReg.imu_tip_threshold` read in `imu_task()`.
  - `statusReg.flags.imu_active` read as task gate in `imu_task()`.
- LED compositor inputs:
  - `configReg.status_led_brightness`, `configReg.flags.led_board_status_enabled`, `configReg.telephone_operating_mode`.
  - `statusReg.flags.system_booting`, `wifi_server_started`, `wifi_update_in_progress`, `modem_no_sim`, `modem_first_registration`, `modem_registered`, `incoming_call_ringing`, `off_hook_active`, `call_volte_active`, `sms_command_processed`.
  - `generalFlags.force_led_update`.
- TFT mode inputs:
  - loop-state transition context in `src/main.cpp` and `statusReg.flags.tft_status` checks before `tft_message(...)` calls.

## Outputs and side effects
- IMU side effects:
  - I2C register access via `imu.i2cRead(...)` / `imu.i2cWrite(...)` wrappers (`imu_read`, `imu_write`).
  - Writes `statusReg.imu_pitch`, `statusReg.imu_roll`, and tilt flags (`statusReg.flags.imu_tilt_exceeded`, `statusReg.flags.imu_tilt_reset`, related logtext flags).
- LED side effects:
  - LED buffer writes (`leds[position] = CRGB(...)`) and device flush via `FastLED.show()`.
  - Runtime writes `statusReg.flags.sms_command_processed = false` and `generalFlags.force_led_update = false` after consumption.
- TFT side effects:
  - Display init/clear/draw calls (`tft.init`, `tft.setRotation`, `tft.fillScreen`, `tft.pushImage`, sprite font/text operations).
  - BMP asset rendering through `drawBmp(...)` with `SPIFFS.open(filename, "r")` (for `/logo.bmp`).

## Owned state
- IMU persistent state (`src/src/imu_handler.cpp`):
  - `static uint32_t imu_tick`, `float x_gf`, `float y_gf`, `float z_gf`, `LIS3DH imu`, `static uint8_t imu_count`, `static uint8_t imu_reset_count`, `static bool imu_tilt_exceeded`.
- LED persistent state (`src/src/leddisplay_handler.cpp`):
  - `CRGB leds[MAX_LEDS]`, `flash_tick`, `status_flash_timer`, `status_flash_onoff`, `led_board_led1_flash_timer`, `led_board_led1_flash_onoff`, `led_board_led2_flash_timer`, `led_board_led2_flash_onoff`, `status_flash_sms`, `led_board_led1_sms`.
- TFT persistent state (`src/src/tft_handler.cpp`):
  - `TFT_eSPI tft`, `TFT_eSprite spr`.
- Hub fields written by this module:
  - `statusReg.imu_pitch`, `statusReg.imu_roll`, `statusReg.flags.imu_tilt_exceeded`, `statusReg.flags.imu_tilt_reset`, `statusReg.flags.imu_tilt_logtext_done`, `statusReg.flags.imu_tilt_reset_logtext_done`.
  - `statusReg.flags.led_display_active`, `statusReg.flags.sms_command_processed` (clear after consume).
  - `statusReg.flags.tft_status` written in `tft_init()` (set `true`) and `tft_deinit()` (set `false`); `tft_message()` does not write this flag.
  - `generalFlags.force_led_update` (clear after consume).

## Timing and triggering
- `imu_task()`:
  - cadence gate: `TickGet() - imu_tick >= TICK_SECOND` in loop context.
- `leddisplay_task()`:
  - cadence gate: `TickGet() - flash_tick >= (TICK_MILLIS * 10)` in loop context.
  - flashing cadence then uses countdown timers (`status_flash_timer`, `led_board_led1_flash_timer`, `led_board_led2_flash_timer`) with constants (`NORMAL_FLASH_PERIOD`, `FAST_FLASH_PERIOD`, `ON_FLASH_PERIOD`, `LONG_ON_FLASH_PERIOD`).
- TFT path:
  - no periodic gate proven in module; `tft_init()/tft_logo()/tft_message()/tft_deinit()` are invoked from loop mode transitions.
  - `tft_task()` currently has empty body and no repository call site.

## Concurrency and ordering rules
- Execution confinement:
  - module entry points with proven call sites execute from `loop` context.
- Cross-context dependency surface:
  - LED compositor reads shared flags produced by other contexts (modem task and WiFi/event/http flows), including modem and WiFi status flags.
- Synchronisation evidence:
  - no `xSemaphore`, mutex, critical-section, or atomic operations are present in `imu_handler.cpp`, `leddisplay_handler.cpp`, or `tft_handler.cpp`.
- Ordering implications:
  - LED/TFT behavior depends on latest shared flag values at each loop iteration; producer-consumer interleaving across contexts is not explicitly synchronised in this module.

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Timebase | call | `src/src/imu_handler.cpp` `imu_task` uses `TickGet()` gate `(TickGet()-imu_tick >= TICK_SECOND)`; `src/src/leddisplay_handler.cpp` `leddisplay_task` uses `TickGet()` gate `(TickGet()-flash_tick >= TICK_MILLIS*10)` |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| WiFi/Web Control Plane | shared-state | `src/src/wifi_handler.cpp` writes `statusReg.flags.wifi_server_started` (`wifi_start_ap`/`wifi_stop_ap`) and `statusReg.flags.wifi_update_in_progress` (`handleDoUpdate`); `src/src/leddisplay_handler.cpp` branches on both fields |
| Modem Core | shared-state | `src/src/modem_handler.cpp` updates `statusReg.flags.modem_registered`, `modem_no_sim`, `sms_command_processed`, `call_volte_active`; `src/src/leddisplay_handler.cpp` reads these for LED state decisions |
| Config Store | shared-state | `src/src/file_handler.cpp` `config_read/config_default` populate `configReg.status_led_brightness`, `configReg.flags.led_board_status_enabled`, `configReg.flags.imu_enabled`, `configReg.imu_tip_threshold`; consumed in `leddisplay_handler.cpp` and `imu_handler.cpp` |
| System Orchestrator | call | `src/main.cpp` `loop()` directly calls `leddisplay_task()` and `imu_task()`; loop state-machine branches call `tft_init()`, `tft_logo()`, and `tft_message()` |

## Evidence sources
- `src/main.cpp` (`loop`, `INITIAL_GO`, `IDLE_SLEEP`, and mode-transition call sites for IMU/LED/TFT functions).
- `src/src/imu_handler.cpp` (`imu_initialise`, `imu_task`, `imu_read`, `imu_write`).
- `src/src/leddisplay_handler.cpp` (`leddisplay_init`, `leddisplay_task`, `leddisplay_show`, timer/static state).
- `src/src/tft_handler.cpp` (`tft_init`, `tft_logo`, `tft_message`, `tft_deinit`, `tft_task`, `drawBmp`).
- `src/src/file_handler.cpp` (`config_read`, `config_default` persisted UI/IMU config fields).
- `src/src/wifi_handler.cpp` (`wifi_start_ap`, `wifi_stop_ap`, `handleDoUpdate` flag writes).
- `src/src/modem_handler.cpp` (modem status flag writes consumed by LED compositor).
- `docs/analysis/13_system_recomposition.md`.
- `docs/analysis/13_dependency_edges.csv`.

## Uncertainties to verify
- `tft_task()` scheduling remains UNCERTAIN because no call site for `tft_task()` is present in `src/`.
- Callback/thread context of external producers that set LED-consumed flags (WiFi callbacks/HTTP handlers) is UNCERTAIN in repository-local code.
