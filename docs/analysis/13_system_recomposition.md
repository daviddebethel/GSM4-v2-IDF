## 1) Canonical responsibility index

| ID | Name | Source slice clusters |
|---|---|---|
| CR01 | Audio thread orchestration and codec/PGA control | 12_sliceA_pass6.md C1 |
| CR02 | DTMF tone playback engine (audio thread side) | 12_sliceA_pass6.md C2 |
| CR03 | Audio test measurement state machine | 12_sliceA_pass6.md C3 |
| CR04 | In-call DTMF command decode and actuator control | 12_sliceA_pass6.md C4 |
| CR05 | Local audio file decode and MP3 DSP backend | 12_sliceA_pass6.md C5 |
| CR06 | Modem control loop and service dispatch | 12_sliceB_pass6.md C1 |
| CR07 | Modem inbound callback and decode pipeline | 12_sliceB_pass6.md C2 |
| CR08 | Thingstream serial transport adapter | 12_sliceB_pass6.md C3 |
| CR09 | Common parsing/normalization primitives | 12_sliceB_pass6.md C4 |
| CR10 | Config persistence and defaults backend | 12_sliceC_pass6.md C1 |
| CR11 | WiFi AP lifecycle and client presence tracking | 12_sliceC_pass6.md C2 |
| CR12 | HTTP access control and command mutation plane | 12_sliceC_pass6.md C3 |
| CR13 | XML/JSON/config transfer and OTA handlers | 12_sliceC_pass6.md C4 |
| CR14 | Tick timebase primitives | 12_sliceD_pass6.md C1 |
| CR15 | Power + external voltage telemetry | 12_sliceD_pass6.md C2 |
| CR16 | Physical input scanning and IO control | 12_sliceD_pass6.md C3 |
| CR17 | IMU orientation and tilt state | 12_sliceD_pass6.md C4 |
| CR18 | LED status board compositor | 12_sliceD_pass6.md C5 |
| CR19 | TFT screen bring-up and asset rendering | 12_sliceD_pass6.md C6 |

## 2) Candidate modules

| Module | Owner context | Responsibilities included |
|---|---|---|
| Audio Runtime | multi-owner (task: audio, loop) | CR01, CR02, CR03, CR05 |
| In-call DTMF Control | unknown | CR04 |
| Modem Core | multi-owner (task: modem, Thingstream callback) | CR06, CR07, CR09 |
| Serial Transport | unknown | CR08 |
| Config Store | multi-owner (loop, http route handler) | CR10 |
| WiFi/Web Control Plane | multi-owner (loop, wifi event callback, http route handler) | CR11, CR12, CR13 |
| System Orchestrator | setup + loop + ISR registration | startup/init, task creation, ISR/timer registration, top-level sequencing in `src/main.cpp` |
| Timebase | unknown | CR14 |
| Power and Physical IO | loop | CR15, CR16 |
| Sensor and Visual UI | multi-owner (loop, unknown) | CR17, CR18, CR19 |

## 2.1 Module source mapping

| Module | Source file mapping |
|---|---|
| System Orchestrator | `src/main.cpp` |

## 3) Module details

### Audio Runtime
- Owned state (persistent): `audio_intialised`, `Audio audio`, `es8388_audio`, `ds4420_audio*`, DTMF runtime (`dtmf_pending_digit`, `dtmf_request_stop`, `dtmf_file`), audio-test runtime (`audio_test_process`, `statusReg.flags.audio_test_*`), MP3 decoder globals (`m_MP3DecInfo`, `m_FrameHeader`) [12_sliceA_pass6.md C1/C2/C3/C5]
- Inputs: `audio_task()` from audio thread and loop-level wrappers `audio_playback()`/`audio_set_volume()`; shared test/control fields `generalFlags.audio_test_*`, `statusReg.audio_test_tone_tick` [12_sliceA_pass6.md C1/C3]
- Outputs / side effects: I2S/file/decoder pipeline (`i2s_write`, `i2s_read`, `SPIFFS.open`, `MP3Decode`), codec/PGA programming (`es8388_*`, `ds4420_set_gain`), test result flags (`statusReg.flags.audio_test_passed/audio_test_complete`) [12_sliceA_pass6.md C1/C2/C3/C5]
- Timing/triggering: Audio-thread cycle `audio_task -> audio.loop + dtmf_task + audio_test_task`; DTMF/test gates use `TickGet()` windows [12_sliceA_pass6.md C1/C2/C3]
- Concurrency/ordering risks (evidence-only): Cross-context request fields (`dtmf_pending_digit`, `dtmf_request_stop`, `generalFlags.audio_test_*`) are consumed in audio thread and set elsewhere [12_sliceA_pass6.md C2/C3]

### In-call DTMF Control
- Owned state (persistent): Decoder/program state (`digit_compare`, `digit_program`, `digit_pointer`, `digit_command`, `dtmf_tick`) and consumed modem fields (`statusReg.modem_response`, `statusReg.modem_dtmf_digit`) [12_sliceA_pass6.md C4]
- Inputs: `dtmf_decode_task()` with DTMF event marker `statusReg.modem_response == ATRQTONEDET` and PIN data `configReg.PIN` [12_sliceA_pass6.md C4]
- Outputs / side effects: PGA/gain calls (`ds4420_mute`, `ds4420_set_gain`), load control (`gpio_controls.load_control`, `gpio_update()`), and command flags (`generalFlags.load_activate_request`) [12_sliceA_pass6.md C4]
- Timing/triggering: Runs when caller invokes `dtmf_decode_task()`; timeout logic uses `TickGet()` and `DTMF_TIMEOUT_PERIOD` [12_sliceA_pass6.md C4]
- Concurrency/ordering risks (evidence-only): `statusReg.modem_response` is consumed and reset in this flow while modem response is produced elsewhere [12_sliceA_pass6.md C4]

### Modem Core
- Owned state (persistent): `modem_process`, `modem_tick`, receive buffers (`sms_receive_buffer`, `ftp_receive_buffer`), response/status fields (`statusReg.modem_response`, `statusReg.modem_signal_level`, `statusReg.flags.modem_*`), request flags (`generalFlags.call_*`, `generalFlags.ftp_*`, `generalFlags.audio_test_*`), CRC state `CRCHold` [12_sliceB_pass6.md C1/C2/C4]
- Inputs: `modem_task()` from modem thread, inbound callback payloads (`Thingstream_Application_modemCallback`, `modem_callback`), and shared request bits from other modules [12_sliceB_pass6.md C1/C2]
- Outputs / side effects: AT/control dispatch (`modem_send`, `call_*`, `sms_task/sms_decode`, `modem_ftp_*`), decode writes to shared status/flags, parser outputs (`getValue`, `normalize_phone_number`) [12_sliceB_pass6.md C1/C2/C4]
- Timing/triggering: Modem thread loop with `vTaskDelay(10)` plus event callbacks for inbound frames [12_sliceB_pass6.md C1/C2]
- Concurrency/ordering risks (evidence-only): Task path and callback path both mutate shared `statusReg`/`generalFlags` response fields; ordering is callback-arrival dependent [12_sliceB_pass6.md C1/C2]

### Serial Transport
- Owned state (persistent): Static `_transport_state` (`buffer`, `callback`, `callback_cookie`, `serial`, `overflowed`, `lineBased`, `timeStamp`) [12_sliceB_pass6.md C3]
- Inputs: Thingstream vtable calls (`serial_init`, `serial_send`, `serial_register_callback`, `serial_run`) [12_sliceB_pass6.md C3]
- Outputs / side effects: UART read/write and callback dispatch (`state->callback(...)`) to modem callback pipeline [12_sliceB_pass6.md C3]
- Timing/triggering: `serial_run(self, millis)` polls until timeout/newline/overflow conditions [12_sliceB_pass6.md C3]
- Concurrency/ordering risks (evidence-only): Registration and run paths share mutable callback pointer without visible synchronization [12_sliceB_pass6.md C3]

### Config Store
- Owned state (persistent): `/config.ini` plus full `configReg.*` snapshot and `generalFlags.config_changed` [12_sliceC_pass6.md C1]
- Inputs: `config_init/config_default/config_read` from loop; `config_write` from upload/config command flows [12_sliceC_pass6.md C1]
- Outputs / side effects: SPIFFS persistence (`SPIFFS.open/read/write`) and global config/auth/whitelist/log fields [12_sliceC_pass6.md C1]
- Timing/triggering: Loop startup/init path and HTTP-triggered save path (`handleConfigUpload -> config_write`) [12_sliceC_pass6.md C1/C4]
- Concurrency/ordering risks (evidence-only): `configReg.*` is written from both loop and HTTP flows; ordering across contexts is UNCERTAIN [12_sliceC_pass6.md C1]

### WiFi/Web Control Plane
- Owned state (persistent): WiFi/AP state (`statusReg.flags.wifi_server_*`, `tracked_clients`, `no_users_connected`), auth/session (`web_level`, `web_user`, `session_cookie_value`, lockout arrays), upload/update state (`statusReg.flags.wifi_update_in_progress`, upload sessions) [12_sliceC_pass6.md C2/C3/C4]
- Inputs: Loop calls (`wifi_start_ap/wifi_stop_ap/wifi_check_client_timeouts`), WiFi events (`wifi_connect_event/wifi_disconnect_event`), HTTP routes (`/login`, `/config`, `/config_upload`, `/values.xml`, etc.) [12_sliceC_pass6.md C2/C3/C4]
- Outputs / side effects: Route registration/responses, config/control mutations (`configReg.*`, `generalFlags.*`, `gpio_controls.*`), OTA/file operations, external calls (`power_set_chargerate`, `modem_sim_switch_guard_start`) [12_sliceC_pass6.md C3/C4]
- Timing/triggering: Loop periodic AP lifecycle + event callbacks + HTTP/upload callback execution [12_sliceC_pass6.md C2/C3/C4]
- Concurrency/ordering risks (evidence-only): Shared `statusReg/configReg/generalFlags` mutated across loop, event callbacks, and HTTP callbacks [12_sliceC_pass6.md C2/C3/C4]

### System Orchestrator
- Owned state (persistent): Top-level runtime orchestration state in `src/main.cpp` (`run_process`, `run_process_tick`, `config_tick`, `wake_tick`, `general_timer`) and timer ownership variables (`hw_timer_t *timer`, `portMUX_TYPE timerMux`, `volatile uint32_t upTime`) [src/main.cpp:44-47, src/main.cpp:56-59, src/main.cpp:67, src/main.cpp:99-107]
- Inputs: Arduino framework entry points `setup()` and `loop()` [src/main.cpp:139, src/main.cpp:201]
- Outputs / side effects: Hardware/service initialization in setup, thread creation (`xTaskCreate` modem/audio), and timebase ISR registration (`timerAttachInterrupt(timer, &SysTimer, true)`) [src/main.cpp:139-179, src/main.cpp:378-381, src/main.cpp:173, src/main.cpp:315]
- Timing/triggering: `setup()` one-time startup sequence; `loop()` continuous top-level sequencing and mode switch dispatch [src/main.cpp:139-179, src/main.cpp:201-229]
- Concurrency/ordering risks (evidence-only): `loop()` invokes `Platform_jumpTick(rtc_dif)` while timer ISR `SysTimer` also writes `upTime`; explicit exclusion at jump callsite is not shown in this file [src/main.cpp:100-106, src/main.cpp:850-867]

### Timebase
- Owned state (persistent): `upTime` and tick utility accessors (`Platform_getTimeMillis`, `TickGet`) [12_sliceD_pass6.md C1]
- Inputs: Timer/ISR writer is documented as inventory only; direct writer linkage is UNCERTAIN [12_sliceD_pass6.md C1]
- Outputs / side effects: Monotonic time values to loop-driven handlers (`TickGet()`) [12_sliceD_pass6.md C1]
- Timing/triggering: Referenced by periodic task gates in power/gpio/imu/led flows [12_sliceD_pass6.md C1]
- Concurrency/ordering risks (evidence-only): Writer context for `upTime` is UNCERTAIN in provided slices [12_sliceD_pass6.md C1]

### Power and Physical IO
- Owned state (persistent): Power FSM state (`power_process`, `power_tick`), external/battery measurements (`statusReg.external_voltage`, `statusReg.vbus_voltage`, `statusReg.battery_voltage`), GPIO control/state (`gpio_controls`, debounce timers, `statusReg.flags.hook/door/button/io*`) [12_sliceD_pass6.md C2/C3]
- Inputs: Loop fan-out task/init calls (`power_task`, `adc_task`, `button_task`, `gpio_task`, `gpio_update`) with request flags (`generalFlags.power_status_request`, `generalFlags.external_voltage_request`) [12_sliceD_pass6.md C2/C3]
- Outputs / side effects: Hardware control (BQ25622, ADC, GPIO expanders, PWM), control/request flags (`generalFlags.modem_reboot_request`, `generalFlags.wifi_request`, `generalFlags.gpio_update_request`), config-reset path (`config_default`, `config_write`) [12_sliceD_pass6.md C2/C3]
- Timing/triggering: Loop periodic execution with `TickGet` interval checks and 5s power cadence [12_sliceD_pass6.md C2/C3]
- Concurrency/ordering risks (evidence-only): Shared flags are set/cleared across power and ADC paths and across input handlers; call order effects are UNCERTAIN [12_sliceD_pass6.md C2/C3]

### Sensor and Visual UI
- Owned state (persistent): IMU filters/counters (`x_gf/y_gf/z_gf`, `imu_count`), LED compositing timers (`status_flash_timer`, `led_board_led*`), display runtime (`statusReg.flags.tft_status`, `TFT_eSPI tft`, `TFT_eSprite spr`) [12_sliceD_pass6.md C4/C5/C6]
- Inputs: Loop calls (`imu_task`, `leddisplay_task`, `tft_init`, `tft_logo`) and shared status/config fields (`statusReg.flags.*`, `configReg.status_led_brightness`) [12_sliceD_pass6.md C4/C5/C6]
- Outputs / side effects: Sensor I2C reads (`imu_read`), LED output (`FastLED.show`), TFT rendering/SPIFFS asset reads (`drawBmp`, `SPIFFS.open`) [12_sliceD_pass6.md C4/C5/C6]
- Timing/triggering: IMU at `TICK_SECOND`; LED compositor at 10ms; TFT init on loop startup; runtime `tft_task` trigger UNCERTAIN [12_sliceD_pass6.md C4/C5/C6]
- Concurrency/ordering risks (evidence-only): Consumes shared modem/wifi/power/call flags produced by other contexts; `tft_task` scheduling is UNCERTAIN [12_sliceD_pass6.md C5/C6]

## 4) Cross-module dependency edges

| Module From | Module To | Interaction type | Evidence |
|---|---|---|---|
| System Orchestrator | Timebase | call | `src/main.cpp` (System Orchestrator) calls `TickGet()` repeatedly in `loop()` (e.g., `src/main.cpp:215`, `src/main.cpp:320`, `src/main.cpp:869`); no direct `Platform_getTimeMillis()` call found in `src/main.cpp` via `rg`. |
| Timebase | Power and Physical IO | call | 12_sliceD_pass6.md C1 `TickGet` -> 12_sliceD_pass6.md C2 `power_task` and C3 `button_task`/`gpio_task` |
| Timebase | Sensor and Visual UI | call | 12_sliceD_pass6.md C1 `TickGet` -> 12_sliceD_pass6.md C4 `imu_task` and C5 `leddisplay_task` |
| Power and Physical IO | Config Store | call | 12_sliceD_pass6.md C3 `button_task` calls `config_default` and `config_write`; 12_sliceC_pass6.md C1 owns `config_default/config_write` |
| WiFi/Web Control Plane | Config Store | call | 12_sliceC_pass6.md C4 `handleConfigUpload` calls `config_write`; 12_sliceC_pass6.md C3 command path calls `config_default` |
| Config Store | WiFi/Web Control Plane | shared-state | 12_sliceC_pass6.md C1 `configReg.*` baseline consumed by C2 `wifi_start_ap` and C3/C4 auth+config handlers |
| WiFi/Web Control Plane | Power and Physical IO | call | 12_sliceC_pass6.md C3 `handle_command_requests` calls `power_set_chargerate`; 12_sliceD_pass6.md C2 defines charger control path |
| WiFi/Web Control Plane | Power and Physical IO | shared-state | 12_sliceC_pass6.md C3 mutates `gpio_controls.*` and `generalFlags.gpio_update_request`; 12_sliceD_pass6.md C3 owns `gpio_update`/GPIO control state |
| WiFi/Web Control Plane | Modem Core | call | 12_sliceC_pass6.md C3 calls `modem_sim_switch_guard_start`; 12_sliceB_pass6.md C1 dispatches modem control requests |
| Power and Physical IO | Modem Core | shared-state | 12_sliceD_pass6.md C3 sets `generalFlags.modem_reboot_request`; 12_sliceB_pass6.md C1 consumes modem request flags in `MDM_RUN` |
| WiFi/Web Control Plane | Sensor and Visual UI | shared-state | 12_sliceC_pass6.md C2/C4 write `statusReg.flags.wifi_server_started` and `statusReg.flags.wifi_update_in_progress`; 12_sliceD_pass6.md C5 consumes both in `leddisplay_task` |
| Modem Core | Sensor and Visual UI | shared-state | 12_sliceB_pass6.md C1/C2 write modem status (`statusReg.flags.modem_registered/modem_no_sim`); 12_sliceD_pass6.md C5 branches on those flags |
| Modem Core | WiFi/Web Control Plane | shared-state | 12_sliceB_pass6.md C2 writes `statusReg.modem_signal_level` and registration flags; 12_sliceC_pass6.md C4 `handleValuesXML` emits these fields |
| Config Store | Audio Runtime | shared-state | 12_sliceC_pass6.md C1 persists `configReg` audio fields; 12_sliceA_pass6.md C1/C3 consume config-driven audio gain/test settings |
| WiFi/Web Control Plane | Audio Runtime | shared-state | 12_sliceC_pass6.md C3 `handle_command_requests` mutates `configReg.handset_modem_*`, `configReg.mimic_*`; 12_sliceA_pass6.md C1/C3 use runtime audio config |
| Modem Core | Audio Runtime | call | 12_sliceB_pass6.md C1 dispatch includes `call_audio_setup` and `call_send_dtmf`; 12_sliceA_pass6.md C1/C2 provide audio/DTMF runtime behavior |
| Audio Runtime | Modem Core | shared-state | 12_sliceA_pass6.md C3 coordinates via `generalFlags.audio_test_request/audio_test_tone_start_request` and `statusReg.audio_test_tone_tick`; 12_sliceB_pass6.md C1 dispatches `generalFlags.audio_test_*` |
| In-call DTMF Control | Audio Runtime | call | 12_sliceA_pass6.md C4 `dtmf_decode_task` calls `ds4420_mute` and `ds4420_set_gain`; 12_sliceA_pass6.md C1 owns PGA control wrappers |
| In-call DTMF Control | Power and Physical IO | call | 12_sliceA_pass6.md C4 writes `gpio_controls.load_control` then calls `gpio_update`; 12_sliceD_pass6.md C3 defines `gpio_update` |
| Serial Transport | Modem Core | event | 12_sliceB_pass6.md C3 `serial_run` invokes registered callback `state->callback(...)`; 12_sliceB_pass6.md C2 callback pipeline handles inbound modem payloads |
| Modem Core | Serial Transport | call | 12_sliceB_pass6.md C3 evidence: transport created in modem init via `serial_transport_create(&hwSerial, ...)` |
| Modem Core | Config Store | shared-state | 12_sliceB_pass6.md C4 notes normalization writes caller-owned `configReg.voice_no*`/`configReg.sms_user*`; 12_sliceC_pass6.md C1 persists `configReg.*` |
| Config Store | Power and Physical IO | shared-state | 12_sliceC_pass6.md C1 persists `configReg.battery_low_level` and `configReg.battery_chargerate`; 12_sliceD_pass6.md C2 reads both in `power_task`/`power_set_config` |
| Config Store | Sensor and Visual UI | shared-state | 12_sliceC_pass6.md C1 persists `configReg.status_led_brightness` and `configReg.flags.led_board_status_enabled`; 12_sliceD_pass6.md C5 uses both in LED init/task |
