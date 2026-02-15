# A) Entry points
- `setup()` — defined in `src/main.cpp:139`
- `loop()` — defined in `src/main.cpp:201`
- `app_main()` — not found in provided files

Top-level init functions directly called from entry points:
- From `setup()` (`src/main.cpp:139`): `timerBegin()`, `timerAttachInterrupt()`, `timerAlarmWrite()`, `timerAlarmEnable()`
- From `loop()` (`src/main.cpp:201`), direct first-level init/start calls include: `adc_init()`, `button_init()`, `config_init()`, `config_default()`, `config_read()`, `gpio_initialise()`, `power_init()`, `leddisplay_init()`, `imu_initialise()`, `pwm_initialise()`, `es8388_audio_init()`, `tft_init()`, `xTaskCreate()` (for `modem_thread`, `audio_thread`), `ds4420_audio_init()`, `wifi_start_ap()`, `wifi_stop_ap()`

# B) Task and interrupt inventory

| Context Type | Name | Entry function | Defined in file | Created or registered in file | Trigger (periodic, event, unknown) | Notes |
|---|---|---|---|---|---|---|
| setup | Arduino setup | `setup()` | `src/main.cpp` | Arduino runtime (not shown in provided files) | event | Entry point function |
| loop | Arduino loop | `loop()` | `src/main.cpp` | Arduino runtime (not shown in provided files) | periodic | Entry point function |
| task | ModemThread | `modem_thread(void *param)` | `src/main.cpp` | `src/main.cpp:378` via `xTaskCreate(... modem_thread ...)` | periodic | Infinite loop with `vTaskDelay(10)` |
| task | AudioThread | `audio_thread(void *param)` | `src/main.cpp` | `src/main.cpp:381` via `xTaskCreate(... audio_thread ...)` | periodic | Infinite loop with `vTaskDelay(1)` |
| timer | HW timer alarm (timer0) | `SysTimer()` | `src/main.cpp` | `src/main.cpp:172-175`, `src/main.cpp:314-317` (`timerBegin` + `timerAttachInterrupt` + alarm enable) | periodic | Alarm configured to `1000` ticks |
| ISR | Timer ISR | `SysTimer()` (`IRAM_ATTR`) | `src/main.cpp` | `src/main.cpp:173`, `src/main.cpp:315` via `timerAttachInterrupt(..., &SysTimer, ...)` | event | ISR attribute present |
| callback | WiFi AP STA connected | `wifi_connect_event(WiFiEvent_t)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:618` via `WiFi.onEvent(...)` | event | Event id `ARDUINO_EVENT_WIFI_AP_STACONNECTED` |
| callback | WiFi AP STA disconnected | `wifi_disconnect_event(WiFiEvent_t)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:619` via `WiFi.onEvent(...)` | event | Event id `ARDUINO_EVENT_WIFI_AP_STADISCONNECTED` |
| callback | HTTP route `/values.xml` | `handleValuesXML(AsyncWebServerRequest*)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:640` via `server.on(...)` | event | HTTP request callback |
| callback | HTTP route `/setup.xml` | `handleSetupXML(AsyncWebServerRequest*)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:641` via `server.on(...)` | event | HTTP request callback |
| callback | HTTP route `/config.xml` | `handleConfigXML(AsyncWebServerRequest*)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:642` via `server.on(...)` | event | HTTP request callback |
| callback | HTTP route `/permissions.xml` | `handlePermissionsXML(AsyncWebServerRequest*)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:643` via `server.on(...)` | event | HTTP request callback |
| callback | HTTP route `/config.json` (GET) | `handleConfigJSON(AsyncWebServerRequest*)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:644` via `server.on(...)` | event | HTTP request callback |
| callback | HTTP route `/login` (GET) | `lambda@src/src/wifi_handler.cpp:645` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:645` via `server.on(...)` | event | Inline lambda |
| callback | HTTP route `/login` (POST) | `lambda@src/src/wifi_handler.cpp:659` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:659` via `server.on(...)` | event | Inline lambda |
| callback | HTTP route `/logout` (GET) | `lambda@src/src/wifi_handler.cpp:700` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:700` via `server.on(...)` | event | Inline lambda |
| callback | HTTP route `/upload` (POST complete) | `lambda@src/src/wifi_handler.cpp:711` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:711` via `server.on(...)` | event | Completion callback |
| callback | HTTP route `/upload` (POST body) | `handle_upload(...)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:715` via `server.on(..., handle_upload)` | event | Upload body callback |
| callback | HTTP route `/config` (POST complete) | `lambda@src/src/wifi_handler.cpp:718` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:717` via `server.on(...)` | event | Completion callback |
| callback | HTTP route `/config` (POST body) | `handleDoUpdate(...)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:758` via upload lambda | event | OTA upload body callback |
| callback | request disconnect callback | `lambda@src/src/wifi_handler.cpp:739` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:739` via `request->onDisconnect(...)` | event | Registered inside `/config` POST handler |
| callback | HTTP route `/config_upload` (POST complete) | `lambda@src/src/wifi_handler.cpp:761` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:760` via `server.on(...)` | event | Completion callback |
| callback | HTTP route `/config_upload` (POST body) | `handleConfigUpload(...)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:771` via upload lambda | event | JSON config upload callback |
| callback | HTTP not-found route | `handle_web_requests(AsyncWebServerRequest*)` | `src/src/wifi_handler.cpp` | `src/src/wifi_handler.cpp:774` via `server.onNotFound(...)` | event | Fallback HTTP handler |
| callback | Modem app callback bridge | `Thingstream_Application_modemCallback(const char*, uint16_t)` | `src/src/modem_handler.cpp` | unknown | event | Function present; registration not shown in provided files |
| callback | Modem text callback | `modem_callback(void*, const char*, uint16_t)` | `src/src/modem_handler.cpp` | `src/src/modem_handler.cpp:4265` direct call from `Thingstream_Application_modemCallback` | event | Static callback implementation |
| callback | Serial transport callback slot | `ThingstreamTransportCallback_t` (user-provided function pointer) | unknown | `src/src/serial_transport.cpp:101` via `serial_register_callback(...)` | event | Invoked at `src/src/serial_transport.cpp:226` in `serial_run()` |

# C) Fan out map

Entry points:
- `setup :: setup -> [setCpuFrequencyMhz] (unknown_file)`
- `setup :: setup -> [Serial.begin] (unknown_file)`
- `setup :: setup -> [xSemaphoreCreateBinary] (unknown_file)`
- `setup :: setup -> [timerBegin] (unknown_file)`
- `setup :: setup -> [timerAttachInterrupt] (unknown_file)`
- `setup :: setup -> [timerAlarmWrite] (unknown_file)`
- `setup :: setup -> [timerAlarmEnable] (unknown_file)`

- `loop :: loop -> [power_task] (unknown_file)`
- `loop :: loop -> [adc_task] (unknown_file)`
- `loop :: loop -> [leddisplay_task] (unknown_file)`
- `loop :: loop -> [gpio_task] (unknown_file)`
- `loop :: loop -> [imu_task] (unknown_file)`
- `loop :: loop -> [button_task] (unknown_file)`
- `loop :: loop -> [config_write] (unknown_file)`
- `loop :: loop -> [adc_init] (unknown_file)`
- `loop :: loop -> [button_init] (unknown_file)`
- `loop :: loop -> [config_init] (unknown_file)`
- `loop :: loop -> [config_default] (unknown_file)`
- `loop :: loop -> [config_read] (unknown_file)`
- `loop :: loop -> [gpio_initialise] (unknown_file)`
- `loop :: loop -> [power_init] (unknown_file)`
- `loop :: loop -> [leddisplay_init] (unknown_file)`
- `loop :: loop -> [imu_initialise] (unknown_file)`
- `loop :: loop -> [gpio_update] (unknown_file)`
- `loop :: loop -> [pwm_initialise] (unknown_file)`
- `loop :: loop -> [es8388_audio_init] (unknown_file)`
- `loop :: loop -> [tft_init] (unknown_file)`
- `loop :: loop -> [tft_logo] (unknown_file)`
- `loop :: loop -> [xTaskCreate] (unknown_file)`
- `loop :: loop -> [ds4420_audio_init] (unknown_file)`
- `loop :: loop -> [audio_set_volume] (unknown_file)`
- `loop :: loop -> [audio_playback] (unknown_file)`
- `loop :: loop -> [log_task] (src/main.cpp)`
- `loop :: loop -> [wifi_start_ap] (src/src/wifi_handler.cpp)`
- `loop :: loop -> [wifi_check_client_timeouts] (src/src/wifi_handler.cpp)`
- `loop :: loop -> [wifi_stop_ap] (src/src/wifi_handler.cpp)`

Tasks:
- `task :: audio_thread -> [audio_init] (unknown_file)`
- `task :: audio_thread -> [audio_task] (unknown_file)`
- `task :: audio_thread -> [vTaskDelay] (unknown_file)`

- `task :: modem_thread -> [modem_task] (src/src/modem_handler.cpp)`
- `task :: modem_thread -> [vTaskDelay] (unknown_file)`

Timer / ISR:
- `timer :: SysTimer -> [portENTER_CRITICAL_ISR] (unknown_file)`
- `timer :: SysTimer -> [portEXIT_CRITICAL_ISR] (unknown_file)`
- `ISR :: SysTimer -> [portENTER_CRITICAL_ISR] (unknown_file)`
- `ISR :: SysTimer -> [portEXIT_CRITICAL_ISR] (unknown_file)`

Callbacks:
- `callback :: wifi_connect_event -> [WiFi.softAPgetStationNum] (unknown_file)`
- `callback :: wifi_connect_event -> [esp_wifi_ap_get_sta_list] (unknown_file)`
- `callback :: wifi_connect_event -> [wifi_update_client_activity] (src/src/wifi_handler.cpp)`

- `callback :: wifi_disconnect_event -> [wifi_check_client_timeouts] (src/src/wifi_handler.cpp)`
- `callback :: wifi_disconnect_event -> [WiFi.softAPgetStationNum] (unknown_file)`
- `callback :: wifi_disconnect_event -> [esp_wifi_ap_get_sta_list] (unknown_file)`

- `callback :: handleValuesXML -> [track_request_activity] (src/src/wifi_handler.cpp)`
- `callback :: handleValuesXML -> [append_tag] (src/src/wifi_handler.cpp)`
- `callback :: handleValuesXML -> [make_no_store_response] (src/src/wifi_handler.cpp)`

- `callback :: handleSetupXML -> [track_request_activity] (src/src/wifi_handler.cpp)`
- `callback :: handleSetupXML -> [append_tag] (src/src/wifi_handler.cpp)`
- `callback :: handleSetupXML -> [make_no_store_response] (src/src/wifi_handler.cpp)`

- `callback :: handleConfigXML -> [track_request_activity] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigXML -> [append_tag] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigXML -> [xml_escape] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigXML -> [make_no_store_response] (src/src/wifi_handler.cpp)`

- `callback :: handlePermissionsXML -> [track_request_activity] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [ensure_authenticated] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [slot_enabled] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [get_slot_role] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [get_slot_username] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [is_slot_locked] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [slot_lock_remaining] (src/src/wifi_handler.cpp)`
- `callback :: handlePermissionsXML -> [make_no_store_response] (src/src/wifi_handler.cpp)`

- `callback :: handleConfigJSON -> [track_request_activity] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigJSON -> [ensure_authenticated] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigJSON -> [build_config_json] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigJSON -> [product_mode_to_name] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigJSON -> [attach_session_cookie] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigJSON -> [serializeJson] (unknown_file)`

- `callback :: lambda@/login GET -> [session_cookie_valid] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/login GET -> [attach_session_cookie] (src/src/wifi_handler.cpp)`

- `callback :: lambda@/login POST -> [is_login_locked] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/login POST -> [login_lock_remaining] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/login POST -> [authenticate_credentials] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/login POST -> [start_session] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/login POST -> [attach_session_cookie] (src/src/wifi_handler.cpp)`

- `callback :: lambda@/logout GET -> [force_logout] (src/src/wifi_handler.cpp)`

- `callback :: lambda@/upload POST complete -> [ensure_authenticated] (src/src/wifi_handler.cpp)`

- `callback :: handle_upload -> [ensure_authenticated] (src/src/wifi_handler.cpp)`
- `callback :: handle_upload -> [SPIFFS.exists] (unknown_file)`
- `callback :: handle_upload -> [SPIFFS.remove] (unknown_file)`
- `callback :: handle_upload -> [SPIFFS.open] (unknown_file)`

- `callback :: lambda@/config POST complete -> [ensure_authenticated] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/config POST complete -> [attach_session_cookie] (src/src/wifi_handler.cpp)`
- `callback :: lambda@/config POST complete -> [request->onDisconnect] (unknown_file)`

- `callback :: lambda@request->onDisconnect -> [wifi_restart_device] (src/src/wifi_handler.cpp)`

- `callback :: handleDoUpdate -> [ensure_authenticated] (src/src/wifi_handler.cpp)`
- `callback :: handleDoUpdate -> [attach_session_cookie] (src/src/wifi_handler.cpp)`
- `callback :: handleDoUpdate -> [esp_ota_get_next_update_partition] (unknown_file)`
- `callback :: handleDoUpdate -> [esp_partition_find_first] (unknown_file)`
- `callback :: handleDoUpdate -> [Update.begin] (unknown_file)`
- `callback :: handleDoUpdate -> [Update.write] (unknown_file)`
- `callback :: handleDoUpdate -> [Update.end] (unknown_file)`

- `callback :: lambda@/config_upload POST complete -> [ensure_authenticated] (src/src/wifi_handler.cpp)`

- `callback :: handleConfigUpload -> [ensure_authenticated] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigUpload -> [deserializeJson] (unknown_file)`
- `callback :: handleConfigUpload -> [apply_config_from_json] (src/src/wifi_handler.cpp)`
- `callback :: handleConfigUpload -> [config_write] (unknown_file)`
- `callback :: handleConfigUpload -> [serializeJson] (unknown_file)`
- `callback :: handleConfigUpload -> [make_no_store_response] (src/src/wifi_handler.cpp)`

- `callback :: handle_web_requests -> [track_request_activity] (src/src/wifi_handler.cpp)`
- `callback :: handle_web_requests -> [session_cookie_valid] (src/src/wifi_handler.cpp)`
- `callback :: handle_web_requests -> [attach_session_cookie] (src/src/wifi_handler.cpp)`
- `callback :: handle_web_requests -> [redirect_to_login] (src/src/wifi_handler.cpp)`
- `callback :: handle_web_requests -> [handle_command_requests] (src/src/wifi_handler.cpp)`
- `callback :: handle_web_requests -> [load_file] (src/src/wifi_handler.cpp)`

- `callback :: Thingstream_Application_modemCallback -> [modem_callback] (src/src/modem_handler.cpp)`

- `callback :: modem_callback -> [modem_decode] (src/src/modem_handler.cpp)`

- `callback :: serial_run -> [state->callback] (unknown_file)`

# D) Lifecycle phases (evidence based)

Explicit phase/state sets with code evidence:
- `run_process` enum (`src/inc/dac_defines.h:758-765`): `PREINIT_GO`, `PREINIT`, `INITIAL_GO`, `INITIAL`, `RUN_GO`, `RUN`
  - Driven in: `loop()` switch on `run_process` (`src/main.cpp:229`)
- `statusReg.telephone_mode` enum (`src/inc/dac_defines.h:768-785`): `REGISTRATION_GO`, `REGISTRATION`, `ONHOOK_GO`, `IDLE_SLEEP`, `IDLE_RUN`, `HANDSET_GO`, `HANDSET`, `HANDSFREE_GO`, `HANDSFREE`, `ANSWER_GO`, `ANSWER`, `AUDIO_TEST_GO`, `AUDIO_TEST`, `FTP_GO`, `FTP`, `AUDIO_TEST_ABORT`
  - Driven in: `loop()` switch on `statusReg.telephone_mode` (`src/main.cpp:724`)
- `statusReg.telephone_sub_mode` enum (`src/inc/dac_defines.h:788-801`): `ANNOUNCE_PLAY`, `OFF_HOOK`, `AUDIO_PLAY`, `AUTO_DIAL`, `MANUAL_DIAL`, `CALL_INITIALISATION`, `CALL_ANSWER`, `CALL_CONTINUATION`, `HANG_UP`, `NO_NETWORK`, `NO_NETWORK_WITH_HANGUP`, `ABORT`
  - Driven in: `submode_task()` switch on `statusReg.telephone_sub_mode` (`src/main.cpp:2094`)
- `FTP` sub-process enum (`src/inc/dac_defines.h:823-830`): `FTP_AWAIT_CONNECT`, `FTP_AWAIT_DIRECTORY`, `FTP_AWAIT_DOWNLOAD`, `FTP_AWAIT_UPDATE_SIZE`, `FTP_AWAIT_UPDATE_INFO`, `FTP_AWAIT_UPDATE_DOWNLOAD`, `FTP_AWAIT_DISCONNECT`
  - Driven in: `loop()` nested switch on `ftp_process` (`src/main.cpp:1697`)
- `modem_process` state values (`MDM_*`) switched in `modem_task()` (`src/src/modem_handler.cpp:261`)
  - Driven in: task context `ModemThread` via `modem_thread -> modem_task` (`src/main.cpp:2993`)

Execution contexts by file (observed):
- `src/main.cpp`: `setup`, `loop`, `task audio_thread`, `task modem_thread`, `timer callback`, `ISR`
- `src/src/wifi_handler.cpp`: `loop` (via `wifi_start_ap()/wifi_stop_ap()/wifi_check_client_timeouts()` calls from `loop`), `callback` (WiFi events + HTTP handlers + lambdas)
- `src/src/modem_handler.cpp`: `task modem_thread` (via `modem_task()`), `callback` (`Thingstream_Application_modemCallback`, `modem_callback`)
- `src/src/serial_transport.cpp`: `callback` (registered callback function pointer invoked in `serial_run()`), `unknown` (caller context for `serial_run()` not shown in provided files)
- `src/src/dac_defines.h`: `unknown` (definitions/declarations only)

# E) Open questions
- Provided file list included `src/src/dac_defines.h`, but in this repository the file is `src/inc/dac_defines.h`. This analysis used `src/inc/dac_defines.h`.
- The Thingstream modem stack is built by composing transports in `src/src/modem_handler.cpp`: `Thingstream_createRingBufferTransport(...)` (`src/src/modem_handler.cpp:4251`), `Thingstream_createModemLogger(...)` (`src/src/modem_handler.cpp:4255`), `Thingstream_createModem2Transport(...)` (`src/src/modem_handler.cpp:4257`).
- `src/src/serial_transport.cpp` implements the transport vtable, including `serial_register_callback(...)` (`src/src/serial_transport.cpp:101`) and callback dispatch in `serial_run(...)` via `state->callback(...)` (`src/src/serial_transport.cpp:226`).
- Callback path evidence in provided files: transport callback dispatch (`serial_run -> state->callback`) and modem callback bridge (`Thingstream_Application_modemCallback(...) -> modem_callback(...)` at `src/src/modem_handler.cpp:4263-4265`).
- Some callees in fan-out are declared/used here but their definitions are outside the provided file set; those are marked `unknown_file`.
