# Modem Core
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: modem control loop and service dispatch (`modem_task`, `modem_run`), inbound decode/callback pipeline (`Thingstream_Application_modemCallback`, `modem_callback`, `modem_decode`), and modem-facing parsing/normalization helpers (`sms_decode`, `ftp_json_decode`, `normalize_phone_number` call sites in this module).
- Non goals: serial byte transport implementation (`serial_transport.cpp`), WiFi HTTP handling (`wifi_handler.cpp`), and system top-level orchestration (`main.cpp`).

## Owner execution context
- Owner context (primary state mutation):
  - `task: modem` (PROVEN for main control path): `src/main.cpp` creates `modem_thread` via `xTaskCreate` (`src/main.cpp:378`), `modem_thread` loops `modem_task()` + `vTaskDelay(10)` (`src/main.cpp:2986-2994`), and `modem_task()` drives `modem_run()` (`src/src/modem_handler.cpp:800`) and `modem_send()` (`src/src/modem_handler.cpp:4467`).
  - Callback mutation path exists: `Thingstream_Application_modemCallback()` forwards to `modem_callback()` (`src/src/modem_handler.cpp:4263-4266`), and `modem_callback()` mutates shared modem state (`src/src/modem_handler.cpp:4812-4837`).
- External invocation contexts (API callers, not primary ownership):
  - `loop`: `main.cpp` calls `modem_sleep()` (`src/main.cpp:830`), `modem_sms_busy()` (`src/main.cpp:981`, `src/main.cpp:1525`, `src/main.cpp:1534`), `modem_call_status_reset()` (`src/main.cpp:2371`, `src/main.cpp:2418`).
  - `http route handler`: `handle_command_requests()` calls `modem_sim_switch_guard_start()` (`src/src/wifi_handler.cpp:1462`, `src/src/wifi_handler.cpp:1890`).
  - `test harness`: `test_task()` calls `modem_wake()` (`src/src/test_handler.cpp:38`, `src/src/test_handler.cpp:59`, `src/src/test_handler.cpp:85`, `src/src/test_handler.cpp:104`).
  - `Thingstream callback` dispatch thread provenance is UNCERTAIN: callback contract is declared in `src/sdk/inc/modem_transport.h:98-115`, but vendor dispatch implementation is not present in this repository.

## Entry points
- Task path:
  - `void modem_task(void)` (`src/src/modem_handler.cpp:254`) called from `modem_thread()` (`src/main.cpp:2993`).
  - `void modem_run(void)` (`src/src/modem_handler.cpp:4434`) called from `modem_task()` (`src/src/modem_handler.cpp:800`).
- Callback path:
  - `void Thingstream_Application_modemCallback(const char* response, uint16_t len)` (`src/src/modem_handler.cpp:4263`).
  - `static void modem_callback(void* cookie, const char* data, uint16_t len)` (`src/src/modem_handler.cpp:4808`).
- External APIs used outside `modem_handler.cpp`:
  - `void modem_sleep(void)` (`src/main.cpp:830`)
  - `bool modem_sms_busy(void)` (`src/main.cpp:981`, `src/main.cpp:1525`, `src/main.cpp:1534`)
  - `void modem_call_status_reset(void)` (`src/main.cpp:2371`, `src/main.cpp:2418`)
  - `void modem_sim_switch_guard_start(bool, bool, const char*)` (`src/src/wifi_handler.cpp:1890`)
  - `void modem_wake(void)` (`src/src/test_handler.cpp:59`, `src/src/test_handler.cpp:85`, `src/src/test_handler.cpp:104`)

## Inputs
- Calls in:
  - From `System Orchestrator`: `modem_thread -> modem_task` (`src/main.cpp:2986-2994`).
  - From loop paths in `main.cpp`: `modem_sleep`, `modem_sms_busy`, `modem_call_status_reset`.
  - From WiFi HTTP route handling: `modem_sim_switch_guard_start` via `handle_command_requests`.
  - From test harness: `modem_wake`.
- Events/callbacks in:
  - `Thingstream_Application_modemCallback` on modem transport responses (`src/sdk/inc/modem_transport.h:98-115`, `src/src/modem_handler.cpp:4263-4266`).
- Data in (shared-state reads; mechanical):
  - 05 bus map (`TEST_ONLY` excluded) shows `src/src/modem_handler.cpp` reads or read-modify-writes hub fields across `configReg`, `statusReg`, `generalFlags`, `gpio_controls`.
  - Read-only examples used in code: `configReg.flags.gnss_enable` (`src/src/modem_handler.cpp:5364`, `src/src/modem_handler.cpp:5439`), `statusReg.flags.wifi_server_started` (`src/src/modem_handler.cpp:2175`, `src/src/modem_handler.cpp:2189`), `statusReg.flags.door_open` (`src/src/modem_handler.cpp:2915`).

## Outputs and side effects
- Calls out:
  - Transport run: `modem->run(modem, 1)` (`src/src/modem_handler.cpp:4438`).
  - AT send path: `Thingstream_Modem2_sendLine(...)` (`src/src/modem_handler.cpp:3441`, `src/src/modem_handler.cpp:4755`).
  - Transport construction: `serial_transport_create`, `Thingstream_createRingBufferTransport`, `Thingstream_createModem2Transport` (`src/src/modem_handler.cpp:4248-4257`).
- Shared-state writes (05 bus mechanical summary, `TEST_ONLY` excluded):
  - Total written hub fields by `src/src/modem_handler.cpp`: 146.
  - By hub: `configReg` 71, `statusReg` 43, `generalFlags` 29, `gpio_controls` 3.
  - Top cross-file written fields include `configReg.telephone_operating_mode`, `configReg.handset_PGA_speaker_gain`, `configReg.handsfree_PGA_speaker_gain`, `statusReg.flags.modem_registered`, `configReg.PIN`, `configReg.auto_answer_rings`, `configReg.auto_dial_time`, `configReg.auto_onhook_time`, `configReg.battery_chargerate`.
  - Concrete write examples:
    - `statusReg.flags.modem_registered` set/cleared in `modem_task` (`src/src/modem_handler.cpp:721`, `src/src/modem_handler.cpp:735`).
    - `configReg.PIN` in `sms_decode` and `ftp_json_decode` (`src/src/modem_handler.cpp:1667-1670`, `src/src/modem_handler.cpp:5690`).
    - `configReg.telephone_operating_mode` in `ftp_json_decode` (`src/src/modem_handler.cpp:5714`).
    - `configReg.auto_answer_rings` in `sms_decode` and `ftp_json_decode` (`src/src/modem_handler.cpp:1034`, `src/src/modem_handler.cpp:5978`).
    - `configReg.battery_chargerate` in `ftp_json_decode` (`src/src/modem_handler.cpp:6011`).
- External effects:
  - UART modem I/O via `hwSerial` and Thingstream transport chain (`src/src/modem_handler.cpp:4245-4257`).
  - System time updates via `settimeofday` in `modem_set_datatime`/`gps_set_datatime` (`src/src/modem_handler.cpp:5277`, `src/src/modem_handler.cpp:5642`).
  - OTA update write/finish path in modem decode flow (`src/src/modem_handler.cpp:5140-5145`).

## Owned state
- Non-bus persistent state in `src/src/modem_handler.cpp`:
  - Modem/gps/sms process state: `modem_process`, `modem_tick`, `modem_timer`, `sms_process`, `sms_tick`, `gps_process`, `gps_tick`, `gps_timer`, `gps_timeout_tick`, `gps_minute_timer`, `modem_registration_tick`.
  - Transport and receive buffers: `modemBuffer`, `ringBuffer`, `sms_send_buffer`, `sms_receive_buffer`, `ftp_receive_buffer`, `sms_receive_buffer_pointer`.
  - Callback/response state: `modem_response`, `current_command`, `tRes`, `qiic_query_*`, `tonevolume_query_*`.
  - SIM guard and SMS queue state: `sim_switch_guard`, `sms_event_queue`, `sms_msg_queue`, `sms_current_msg`, queue indices/counters.
  - JSON document: `StaticJsonDocument<8000u> json_doc`.
- Hub fields primarily written by Modem Core (sole-writer in 05 bus map): 16 fields.
  - `generalFlags.force_reset_request`, `generalFlags.ftp_file_request`, `generalFlags.incoming_ring_request`
  - `statusReg.flags.call_disconnected`, `statusReg.flags.incoming_sms_ready`, `statusReg.flags.io1_trigger_status`, `statusReg.flags.io2_logtext_done`, `statusReg.flags.io2_trigger_status`, `statusReg.flags.modem_dtr_status`, `statusReg.flags.modem_no_sim`, `statusReg.flags.sms_inprogress`, `statusReg.flags.timedate_set`
  - `statusReg.ftp_file_date`, `statusReg.ftp_file_size`, `statusReg.modem_rx_level`, `statusReg.modem_signal_level`

## Timing and triggering
- Task scheduler trigger:
  - `modem_thread` runs continuously with `vTaskDelay(10)` (`src/main.cpp:2989-2995`).
  - Each cycle calls `modem_task()` (`src/main.cpp:2993`).
- Modem transport polling:
  - `modem_task()` calls `modem_run()` (`src/src/modem_handler.cpp:800`) and `modem_run()` executes `modem->run(modem, 1)` (`src/src/modem_handler.cpp:4438`).
- State-machine driven timing:
  - `modem_task` uses `switch (modem_process)` and `TickGet()` thresholds (`src/src/modem_handler.cpp:261`, `src/src/modem_handler.cpp:266`, `src/src/modem_handler.cpp:304`, `src/src/modem_handler.cpp:547`, `src/src/modem_handler.cpp:553`).
  - `sms_task` retry/backoff and queue timing use `TickGet()` (`src/src/modem_handler.cpp:3292`, `src/src/modem_handler.cpp:3336`, `src/src/modem_handler.cpp:3401`).
  - `gps_task` periodic/timeout checks use `TickGet()` (`src/src/modem_handler.cpp:5365`, `src/src/modem_handler.cpp:5385`, `src/src/modem_handler.cpp:5406`).

## Concurrency and ordering rules
- Cross-context access is present:
  - Task path (`modem_task`) and callback path (`modem_callback`) both write shared flags/state (`src/src/modem_handler.cpp:721`, `src/src/modem_handler.cpp:735`, `src/src/modem_handler.cpp:4812-4837`).
  - Loop/http/test paths call modem APIs concurrently with task scheduling (`src/main.cpp:830`, `src/main.cpp:981`, `src/src/wifi_handler.cpp:1890`, `src/src/test_handler.cpp:59`).
- Callback behavior constraints (SDK contract):
  - Callback is invoked on modem transport responses including during `Thingstream_Modem2_sendLine()`; SDK explicitly states not to call `Thingstream_xxx()` from inside callback (`src/sdk/inc/modem_transport.h:98-109`).
- Callback thread provenance assessment:
  - Visible chain segments: `modem_thread -> modem_task -> modem_run -> modem->run(modem, 1)` (`src/main.cpp:2986-2994`, `src/src/modem_handler.cpp:4434-4438`) and serial transport callback invocation inside `serial_run()` (`src/src/serial_transport.cpp:136-137`, `src/src/serial_transport.cpp:224-227`).
  - Opaque segment: `Thingstream_createModem2Transport()` and `Thingstream_Modem2_sendLine()` implementations are vendor/SDK functions not present in this repository (`src/src/modem_handler.cpp:4257`, `src/src/modem_handler.cpp:4755`), so callback thread provenance to `Thingstream_Application_modemCallback()` cannot be fully proven from repo code alone.
- Protection primitives:
  - No `xSemaphore`, mutex, `portENTER_CRITICAL`, `portEXIT_CRITICAL`, `atomic`, or `__atomic` usage found in `src/src/modem_handler.cpp` and `src/src/serial_transport.cpp` for shared modem state paths (repository grep evidence).

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Sensor and Visual UI | shared-state | 12_sliceB_pass6.md C1/C2 write modem status (`statusReg.flags.modem_registered/modem_no_sim`); 12_sliceD_pass6.md C5 branches on those flags |
| WiFi/Web Control Plane | shared-state | 12_sliceB_pass6.md C2 writes `statusReg.modem_signal_level` and registration flags; 12_sliceC_pass6.md C4 `handleValuesXML` emits these fields |
| Audio Runtime | call | 12_sliceB_pass6.md C1 dispatch includes `call_audio_setup` and `call_send_dtmf`; 12_sliceA_pass6.md C1/C2 provide audio/DTMF runtime behavior |
| Serial Transport | call | 12_sliceB_pass6.md C3 evidence: transport created in modem init via `serial_transport_create(&hwSerial, ...)` |
| Config Store | shared-state | 12_sliceB_pass6.md C4 notes normalization writes caller-owned `configReg.voice_no*`/`configReg.sms_user*`; 12_sliceC_pass6.md C1 persists `configReg.*` |
| Timebase | call | `src/src/modem_handler.cpp` calls `TickGet()` in `modem_task`, `sms_task`, and `gps_task` timing gates |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| System Orchestrator | call | `src/main.cpp` creates `modem_thread` via `xTaskCreate` and `modem_thread()` calls `modem_task()` in its loop |
| WiFi/Web Control Plane | call | 12_sliceC_pass6.md C3 calls `modem_sim_switch_guard_start`; 12_sliceB_pass6.md C1 dispatches modem control requests |
| Power and Physical IO | shared-state | 12_sliceD_pass6.md C3 sets `generalFlags.modem_reboot_request`; 12_sliceB_pass6.md C1 consumes modem request flags in `MDM_RUN` |
| Audio Runtime | shared-state | 12_sliceA_pass6.md C3 coordinates via `generalFlags.audio_test_request/audio_test_tone_start_request` and `statusReg.audio_test_tone_tick`; 12_sliceB_pass6.md C1 dispatches `generalFlags.audio_test_*` |
| Serial Transport | event | 12_sliceB_pass6.md C3 `serial_run` invokes registered callback `state->callback(...)`; 12_sliceB_pass6.md C2 callback pipeline handles inbound modem payloads |

## Uncertainties to verify
- Callback thread provenance to `Thingstream_Application_modemCallback()` is UNCERTAIN: dispatch/registration implementation in `Thingstream_createModem2Transport()` and `Thingstream_Modem2_sendLine()` is not present in repository source.
- Whether callback execution can overlap `modem_task` state mutations is UNCERTAIN because callback thread provenance is unproven and no repository-visible lock/critical-section policy exists for shared fields touched by both paths.

## Evidence sources
- `src/main.cpp` (`xTaskCreate` modem thread creation, `modem_thread`, loop call sites for modem APIs).
- `src/src/modem_handler.cpp` (`modem_task`, `modem_run`, `modem_init`, `Thingstream_Application_modemCallback`, `modem_callback`, `modem_decode`, `sms_task`, `gps_task`, `ftp_json_decode`, static state declarations).
- `src/src/wifi_handler.cpp` (`handle_web_requests`, `handle_command_requests`, HTTP path calling `modem_sim_switch_guard_start`).
- `src/src/test_handler.cpp` (`test_task` calls `modem_wake`).
- `src/src/serial_transport.cpp` (callback dispatch in `serial_run`, transport state and callback storage).
- `src/inc/modem_handler.h` (public Modem Core API surface).
- `src/sdk/inc/modem_transport.h` (Thingstream callback contract and callback constraints).
- `docs/refactor_analysis/agents/03_entry_points.md` (entry-point and context inventory).
- `docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md` (call-coupling evidence including `main.cpp -> modem_handler.cpp`, `modem_handler.cpp -> tick.cpp`, `modem_handler.cpp -> serial_transport.cpp`).
- `docs/refactor_analysis/agents/05_bus_access_union_fn_refined.md` (mechanical read/write field ownership and execution-context labels, `TEST_ONLY` excluded).
- `docs/modules/serial_transport.md` (trusted v1 Serial Transport ownership and run-path provenance).
