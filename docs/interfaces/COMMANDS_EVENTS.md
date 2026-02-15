# Commands and Events
## Naming conventions
- Commands use: `CMD_<DOMAIN>_<VERB>_<NOUN>`.
- Events use: `EVT_<DOMAIN>_<NOUN>_<STATE/CHANGE>`.
- Allowed domains: `WEB`, `CFG`, `PWR`, `IO`, `MODEM`, `AUD`, `UI`, `DTMF`, `SYS`.
- Commands represent side-effect requests with a single owning consumer.
- Events represent state changes and telemetry publication from a single owning producer.
- Snapshot fields are read-mostly state exposed for status polling.
## Command definitions
| Command | Producer | Consumer (single owner) | Payload fields | Ack/Response | Trigger evidence |
|---|---|---|---|---|---|
| CMD_SYS_BOOT_LOAD_CONFIG | SYS | CFG (config_task) | boot_reason: enum, force_default: bool | deferred | Config Store v1: PREINIT_GO `config_init/config_read/config_default/config_write` path |
| CMD_SYS_BOOT_INIT_IO | SYS | IO (io_task) | init_phase: enum | immediate | 03_entry_points: loop startup invokes `power_init/adc_init/button_init/gpio_initialise/pwm_initialise` |
| CMD_SYS_BOOT_INIT_AUDIO | SYS | AUD (audio_task) | init_codec: bool, init_pga: bool | immediate | 03_entry_points + Audio Runtime v1: `xTaskCreate(audio_thread)` then `audio_init` |
| CMD_SYS_BOOT_INIT_MODEM | SYS | MODEM (modem_task) | cold_start: bool | immediate | 03_entry_points + Modem Core v1: `xTaskCreate(modem_thread)` then `modem_task` |
| CMD_SYS_BOOT_INIT_UI | SYS | UI (ui_task) | show_logo: bool | immediate | 03_entry_points + Sensor/UI v1: `leddisplay_init`, `imu_initialise`, `tft_init`, `tft_logo` |
| CMD_SYS_WEB_AP_START | SYS | WEB (web_task) | ap_enable: bool | immediate | WiFi/Web v1: loop calls `wifi_start_ap()` |
| CMD_SYS_WEB_AP_STOP | SYS | WEB (web_task) | reason: enum | immediate | WiFi/Web v1: loop calls `wifi_stop_ap()` |
| CMD_SYS_WEB_CLIENT_TIMEOUT_CHECK | SYS | WEB (web_task) | now_ms: uint32 | none | WiFi/Web v1: loop/event calls `wifi_check_client_timeouts()` |
| CMD_SYS_AUD_PLAYBACK_START | SYS | AUD (audio_task) | file_path: string, volume: uint8 | immediate | Audio Runtime v1: loop calls `audio_playback` + `audio_set_volume` |
| CMD_SYS_AUD_PLAYBACK_STOP | SYS | AUD (audio_task) | reason: enum | immediate | Audio Runtime v1: loop/test calls `audio_playback_stop()` |
| CMD_SYS_AUD_SET_VOLUME | SYS | AUD (audio_task) | volume: uint8 | immediate | Audio Runtime v1: loop/test calls `audio_set_volume()` |
| CMD_SYS_AUD_DTMF_START | SYS | AUD (audio_task) | digit: char | immediate | Audio Runtime v1: loop/test calls `audio_dtmf()` |
| CMD_SYS_AUD_DTMF_STOP | SYS | AUD (audio_task) | reason: enum | immediate | Audio Runtime v1: loop/test calls `audio_dtmf_stop()` |
| CMD_SYS_AUD_TEST_START | SYS | AUD (audio_task) | request_id: uint32 | deferred | Audio Runtime v1: `generalFlags.audio_test_request` consumed by `audio_test_task` |
| CMD_SYS_AUD_TEST_ABORT | SYS | AUD (audio_task) | request_id: uint32 | deferred | Audio Runtime v1: `generalFlags.audio_test_abort_request` consumed by `audio_test_task` |
| CMD_WEB_CFG_WRITE | WEB | CFG (config_task) | source: enum(route_upload|route_config), changed_fields: string[] | deferred | WiFi/Web v1: `/config_upload` `handleConfigUpload` and `/config` update path call `config_write` |
| CMD_WEB_CFG_DEFAULT | WEB | CFG (config_task) | source: enum(route_command) | deferred | WiFi/Web v1: `handle_command_requests` action `config_default` |
| CMD_WEB_PWR_SET_CHARGERATE | WEB | IO (io_task) | battery_chargerate: uint16 | immediate | WiFi/Web v1 + edges: `handle_command_requests(action=setup_advan)` -> `power_set_chargerate` |
| CMD_WEB_IO_SET_OUTPUTS | WEB | IO (io_task) | io1_control: bool, io2_control: bool, sim_select_control: bool | immediate | WiFi/Web v1 + edges: gpio command actions write `gpio_controls.*` and request update |
| CMD_WEB_MODEM_SIM_SWITCH_GUARD_START | WEB | MODEM (modem_task) | esim_enable: bool, reboot_after: bool, reason: string | deferred | WiFi/Web v1 + edges: `handle_command_requests` calls `modem_sim_switch_guard_start` |
| CMD_WEB_AUDIO_SET_PARAMS | WEB | AUD (audio_task) | mimic_volume: uint8, handset_modem_speaker_volume: uint8, handsfree_modem_speaker_volume: uint8 | status-poll | WiFi/Web v1 + edges: web handlers mutate audio-related `configReg.*` consumed by audio/main paths |
| CMD_IO_CFG_DEFAULT_AND_WRITE | IO | CFG (config_task) | reason: enum(wifi_button_hold), hold_ms: uint32 | deferred | Power/IO v1 + edges: `button_task` long-hold calls `config_default` then `config_write` |
| CMD_IO_MODEM_REBOOT_REQUEST | IO | MODEM (modem_task) | reason: enum(button|other) | deferred | Power/IO v1 + edges: `button_task` sets `generalFlags.modem_reboot_request`, modem consumes |
| CMD_MODEM_CFG_WRITE | MODEM | CFG (config_task) | reason: enum(sms_apply_esim) | deferred | Config Store v1 + edges: `sms_apply_esim` calls `config_write` |
| CMD_MODEM_DTMF_DIGIT_RX | MODEM | DTMF (dtmf_owner) | modem_response: enum, modem_dtmf_digit: char | immediate | In-call DTMF v1 + Modem Core v1: `ATRQTONEDET` + digit consumed by `dtmf_decode_task` |
| CMD_DTMF_IO_SET_LOAD_OUTPUT | DTMF | IO (io_task) | load_control: bool, apply_now: bool | immediate | In-call DTMF v1 + edges: `dtmf_decode_task` writes `gpio_controls.load_control` and calls `gpio_update` |
| CMD_AUD_MODEM_AUDIO_TEST_TONE_REQ | AUD | MODEM (modem_task) | start_request: bool | deferred | Audio Runtime v1 + Modem Core v1: `audio_test_tone_start_request` produced in audio, consumed in modem |
| CMD_SYS_UI_TFT_SET_STATE | SYS | UI (ui_task) | action: enum(init|logo|deinit) | immediate | Sensor/UI v1: loop calls `tft_init`, `tft_logo`, `tft_deinit` |
| CMD_SYS_UI_TFT_MESSAGE_SHOW | SYS | UI (ui_task) | message: string | immediate | Sensor/UI v1: loop mode transitions call `tft_message(...)` |
## Event definitions
| Event | Producer (single owner) | Subscribers | Payload fields | Delivery | Evidence |
|---|---|---|---|---|---|
| EVT_SYS_BOOT_PHASE_READY | SYS (boot_coordinator) | WEB, CFG, IO, MODEM, AUD, UI | phase: enum | event-group-bit | 03_entry_points + system recomposition: startup phase transitions |
| EVT_SYS_TICK_WINDOW | SYS (timebase adapter) | CFG, IO, MODEM, AUD, UI | tick_ms: uint32 | event-group-bit | Timebase v1 + dependency edges: all major modules consume TickGet timing |
| EVT_SYS_TIME_JUMP_APPLIED | SYS (timebase adapter) | IO, MODEM, AUD, UI | jump_ms: uint32 | queue | Timebase v1: `Platform_jumpTick(rtc_dif)` wake compensation |
| EVT_CFG_STORE_LOADED | CFG (config_task) | SYS, WEB, MODEM, AUD, IO, UI | source: enum(file|default|recovery), marker: uint32 | queue | Config Store v1: boot/read/default recovery paths |
| EVT_CFG_STORE_WRITTEN | CFG (config_task) | SYS, WEB | path: string, marker: uint32 | queue | Config Store v1: `config_write()` from loop/http/modem/button triggers |
| EVT_WEB_AP_SERVER_STARTED | WEB (web_task) | SYS, UI | wifi_server_started: bool | event-group-bit | WiFi/Web v1: `wifi_start_ap`/`wifi_stop_ap` write `statusReg.flags.wifi_server_started` |
| EVT_WEB_AP_CLIENT_CONNECTED | WEB (web_task) | UI, SYS | station_count: uint8 | queue | WiFi/Web v1: `wifi_connect_event` |
| EVT_WEB_AP_CLIENT_DISCONNECTED | WEB (web_task) | UI, SYS | station_count: uint8 | queue | WiFi/Web v1: `wifi_disconnect_event` |
| EVT_WEB_UPDATE_PROGRESS_CHANGE | WEB (web_task) | UI, SYS | wifi_update_in_progress: bool | event-group-bit | WiFi/Web v1: `handleDoUpdate` toggles `statusReg.flags.wifi_update_in_progress` |
| EVT_MODEM_REGISTERED_CHANGE | MODEM (modem_task) | WEB, UI, SYS | modem_registered: bool, modem_no_sim: bool | event-group-bit | Modem Core v1 + edges: modem updates registration flags consumed by web/ui |
| EVT_MODEM_SIGNAL_LEVEL_CHANGE | MODEM (modem_task) | WEB, UI | modem_signal_level: uint8, modem_rx_level: int16 | snapshot-only | Dependency edges: modem signal fields exported in web values and ui decisions |
| EVT_MODEM_RESPONSE_QTONEDET | MODEM (modem_task) | DTMF | modem_response: enum, modem_dtmf_digit: char | queue | Modem Core v1: modem_decode writes `ATRQTONEDET` and digit |
| EVT_MODEM_SMS_READY | MODEM (modem_task) | WEB, SYS | incoming_sms_ready: bool | event-group-bit | Modem Core v1 owned fields include `statusReg.flags.incoming_sms_ready` |
| EVT_MODEM_AUDIO_TONE_ACTIVE | MODEM (modem_task) | AUD | audio_test_tone_active: bool, audio_test_tone_tick: uint32 | queue | Modem Core -> Audio Runtime shared-state contract |
| EVT_AUD_DIGIT_PLAYING_CHANGE | AUD (audio_task) | SYS, UI | audio_digit_playing: bool | event-group-bit | Audio Runtime v1: `dtmf_task` writes `statusReg.flags.audio_digit_playing` |
| EVT_AUD_TEST_TONE_START_REQUEST | AUD (audio_task) | MODEM | audio_test_tone_start_request: bool | event-group-bit | Audio Runtime v1: `audio_test_task` writes `generalFlags.audio_test_tone_start_request` |
| EVT_AUD_TEST_COMPLETE | AUD (audio_task) | WEB, SYS, UI | audio_test_complete: bool, audio_test_passed: bool | queue | Audio Runtime v1: `audio_test_task` writes completion/result flags |
| EVT_IO_POWER_STATUS_UPDATED | IO (io_task) | WEB, SYS, UI | power_status: bool, external_power: bool, battery_low: bool | snapshot-only | Power/IO v1: power task writes power status flags/telemetry |
| EVT_IO_EXTERNAL_VOLTAGE_UPDATED | IO (io_task) | WEB, SYS | external_voltage: uint16, vbus_voltage: uint16, battery_voltage: uint16 | snapshot-only | Power/IO v1: adc/power tasks write voltage fields |
| EVT_IO_OUTPUTS_APPLIED | IO (io_task) | WEB, SYS | gpio_controls_val: uint16 | queue | Power/IO v1: `gpio_update()` applies `gpio_controls.val` to outputs |
| EVT_IO_KEYPAD_DIGIT_CHANGED | IO (io_task) | SYS, MODEM, DTMF | keypad_keypress: char | queue | Power/IO v1: `gpio_task` writes `statusReg.keypad_keypress` |
| EVT_UI_TFT_STATUS_CHANGE | UI (ui_task) | SYS, WEB | tft_status: bool | event-group-bit | Sensor/UI v1: `tft_init` sets and `tft_deinit` clears `statusReg.flags.tft_status` |
| EVT_UI_IMU_TILT_CHANGE | UI (ui_task) | WEB, SYS | imu_tilt_exceeded: bool, imu_pitch: int16, imu_roll: int16 | queue | Sensor/UI v1: `imu_task` updates tilt flags and orientation |
| EVT_DTMF_LOAD_ACTIVATE_REQUEST | DTMF (dtmf_owner) | IO, SYS | load_activate_request: bool | event-group-bit | In-call DTMF v1: successful PIN compare sets `generalFlags.load_activate_request` |
| EVT_DTMF_ONHOOK_REQUEST | DTMF (dtmf_owner) | SYS | onhook_request: bool | queue | In-call DTMF v1: command 5 + '*' path returns true for on-hook |
## State snapshot schema
```yaml
status_snapshot:
  wifi:
    wifi_server_started: bool
    wifi_server_connected: bool
    wifi_server_disconnected: bool
    wifi_update_in_progress: bool
    wifi_station_count: uint8
    tracked_client_count: uint8
    no_users_connected: bool
    session_active: bool
    web_level: uint8
    last_client_check_ms: uint32
  modem:
    modem_registered: bool
    modem_no_sim: bool
    modem_first_registration: bool
    modem_signal_level: uint8
    modem_rx_level: int16
    modem_response: enum
    modem_dtmf_digit: char
    modem_cell_type: string
    modem_cell_id: string
    incoming_sms_ready: bool
    sms_inprogress: bool
    timedate_set: bool
  call:
    telephone_mode: enum
    telephone_sub_mode: enum
    call_status: enum
    call_connected: bool
    call_inprogress: bool
    off_hook_active: bool
    incoming_call_ringing: bool
    call_volte_active: bool
    last_call_length: uint32
    keypad_keypress: char
  audio_test:
    audio_test_request: bool
    audio_test_abort_request: bool
    audio_test_tone_start_request: bool
    audio_test_cleanup_request: bool
    audio_test_tone_active: bool
    audio_test_tone_tick: uint32
    audio_test_complete: bool
    audio_test_passed: bool
    audio_digit_playing: bool
    audo_path_test_period: uint16
  power_telemetry:
    power_status: bool
    external_power: bool
    battery_low: bool
    power_task_complete: bool
    external_voltage: uint16
    vbus_voltage: uint16
    battery_voltage: uint16
    battery_chargerate: uint16
  ui_indicators:
    tft_status: bool
    led_display_active: bool
    imu_active: bool
    imu_pitch: int16
    imu_roll: int16
    imu_tilt_exceeded: bool
    imu_tilt_reset: bool
    status_led_brightness: uint8
    force_led_update: bool
```
## Mapping from legacy bus fields
| Legacy field | New interface (command/event/snapshot) | Owner | Notes |
|---|---|---|---|
| generalFlags.config_changed | CMD_WEB_CFG_WRITE / EVT_CFG_STORE_WRITTEN | CFG | Legacy multi-writer; serialize through CFG owner |
| generalFlags.gpio_update_request | CMD_WEB_IO_SET_OUTPUTS / EVT_IO_OUTPUTS_APPLIED | IO | Legacy set by WEB/MODEM; IO owner applies outputs |
| generalFlags.modem_reboot_request | CMD_IO_MODEM_REBOOT_REQUEST | MODEM | Legacy button/http/modem flag; modem consumes/clears |
| generalFlags.load_activate_request | EVT_DTMF_LOAD_ACTIVATE_REQUEST | DTMF | Raised on PIN success in dtmf decode |
| generalFlags.audio_test_request | CMD_SYS_AUD_TEST_START | AUD | Audio test request trigger |
| generalFlags.audio_test_abort_request | CMD_SYS_AUD_TEST_ABORT | AUD | Audio test abort trigger |
| generalFlags.audio_test_tone_start_request | EVT_AUD_TEST_TONE_START_REQUEST | AUD | Audio->modem handshake |
| generalFlags.audio_test_cleanup_request | EVT_AUD_TEST_COMPLETE | AUD | Cleanup/result handshake path |
| generalFlags.power_status_request | CMD_SYS_WEB_CLIENT_TIMEOUT_CHECK / snapshot | IO | Used as periodic/request gate in power task |
| generalFlags.external_voltage_request | EVT_IO_EXTERNAL_VOLTAGE_UPDATED | IO | ADC request/complete path |
| generalFlags.force_led_update | EVT_WEB_UPDATE_PROGRESS_CHANGE / snapshot | UI | Consumed and cleared by LED task |
| statusReg.modem_response | EVT_MODEM_RESPONSE_QTONEDET / snapshot | MODEM | Producer modem decode; consumed by DTMF |
| statusReg.modem_dtmf_digit | EVT_MODEM_RESPONSE_QTONEDET / snapshot | MODEM | Crosses modem->dtmf and loop keypad paths |
| statusReg.flags.modem_registered | EVT_MODEM_REGISTERED_CHANGE / snapshot | MODEM | Consumed by WEB/UI/call logic |
| statusReg.flags.modem_no_sim | EVT_MODEM_REGISTERED_CHANGE / snapshot | MODEM | Consumed by UI |
| statusReg.flags.modem_first_registration | snapshot | MODEM | UI behavior input |
| statusReg.modem_signal_level | EVT_MODEM_SIGNAL_LEVEL_CHANGE / snapshot | MODEM | Published to `/values.xml` |
| statusReg.modem_rx_level | EVT_MODEM_SIGNAL_LEVEL_CHANGE / snapshot | MODEM | Signal quality telemetry |
| statusReg.flags.incoming_sms_ready | EVT_MODEM_SMS_READY | MODEM | Incoming SMS state |
| statusReg.flags.sms_inprogress | snapshot | MODEM | SMS engine state |
| statusReg.flags.timedate_set | snapshot | MODEM | Time sync state |
| statusReg.audio_test_tone_tick | EVT_MODEM_AUDIO_TONE_ACTIVE / snapshot | MODEM | Modem->audio timing marker |
| statusReg.flags.audio_test_tone_active | EVT_MODEM_AUDIO_TONE_ACTIVE / snapshot | MODEM | Audio test tone running marker |
| statusReg.flags.audio_test_complete | EVT_AUD_TEST_COMPLETE / snapshot | AUD | Audio test completion state |
| statusReg.flags.audio_test_passed | EVT_AUD_TEST_COMPLETE / snapshot | AUD | Audio test result state |
| statusReg.flags.audio_digit_playing | EVT_AUD_DIGIT_PLAYING_CHANGE / snapshot | AUD | DTMF playback active state |
| statusReg.flags.wifi_server_started | EVT_WEB_AP_SERVER_STARTED / snapshot | WEB | AP/server lifecycle state |
| statusReg.flags.wifi_server_connected | EVT_WEB_AP_CLIENT_CONNECTED / snapshot | WEB | AP station connected state |
| statusReg.flags.wifi_server_disconnected | EVT_WEB_AP_CLIENT_DISCONNECTED / snapshot | WEB | AP station disconnected state |
| statusReg.flags.wifi_update_in_progress | EVT_WEB_UPDATE_PROGRESS_CHANGE / snapshot | WEB | OTA/update state |
| statusReg.external_voltage | EVT_IO_EXTERNAL_VOLTAGE_UPDATED / snapshot | IO | ADC telemetry |
| statusReg.vbus_voltage | EVT_IO_POWER_STATUS_UPDATED / snapshot | IO | Power telemetry |
| statusReg.battery_voltage | EVT_IO_POWER_STATUS_UPDATED / snapshot | IO | Power telemetry |
| statusReg.flags.battery_low | EVT_IO_POWER_STATUS_UPDATED / snapshot | IO | Low battery status |
| statusReg.flags.external_power | EVT_IO_POWER_STATUS_UPDATED / snapshot | IO | External power status |
| statusReg.flags.power_task_complete | snapshot | IO | Power task cycle completion |
| statusReg.flags.hook_button_pressed | snapshot | IO | Button scan output |
| statusReg.flags.hook_button_released | snapshot | IO | Button scan output |
| statusReg.flags.volume_button_up_pressed | snapshot | IO | Button scan output |
| statusReg.flags.volume_button_dwn_pressed | snapshot | IO | Button scan output |
| statusReg.flags.io1_input_status | snapshot | IO | GPIO input telemetry |
| statusReg.flags.io2_input_status | snapshot | IO | GPIO input telemetry |
| statusReg.keypad_keypress | EVT_IO_KEYPAD_DIGIT_CHANGED / snapshot | IO | Keypad digit source |
| statusReg.telephone_mode | snapshot | SYS | Top-level call mode state |
| statusReg.telephone_sub_mode | snapshot | SYS | Submode state |
| statusReg.call_status | snapshot | SYS | Call status telemetry |
| statusReg.flags.call_connected | snapshot | SYS | Call connection state |
| statusReg.flags.call_volte_active | snapshot | MODEM | Cell mode indicator consumed by UI |
| statusReg.flags.off_hook_active | snapshot | SYS | Call/UI state |
| statusReg.flags.incoming_call_ringing | snapshot | SYS | Call/UI state |
| statusReg.flags.imu_tilt_exceeded | EVT_UI_IMU_TILT_CHANGE / snapshot | UI | IMU tilt result |
| statusReg.flags.imu_tilt_reset | snapshot | UI | IMU reset state |
| statusReg.imu_pitch | EVT_UI_IMU_TILT_CHANGE / snapshot | UI | IMU telemetry |
| statusReg.imu_roll | EVT_UI_IMU_TILT_CHANGE / snapshot | UI | IMU telemetry |
| statusReg.flags.led_display_active | snapshot | UI | LED compositor active status |
| statusReg.flags.sms_command_processed | snapshot | UI | Consumed/cleared by LED logic |
| statusReg.flags.tft_status | EVT_UI_TFT_STATUS_CHANGE / snapshot | UI | Set in tft_init, clear in tft_deinit |
| configReg.PIN | snapshot | CFG | Read by DTMF; persisted by config store |
| configReg.telephone_operating_mode | snapshot | CFG | Consumed across modem/audio/io/ui |
| configReg.battery_chargerate | CMD_WEB_PWR_SET_CHARGERATE / snapshot | IO | Configured from web/config; applied by power task |
| configReg.battery_low_level | snapshot | CFG | Consumed by power low-battery checks |
| configReg.status_led_brightness | snapshot | CFG | Consumed by LED/UI |
| configReg.flags.led_board_status_enabled | snapshot | CFG | Consumed by LED/UI |
| configReg.flags.imu_enabled | snapshot | CFG | Consumed by IMU task |
| configReg.imu_tip_threshold | snapshot | CFG | Consumed by IMU task |
| configReg.handset_modem_speaker_volume | CMD_WEB_AUDIO_SET_PARAMS / snapshot | AUD | Audio path setup parameter |
| configReg.handsfree_modem_speaker_volume | CMD_WEB_AUDIO_SET_PARAMS / snapshot | AUD | Audio path setup parameter |
| configReg.mimic_volume | CMD_WEB_AUDIO_SET_PARAMS / snapshot | AUD | Mimic audio parameter |
| configReg.ringer_volume | snapshot | CFG | Consumed by audio/main call flows |
| configReg.announcement_volume | snapshot | CFG | Consumed by audio/main call flows |
| configReg.mimic_dtmf_tone_period | snapshot | CFG | Mimic timing parameter |
| configReg.mimic_dtmf_intertone_period | snapshot | CFG | Mimic timing parameter |
| configReg.auto_onhook_time | snapshot | CFG | Call timeout behavior |
| configReg.auto_dial_time | snapshot | CFG | Dial behavior |
| configReg.auto_answer_rings | snapshot | CFG | Answer behavior |
| gpio_controls.io1_control | CMD_WEB_IO_SET_OUTPUTS / EVT_IO_OUTPUTS_APPLIED | IO | Output control bit |
| gpio_controls.io2_control | CMD_WEB_IO_SET_OUTPUTS / EVT_IO_OUTPUTS_APPLIED | IO | Output control bit |
| gpio_controls.sim_select_control | CMD_WEB_IO_SET_OUTPUTS / EVT_IO_OUTPUTS_APPLIED | IO | SIM select output bit |
| gpio_controls.load_control | CMD_DTMF_IO_SET_LOAD_OUTPUT / EVT_IO_OUTPUTS_APPLIED | IO | Load output from DTMF command path |
| gpio_controls.charge_enable | snapshot | IO | Charge control output bit |
## Open questions
- TODO: `ThingstreamTransport::register_callback`; serial_register_callback callsite context unclear; suspected producer contexts: MODEM task path and/or vendor callback setup; evidence needed: Inspect vendor Thingstream transport creation/registration implementation used by modem init.
- TODO: `Thingstream_Application_modemCallback dispatch thread`; callback vs modem task overlap uncertain; suspected producer contexts: MODEM callback thread and MODEM task; evidence needed: Inspect vendor modem transport runtime dispatch and callback threading model.
- TODO: `Platform_jumpTick vs SysTimer exclusion`; writer interleaving around upTime update uncertain; suspected producer contexts: SYS loop wake path and ISR; evidence needed: Inspect `src/main.cpp` wake callsite for timer/interrupt exclusion or equivalent serialization.
- TODO: `config_write concurrent call order`; loop/http/modem write interleaving uncertain; suspected producer contexts: SYS loop, WEB handlers, MODEM task; evidence needed: Inspect runtime scheduling/locking around config persistence entrypoints.
- TODO: `ESP HTTP callback execution context`; web callback thread identity uncertain from current contracts; suspected producer contexts: WEB task and event callbacks; evidence needed: Inspect IDF `esp_http_server` tasking configuration in rebuild implementation.
- TODO: `tft_task scheduling`; function exists but no callsite in current code evidence; suspected producer contexts: SYS/UI; evidence needed: Inspect UI dispatch policy and confirm if `tft_task` remains unused.
- TODO: `statusReg.modem_dtmf_digit semantics`; legacy loop keypad writes may overlap modem-provided DTMF; suspected producer contexts: SYS loop keypad path and MODEM decode path; evidence needed: Inspect call-mode sequencing and producer precedence in runtime traces.
- TODO: `TODO set coverage`; remaining lower-signal bus fields not mapped in top set; suspected producer contexts: all module owners; evidence needed: Add additional mappings from bus union rows after Phase 1 interface stabilization.
