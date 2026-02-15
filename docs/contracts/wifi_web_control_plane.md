# WiFi/Web Control Plane
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: run AP lifecycle, HTTP auth/control endpoints, XML/JSON/config transfer, and OTA upload flow implemented in `src/src/wifi_handler.cpp`.
- Non goals: None stated in code.

## Owner execution context
- Owner contexts (state mutation observed):
  - `loop`: `src/main.cpp:loop()` calls `wifi_start_ap()`, `wifi_check_client_timeouts()`, `wifi_stop_ap()`.
  - `wifi event callback`: `wifi_connect_event()` and `wifi_disconnect_event()` registered by `WiFi.onEvent(...)` in `wifi_start_ap()`.
  - `http route handler`: `server.on(...)` route handlers and upload body callbacks (`handle_upload`, `handleDoUpdate`, `handleConfigUpload`).
- External invocation contexts:
  - `System Orchestrator` (`src/main.cpp`) invokes exported WiFi entry points from loop.
  - No direct ISR entry points into this module found via repository search.

## Entry points
- Loop-called API entry points:
  - `void wifi_start_ap(void)` called from `src/main.cpp:loop()`.
  - `void wifi_check_client_timeouts(void)` called from `src/main.cpp:loop()` (and from `wifi_disconnect_event`).
  - `void wifi_stop_ap(void)` called from `src/main.cpp:loop()`.
- WiFi event callbacks:
  - `wifi_connect_event(WiFiEvent_t)` via `WiFi.onEvent(..., ARDUINO_EVENT_WIFI_AP_STACONNECTED)`.
  - `wifi_disconnect_event(WiFiEvent_t)` via `WiFi.onEvent(..., ARDUINO_EVENT_WIFI_AP_STADISCONNECTED)`.
- HTTP routes (`AsyncWebServer server(80)`):
  - `/values.xml` -> `handleValuesXML`
  - `/setup.xml` -> `handleSetupXML`
  - `/config.xml` -> `handleConfigXML`
  - `/permissions.xml` -> `handlePermissionsXML`
  - `/config.json` (`HTTP_GET`) -> `handleConfigJSON`
  - `/login` (`HTTP_GET`) -> lambda
  - `/login` (`HTTP_POST`) -> lambda
  - `/logout` (`HTTP_GET`) -> lambda
  - `/upload` (`HTTP_POST`) -> lambda + `handle_upload`
  - `/config` (`HTTP_POST`) -> lambda + `handleDoUpdate`
  - `/config_upload` (`HTTP_POST`) -> lambda + `handleConfigUpload`
  - catch-all: `server.onNotFound(handle_web_requests)`
- Not found as external entry point:
  - `wifi_task()` is defined but no external call sites found via `rg`.

## Inputs
- Calls/events in:
  - loop invocation of `wifi_start_ap/wifi_check_client_timeouts/wifi_stop_ap` from `src/main.cpp`.
  - WiFi AP station events into `wifi_connect_event/wifi_disconnect_event`.
  - HTTP request callbacks for login/config/upload/xml/json handlers.
- Data in (read examples):
  - `configReg.flags.location_text_setup`, `configReg.phone_loc` in `wifi_start_ap` SSID build path.
  - `configReg.*` and `statusReg.*` fields in `handleValuesXML`, `handleConfigXML`, `handlePermissionsXML`, `handleConfigJSON`.
  - request parameters via `request->arg(...)` / `request->getParam(...)` in `handle_command_requests` and auth handlers.

## Outputs and side effects
- Hub/shared-state writes (examples):
  - `statusReg.flags.wifi_server_started/connected/disconnected/wifi_update_in_progress` in AP/event/update flows.
  - `generalFlags.config_changed`, `generalFlags.gpio_update_request`, `generalFlags.force_led_update`, `generalFlags.modem_reboot_request`, `generalFlags.sim_switch_reboot_request`.
  - `gpio_controls.io1_control`, `gpio_controls.io2_control`, `gpio_controls.sim_select_control`.
  - broad `configReg.*` mutation from HTTP command/update handlers.
- File/network/update effects:
  - `SPIFFS.begin/end/open`, `request->send(...)`, route responses and redirects.
  - OTA path using `Update.begin/write/end/abort`.
  - restart trigger via `wifi_restart_device(...)` after update completion path.
- Cross-module call-outs:
  - `power_set_chargerate(configReg.battery_chargerate)` in `handle_command_requests`.
  - `modem_sim_switch_guard_start(...)` in `handle_command_requests`.
  - config persistence calls `config_write()` and reset call `config_default()`.

## Owned state
- Hub fields primarily written by this module (mechanical inventory):
  - Approximate unique write set in bus-access inventory for this file: `128` fields (`configReg`: 109, `statusReg`: 10, `generalFlags`: 6, `gpio_controls`: 3; TEST_ONLY excluded).
  - Sole-writer fields in inventory: `statusReg.flags.wifi_server_connected`, `statusReg.flags.wifi_server_disconnected`, `statusReg.flags.wifi_server_started`, `statusReg.flags.wifi_update_in_progress`, `configReg.flags.audio_path_test_enable`, `configReg.flags.imu_vertical_swap_enabled`, `configReg.flags.no_network_response_enabled`.
  - Top hub fields written (most cross-cutting):
    - `statusReg.flags.wifi_server_started`
    - `statusReg.flags.wifi_server_connected`
    - `statusReg.flags.wifi_server_disconnected`
    - `statusReg.flags.wifi_update_in_progress`
    - `generalFlags.config_changed`
    - `generalFlags.gpio_update_request`
    - `generalFlags.force_led_update`
    - `generalFlags.modem_reboot_request`
    - `gpio_controls.io1_control`
    - `gpio_controls.io2_control`
    - `gpio_controls.sim_select_control`
- Local non-bus state (`src/src/wifi_handler.cpp`):
  - AP/session/auth: `web_level`, `web_user`, `wifi_configured`, `session_cookie_value`, `session_generation`, `web_failures[]`, `web_lock_stage[]`, `web_lockout_until[]`, `login_lockout_until`, `unknown_*`.
  - Client tracking: `tracked_clients[]`, `last_client_check_ms`, `no_users_connected`, `wifi_station_list`.
  - Storage/runtime: `spiffs_mounted`, upload session temporary objects via `request->_tempObject`.
- Writer contexts for local state:
  - `loop` (`wifi_start_ap`, `wifi_check_client_timeouts`, `wifi_stop_ap`), `wifi event callback` (`wifi_connect_event`, `wifi_disconnect_event`), and `http route handler` paths (`track_request_activity`, auth/config/upload handlers).

## Timing and triggering
- Loop-driven maintenance:
  - `wifi_check_client_timeouts()` called from `src/main.cpp:loop()` when web server is active.
- Internal periodic gate:
  - `wifi_check_client_timeouts()` runs only when `(millis() - last_client_check_ms) >= CLIENT_CHECK_INTERVAL_MS` with `CLIENT_CHECK_INTERVAL_MS = 5000`.
- Event-driven:
  - AP connect/disconnect callbacks registered through `WiFi.onEvent(...)`.
- HTTP-driven:
  - route handlers execute per incoming request.
- Upload-driven:
  - `handleDoUpdate`/`handleConfigUpload` consume chunked upload callbacks (`index`, `len`, `final` flow).

## Concurrency and ordering rules
- Proven multi-context mutations:
  - `statusReg.flags.wifi_server_connected/disconnected` are written in loop path (`wifi_check_client_timeouts`), WiFi event callbacks (`wifi_connect_event`, `wifi_disconnect_event`), and loop stop path (`wifi_stop_ap`).
  - `generalFlags.config_changed` is written in HTTP handlers (`handle_command_requests`, `handleConfigUpload`) and WiFi event callback (`wifi_connect_event`); main loop also consumes/clears config-change flow.
  - client tracking state (`tracked_clients[]`, `no_users_connected`) is mutated in HTTP request activity tracking, periodic timeout checks, and WiFi connect/disconnect callbacks.
- Ordering notes:
  - `wifi_disconnect_event` explicitly calls `wifi_check_client_timeouts()` before final flag update.
  - OTA path toggles `statusReg.flags.wifi_update_in_progress` and `generalFlags.force_led_update` around begin/error/end branches.
- UNCERTAIN threading detail:
  - exact FreeRTOS task/thread used by `ESPAsyncWebServer` route callbacks and `WiFi.onEvent` callbacks is not proven from repository code alone.

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Config Store | call | `src/src/wifi_handler.cpp`: route `/config_upload` body handler `handleConfigUpload(...)` calls `config_write()`; `handle_command_requests(...)` action `config_default` calls `config_default()`. |
| Power and Physical IO | call | `src/src/wifi_handler.cpp`: `handle_command_requests(...)` action `setup_advan` calls `power_set_chargerate(configReg.battery_chargerate)`. |
| Power and Physical IO | shared-state | `src/src/wifi_handler.cpp`: `handle_command_requests(...)` actions `gpio_on`/`gpio_off`/`esim_apply_reset` write `gpio_controls.*` and set `generalFlags.gpio_update_request = true`. |
| Modem Core | call | `src/src/wifi_handler.cpp`: `handle_command_requests(...)` action `esim_apply_reset` calls `modem_sim_switch_guard_start(...)`. |
| Sensor and Visual UI | shared-state | `src/src/wifi_handler.cpp`: `wifi_start_ap()`/`wifi_stop_ap()` write `statusReg.flags.wifi_server_started`; OTA handler `handleDoUpdate(...)` writes `statusReg.flags.wifi_update_in_progress`. |
| Audio Runtime | shared-state | `src/src/wifi_handler.cpp`: `handle_command_requests(...)` action `setup_audio` writes `configReg.handset_modem_*`, `configReg.handsfree_modem_*`, `configReg.mimic_*`, `configReg.audo_path_test_period`. |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| Config Store | shared-state | `src/src/wifi_handler.cpp`: `wifi_start_ap()` reads `configReg.flags.location_text_setup` and `configReg.phone_loc`; `handleConfigXML(...)` and `handleValuesXML(...)` read persisted `configReg.*` fields. |
| Modem Core | shared-state | `src/src/wifi_handler.cpp`: `handleValuesXML(...)` reads `statusReg.modem_signal_level` and `statusReg.flags.modem_registered` for `/values.xml` response payload. |
| System Orchestrator | call | src/main.cpp `loop()` calls `wifi_start_ap()` and `wifi_check_client_timeouts()` and `wifi_stop_ap()` during RUN mode WiFi lifecycle |

## Evidence sources
- `src/src/wifi_handler.cpp`: `wifi_start_ap`, `wifi_stop_ap`, `wifi_check_client_timeouts`, `wifi_connect_event`, `wifi_disconnect_event`, `handle_command_requests`, `handleConfigUpload`, `handleDoUpdate`, `handleValuesXML`, `handleConfigXML`, `handlePermissionsXML`, `handleConfigJSON`.
- `src/main.cpp`: `loop()` call sites for `wifi_start_ap`, `wifi_check_client_timeouts`, `wifi_stop_ap`.
- `docs/refactor_analysis/agents/05_bus_access_union_fn_refined.md`: mechanical field read/write inventory for `src/src/wifi_handler.cpp` (TEST_ONLY excluded).
- `docs/analysis/13_dependency_edges.csv`: module dependency rows for incoming/outgoing edges.

## Uncertainties to verify
- Exact callback execution thread identity for `ESPAsyncWebServer` route handlers is not shown in repository code.
- Exact callback execution thread identity for `WiFi.onEvent` handlers (`wifi_connect_event`, `wifi_disconnect_event`) is not shown in repository code.
- No repository-local evidence of explicit synchronization primitives for shared WiFi/auth/client-tracking state across loop/event/http contexts.
