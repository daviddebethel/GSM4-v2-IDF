# Power and Physical IO
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: `CR15` power and voltage telemetry control (`power_handler` + `adc_handler`) and `CR16` physical input/output control (`button_handler` + `gpio_handler`).
- Purpose: expose loop-driven control functions for charger setup, GPIO output updates, button/door/keypad state detection, and telemetry flag updates.
- Non goals: None stated in code.

## Owner execution context
- Primary execution context(s):
  - `loop`: `src/main.cpp` directly calls `power_task()`, `adc_task()`, `gpio_task()`, `button_task()` each iteration.
- External invocation contexts:
  - `http route handler`: `src/src/wifi_handler.cpp:handle_command_requests()` calls `power_set_chargerate(...)`.
  - `task: modem`: `src/src/modem_handler.cpp` writes `gpio_controls.*` and sets `generalFlags.gpio_update_request`; `src/main.cpp` loop then applies via `gpio_update()`.

## Entry points
- Loop-driven periodic/service entry points:
  - `power_task()` (called from `src/main.cpp:loop`)
  - `adc_task()` (called from `src/main.cpp:loop`)
  - `button_task()` (called from `src/main.cpp:loop`)
  - `gpio_task()` (called from `src/main.cpp:loop`)
- Initialization entry points called from loop state machine:
  - `power_init()`
  - `adc_init()`
  - `button_init()`
  - `gpio_initialise()`
  - `pwm_initialise()`
- Externally callable control entry points used outside module files:
  - `gpio_update()` (called from `src/main.cpp`, `src/src/dtmf_handler.cpp`, `src/src/wifi_handler.cpp` request flow via `generalFlags.gpio_update_request` consumer)
  - `power_set_chargerate(uint16_t)` (called from `src/src/wifi_handler.cpp:handle_command_requests` action `setup_advan`)
  - `power_control_external_power(bool)` (called from `src/main.cpp` run-mode branches)
- GPIO output update patterns:
  - Preferred pattern: writer sets `gpio_controls.*` and sets `generalFlags.gpio_update_request`; `src/main.cpp` loop consumes request and calls `gpio_update()`.
  - Direct-call pattern exists: `src/src/dtmf_handler.cpp:dtmf_decode_task` calls `gpio_update()` directly.

## Inputs
- Request flags read:
  - `generalFlags.power_status_request` in `power_task()`.
  - `generalFlags.external_voltage_request` in `adc_task()`.
- Configuration fields read:
  - `configReg.battery_low_level` in `power_task()` low-battery checks.
  - `configReg.battery_chargerate` in `power_set_config()`.
  - `configReg.flags.charger_temperature_enable` in `power_set_config()`.
  - `configReg.telephone_operating_mode` and `configReg.flags.door_stuck_message_disable` in `gpio_task()`.
- Digital input sources:
  - `digitalRead(GPIO_WIFI_BOOT_BUT)` in `button_task()`.
  - `digitalRead(GPIO_GPIO_INT)` in `gpio_task()`.

## Outputs and side effects
- Hardware side effects:
  - Charger IC (`BQ25622`) register reads/writes in `power_task()`, `power_set_chargerate()`, `power_set_charge()`, `power_set_config()`.
  - ADC read path in `adc_task()` via `adc1_get_raw(...)` and `esp_adc_cal_raw_to_voltage(...)`.
  - GPIO expander output writes in `gpio_update()` via `gpio2.write_port0/1(...)`.
  - PWM output write in `gpio_set_display_brightness()` via `pwm.writePWM1(...)`.
- Hub/state writes (examples):
  - `generalFlags.external_voltage_request` set in `power_task()`.
  - `generalFlags.modem_reboot_request` and `generalFlags.wifi_request` set in `button_task()`.
  - `statusReg.external_voltage` written in `adc_task()`.
  - `statusReg.vbus_voltage`, `statusReg.battery_voltage`, `statusReg.flags.external_power`, `statusReg.flags.battery_low`, `statusReg.flags.power_task_complete` written in `power_task()`.
  - `statusReg.flags.hook_button_*`, `statusReg.flags.handsfree_button*_*(pressed/released/stuck)`, `statusReg.flags.door_*`, `statusReg.flags.io1_input_status`, `statusReg.flags.io2_input_status`, `statusReg.keypad_keypress` written in `gpio_task()`.
- Cross-module call-outs:
  - `config_default()` and `config_write()` called from `button_task()` on long WIFI button hold.

## Owned state
- Internal persistent locals:
  - `src/src/power_handler.cpp`: `power_tick`, `power_process`, `current`, `battery_current`, `last_battery_voltage`, `stat0`, `stat1`.
  - `src/src/adc_handler.cpp`: `voltage_raw`, `last_external_voltage`, `adc_chars`.
  - `src/src/button_handler.cpp`: `button_tick`, `wifi_button_hold_timer`, `wifi_held_*`, `wifi_released`.
  - `src/src/gpio_handler.cpp`: `gpio_tick`, keypad/button last-state variables, debounce/stuck timers, `GPIO_BUTTONS buttons`, `GPIO_MON_BUTTONS mon_buttons`, `GPIO_ROW keypad_row`, `GPIO_COLUMN keypad_column`.
- Hub fields primarily written by this module:
  - `generalFlags.power_status_request` (clear), `generalFlags.external_voltage_request`, `generalFlags.modem_reboot_request`, `generalFlags.wifi_request`.
  - `statusReg.external_voltage`, `statusReg.vbus_voltage`, `statusReg.battery_voltage`, `statusReg.flags.battery_low`, `statusReg.flags.external_power`, `statusReg.flags.power_task_complete`.
  - `statusReg.flags.door_open`, `statusReg.flags.door_stuck`, `statusReg.flags.hook_button_pressed`, `statusReg.flags.volume_button_up_pressed`, `statusReg.flags.io1_input_status`, `statusReg.flags.io2_input_status`, `statusReg.keypad_keypress`.
  - `gpio_controls.charge_enable`.
  - List is partial; authoritative cross file view is in bus union report.

## Timing and triggering
- `power_task()` trigger:
  - loop-called function; executes full status cycle when `generalFlags.power_status_request` is set or when `TickGet() - power_tick >= (TICK_SECOND * 5)`.
- `adc_task()` trigger:
  - loop-called function; executes only when `generalFlags.external_voltage_request` is true.
- `button_task()` trigger:
  - loop-called function; runs button handling when `TickGet() - button_tick >= (TICK_MILLIS * BUTTON_CHECK_PERIOD)`.
- `gpio_task()` trigger:
  - loop-called function; runs scan/debounce when `TickGet() - gpio_tick >= (TICK_MILLIS * 10u)`.

## Concurrency and ordering rules
- Primary mutation ownership is loop-driven for `power_task/adc_task/button_task/gpio_task` state.
- Cross-context shared flag ordering is present:
  - `generalFlags.gpio_update_request` is written in `src/src/wifi_handler.cpp` (HTTP handlers) and `src/src/modem_handler.cpp` (modem task), then consumed/cleared in `src/main.cpp:loop` before `gpio_update()`.
  - `generalFlags.modem_reboot_request` is written by `button_task()` and HTTP/modem code paths, then read/cleared in `src/src/modem_handler.cpp:modem_task()`.
  - `gpio_controls.io1_control/io2_control` are written in WiFi and modem flows; output application occurs when `gpio_update()` runs in loop or direct caller context.
- Protection evidence:
  - No `xSemaphore`, mutex, `portENTER_CRITICAL`, or atomic primitives were found in `power_handler.cpp`, `adc_handler.cpp`, `button_handler.cpp`, or `gpio_handler.cpp`.

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Config Store | call | `src/src/button_handler.cpp` `button_task` calls `config_default()` then `config_write()` when WIFI button hold reaches `WIFI_BUTTON_DEFAULT_PERIOD` |
| Modem Core | shared-state | `src/src/button_handler.cpp` `button_task` sets `generalFlags.modem_reboot_request`; `src/src/modem_handler.cpp` `modem_task` checks and clears it |
| Timebase | call | `TickGet()` gates in `src/src/power_handler.cpp:power_task`, `src/src/button_handler.cpp:button_task`, and `src/src/gpio_handler.cpp:gpio_task` |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| System Orchestrator | call | `src/main.cpp` `loop()` directly calls `power_task()`, `adc_task()`, `gpio_task()`, and `button_task()` |
| WiFi/Web Control Plane | call | `src/src/wifi_handler.cpp` `handle_command_requests(action="setup_advan")` calls `power_set_chargerate(configReg.battery_chargerate)` |
| WiFi/Web Control Plane | shared-state | `src/src/wifi_handler.cpp` writes `gpio_controls.io1_control/io2_control` and sets `generalFlags.gpio_update_request`; `src/main.cpp` loop applies via `gpio_update()` |
| In-call DTMF Control | call | `src/src/dtmf_handler.cpp` `dtmf_decode_task` writes `gpio_controls.load_control` and calls `gpio_update()` |
| Config Store | shared-state | `src/src/file_handler.cpp` `config_read/config_default` load `configReg.battery_low_level` and `configReg.battery_chargerate`; `src/src/power_handler.cpp` reads both |

## Evidence sources
- `src/main.cpp` (`loop`, `PREINIT/INITIAL/RUN` branches, `generalFlags.gpio_update_request` consumer)
- `src/src/power_handler.cpp` (`power_init`, `power_task`, `power_set_chargerate`, `power_control_external_power`, `power_set_config`)
- `src/src/adc_handler.cpp` (`adc_init`, `adc_task`)
- `src/src/button_handler.cpp` (`button_init`, `button_task`)
- `src/src/gpio_handler.cpp` (`gpio_task`, `gpio_update`, `gpio_initialise`, `pwm_initialise`)
- `src/src/wifi_handler.cpp` (`handle_command_requests`, `server.onNotFound(handle_web_requests)`)
- `src/src/modem_handler.cpp` (`modem_task` request-flag consumers/writers)
- `src/src/dtmf_handler.cpp` (`dtmf_decode_task`)
- `src/src/file_handler.cpp` (`config_read`, `config_default`, `config_write`)
- `docs/analysis/13_dependency_edges.csv`

## Uncertainties to verify
- `AsyncWebServer` handler thread model (same core/thread as loop vs separate task) is UNCERTAIN from repository code alone.
- No explicit synchronisation is visible for shared-field updates crossing `loop`, `task: modem`, and HTTP handler contexts; execution interleaving remains UNCERTAIN.
