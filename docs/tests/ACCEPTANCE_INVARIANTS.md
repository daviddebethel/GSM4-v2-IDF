# Acceptance and Invariants

## How to use this document

- Use this file as a pre-merge acceptance gate for rewrite parity against existing v1 module contracts.
- Treat each "Must preserve" item as required runtime behavior parity.
- Treat each "Must not change" item as an invariant contract boundary.
- Execute all "Observable acceptance checks" with repeatable setup/action/observation capture.
- Use "Failure signatures" to triage regressions quickly.
- Track unresolved items under "Open questions and evidence gaps" as TODO gate blockers.

## Global acceptance gates

1. MUST preserve module owner execution contexts documented in v1 contracts (loop/task/ISR/callback) for each acceptance path.
2. MUST preserve documented task-chain entry paths (System Orchestrator -> module entry functions) for all runtime-critical modules.
3. MUST preserve shared-state field names and semantics used as inter-module contracts (statusReg/configReg/generalFlags/gpio_controls).
4. MUST preserve TickGet-driven timing gates and timeout semantics where explicitly documented in module contracts.
5. MUST preserve HTTP route surface and response contract shapes documented for WiFi/Web Control Plane.
6. MUST preserve config persistence compatibility for /config.ini read/write/recovery marker behavior until migration stage is explicitly gated.
7. MUST preserve modem transport polling and callback dispatch integration through Serial Transport and Modem Core boundaries.
8. MUST preserve documented power/GPIO control side effects (request flags, gpio_controls writes, gpio_update application path).
9. MUST preserve documented audio test and DTMF control handshakes across Audio Runtime, Modem Core, and In-call DTMF Control.
10. MUST preserve evidence-backed uncertainty list as open gates; unresolved uncertain items remain explicit and tracked as TODO evidence gaps.

## Module acceptance gates

### Timebase

#### Must preserve
- WHEN hardware timer ISR SysTimer fires, THEN upTime increments by 1.
- WHEN loop resumes from light sleep and computes rtc_dif, THEN Platform_jumpTick(rtc_dif) adds that delta to upTime.
- WHEN TickGet() is called from loop/task code, THEN it returns the current monotonic tick value.
- WHEN Platform_getTimeMillis() is called, THEN it returns the same backing timebase used by TickGet().
- WHEN timerBegin/timerAttachInterrupt/timerAlarmWrite/timerAlarmEnable run in setup path, THEN periodic tick interrupts start.
- WHEN timer registration is re-run in restart path, THEN periodic tick behavior is re-established.
- WHEN loop/task modules gate logic on TickGet deltas, THEN timing gates advance with monotonic upTime progression.
- WHEN Platform_getTimeString() is invoked internally, THEN it formats time from the same millis source used by TickGet().
TODO: add remaining invariants from docs/modules/timebase.md


#### Must not change
- MUST NOT change upTime storage type away from 32-bit volatile semantics without preserving observable behavior.
- MUST NOT remove SysTimer as the periodic writer path for base tick increments.
- MUST NOT remove Platform_jumpTick-based sleep compensation behavior.
- MUST NOT change the timer alarm periodic configuration contract (registered periodic alarm path must remain).
- MUST NOT change TickGet/Platform_getTimeMillis relationship so they diverge in observed monotonic source.

#### Observable acceptance checks
1. Boot timer attach path executed
   1) Setup: Boot through setup/init path with timer registration enabled.
   2) Action: Run for a fixed wall-clock interval and sample TickGet() repeatedly.
   3) Expected observations:
      - TickGet values strictly increase over time.
      - No flatline in tick progression after timer registration.
2. Sleep compensation path
   1) Setup: Enter light sleep wake path with measurable rtc_dif.
   2) Action: After wake, call TickGet() before and after Platform_jumpTick(rtc_dif) path.
   3) Expected observations:
      - Tick increases by compensation delta, not only by ISR single-step progression.
      - Post-wake timeouts based on TickGet remain aligned with resumed runtime.
3. API consistency
   1) Setup: Instrument both Platform_getTimeMillis() and TickGet() in same execution window.
   2) Action: Call both APIs back-to-back multiple times.
   3) Expected observations:
      - Both reflect the same monotonic epoch behavior.
      - No negative/retrograde delta between successive observations.
4. Cross-module time gate viability
   1) Setup: Enable modules that use TickGet gates (modem/audio/power/ui).
   2) Action: Run for several gate cycles.
   3) Expected observations:
      - Each module gate condition reaches its threshold and triggers expected state transitions.
      - No module remains permanently blocked on stale tick values.
5. ISR writer active
   1) Setup: System running with timer interrupt attached.
   2) Action: Observe periodic changes in upTime while loop is idle.
   3) Expected observations:
      - upTime increments without explicit loop writes.
      - Periodic tick cadence is observable in timing-dependent flags/logs.
6. Restart timer path
   1) Setup: Trigger restart path that re-attaches timer alarm sequence.
   2) Action: Observe TickGet after restart sequence completes.
   3) Expected observations:
      - Tick progression continues after restart reconfiguration.
      - No persistent timer-disabled state after restart path.

#### Failure signatures
- Tick-dependent tasks stop advancing while CPU continues running.
- Post-sleep timing jumps are missing or grossly wrong.
- Timeout gates across modules never fire or fire immediately due to invalid tick behavior.
- Observed time goes backwards between consecutive API reads.

#### Evidence source
- docs/modules/timebase.md

### Serial Transport

#### Must preserve
- WHEN modem init runs transport wiring, THEN serial_transport_create returns a ThingstreamTransport-backed instance.
- WHEN modem run path executes modem->run(modem, 1), THEN serial_run polling is performed through the transport vtable.
- WHEN serial_send is invoked, THEN bytes are written to the configured Stream backend.
- WHEN serial_run receives bytes and a callback is set, THEN callback(cookie, buffer, count) is invoked with received payload.
- WHEN lineBased mode receives line terminator, THEN callback dispatch occurs on assembled line payload.
- WHEN transport debug level/output is enabled, THEN init/send/run emits debug prints to configured Print sink.
- WHEN callback pointer is null during polling, THEN receive bytes are consumed without callback invocation.
- WHEN overflow condition is hit in receive buffering, THEN overflow handling path executes and normal callback framing is interrupted.
TODO: add remaining invariants from docs/modules/serial_transport.md


#### Must not change
- MUST NOT remove static transport state contract fields used by create/send/register/run paths.
- MUST NOT change vtable method mapping for init/send/register_callback/run.
- MUST NOT change modem-owned poll entry path (modem->run) to a different observable trigger without preserving behavior.
- MUST NOT change serial_send to a non-Stream write side effect.
- MUST NOT change callback invocation signature/ordering contract for serial_run dispatch.

#### Observable acceptance checks
1. Transport creation chain
   1) Setup: Initialize modem transport chain in modem init flow.
   2) Action: Inspect created transport pointers and execute one run cycle.
   3) Expected observations:
      - serial_transport_create path is exercised.
      - modem->run reaches transport run implementation.
2. TX path
   1) Setup: Transport created and serial backend attached.
   2) Action: Invoke send via transport API with known payload.
   3) Expected observations:
      - Backend Stream write is observed with same payload length.
      - Debug output (if enabled) records send activity.
3. RX callback dispatch
   1) Setup: Register callback in transport stack and feed inbound bytes.
   2) Action: Call transport run with sufficient millis budget.
   3) Expected observations:
      - Callback fires with non-zero count.
      - Payload bytes match inbound serial data framing.
4. Null callback behavior
   1) Setup: Ensure callback pointer is unset.
   2) Action: Feed inbound serial data and call run.
   3) Expected observations:
      - No callback invocation occurs.
      - Run loop still consumes incoming bytes.
5. Line-based framing
   1) Setup: Enable lineBased transport mode and provide CR/LF-terminated input.
   2) Action: Execute run loop until terminator is received.
   3) Expected observations:
      - Callback boundary aligns to line terminator.
      - Multiple lines produce multiple callback invocations.
6. Poll cadence integration
   1) Setup: Run modem thread loop with vTaskDelay cadence.
   2) Action: Observe repeated modem->run invocations over time.
   3) Expected observations:
      - Transport run is repeatedly entered from modem task context.
      - No starvation of RX/TX processing under normal cadence.

#### Failure signatures
- No inbound modem payload reaches callback despite serial RX activity.
- Outbound modem writes do not appear on UART stream backend.
- Transport run is never reached from modem task chain.
- Frequent buffer overflow symptoms with truncated callback payloads.

#### Evidence source
- docs/modules/serial_transport.md

### Modem Core

#### Must preserve
- WHEN modem_thread is created, THEN it repeatedly calls modem_task with its configured delay cadence.
- WHEN modem_task executes, THEN modem_run is invoked as part of the modem state-machine flow.
- WHEN modem_run executes, THEN modem->run(modem, 1) is called to poll transport/modem stack.
- WHEN inbound modem data is delivered via Thingstream callback path, THEN modem_callback/modem_decode mutate modem response state.
- WHEN modem_decode matches +QTONEDET/+DTMF responses, THEN statusReg.modem_dtmf_digit and statusReg.modem_response are updated accordingly.
- WHEN modem registration transitions occur, THEN modem_registered/no_sim related status flags are updated in modem task paths.
- WHEN SMS/GPS timing gates elapse on TickGet thresholds, THEN sms_task/gps_task advance their respective state machines.
- WHEN modem API calls are made from loop/http/test contexts, THEN they enter documented modem entry points without changing owner task semantics.
TODO: add remaining invariants from docs/modules/modem_core.md


#### Must not change
- MUST NOT remove modem_thread -> modem_task -> modem_run execution chain.
- MUST NOT remove Thingstream callback bridge entry points (Thingstream_Application_modemCallback/modem_callback).
- MUST NOT change modem response token handling semantics for ATR* decode paths without preserving outputs.
- MUST NOT remove TickGet-gated timing behavior in modem_task/sms_task/gps_task.
- MUST NOT change shared-state publication of core modem status fields consumed by other modules.

#### Observable acceptance checks
1. Task chain liveness
   1) Setup: System booted with modem thread enabled.
   2) Action: Observe one full modem loop interval.
   3) Expected observations:
      - modem_thread repeatedly invokes modem_task.
      - modem_task enters modem_run path.
2. Transport poll integration
   1) Setup: Transport is initialized in modem init.
   2) Action: Observe modem_run execution.
   3) Expected observations:
      - modem->run(modem,1) is called.
      - Inbound processing path can be reached from callback dispatch.
3. DTMF response decode
   1) Setup: Inject modem response equivalent to +QTONEDET/+DTMF.
   2) Action: Run modem decode path.
   3) Expected observations:
      - statusReg.modem_dtmf_digit updates to decoded digit.
      - statusReg.modem_response is set to ATRQTONEDET or ATRDTMF accordingly.
4. Registration flag behavior
   1) Setup: Simulate registration success/failure responses.
   2) Action: Run modem task state transitions.
   3) Expected observations:
      - statusReg.flags.modem_registered toggles per response.
      - No-sim/error flags update consistently with decoded outcomes.
5. SMS/GPS timing gates
   1) Setup: Enable SMS and GNSS processing conditions.
   2) Action: Advance TickGet beyond configured thresholds.
   3) Expected observations:
      - sms_task retry/timeout branches execute.
      - gps_task periodic/timeout branches execute.
6. External API call-in
   1) Setup: Invoke modem_sleep/modem_sms_busy/modem_call_status_reset from loop test path.
   2) Action: Observe modem core state before/after call.
   3) Expected observations:
      - APIs execute without breaking modem task loop.
      - Expected modem state fields/flags reflect API action.

#### Failure signatures
- Modem thread alive but modem_run never executes.
- Incoming modem payloads do not update shared modem response fields.
- Registration/SMS/GPS state machines stall despite elapsed TickGet thresholds.
- External call-ins corrupt modem task progression.

#### Evidence source
- docs/modules/modem_core.md

### WiFi/Web Control Plane

#### Must preserve
- WHEN loop invokes wifi_start_ap(), THEN AP/web server start flags transition to started state.
- WHEN loop invokes wifi_stop_ap(), THEN AP/web server started flag clears and stop-side effects occur.
- WHEN WiFi station connect/disconnect events fire, THEN wifi_connect_event/wifi_disconnect_event update connection/disconnection flags.
- WHEN /values.xml is requested, THEN XML response includes modem/wifi/status/config-derived values used by current handlers.
- WHEN /config_upload POST final chunk arrives, THEN config persistence path triggers config_write().
- WHEN /config POST update path runs, THEN OTA/update flags (wifi_update_in_progress, force_led_update) toggle through begin/end/error branches.
- WHEN command action setup_advan is posted, THEN power_set_chargerate(configReg.battery_chargerate) call path executes.
- WHEN gpio_on/gpio_off/esim_apply_reset command actions are posted, THEN gpio_controls writes and gpio_update_request signaling occur.
TODO: add remaining invariants from docs/modules/wifi_web_control_plane.md


#### Must not change
- MUST NOT change documented route paths (/values.xml, /setup.xml, /config.xml, /permissions.xml, /config.json, /login, /logout, /upload, /config, /config_upload).
- MUST NOT remove WiFi.onEvent registrations for AP station connect/disconnect callbacks.
- MUST NOT change 5000ms client-timeout gate behavior in wifi_check_client_timeouts without preserving observed timeout cadence.
- MUST NOT remove config persistence call-outs from upload/config command flows.
- MUST NOT remove shared-state publication of wifi_server_started/wifi_update_in_progress flags consumed by UI modules.

#### Observable acceptance checks
1. AP lifecycle start
   1) Setup: System in run mode with WiFi activation condition true.
   2) Action: Call wifi_start_ap path once.
   3) Expected observations:
      - statusReg.flags.wifi_server_started becomes true.
      - HTTP server routes become reachable.
2. AP lifecycle stop
   1) Setup: AP previously started.
   2) Action: Call wifi_stop_ap path.
   3) Expected observations:
      - statusReg.flags.wifi_server_started becomes false.
      - Subsequent route requests no longer succeed until restart.
3. WiFi event callback handling
   1) Setup: AP running with station join/leave activity.
   2) Action: Trigger connect then disconnect event.
   3) Expected observations:
      - wifi_server_connected/disconnected flags update accordingly.
      - Client tracking state reflects station changes.
4. Config upload persistence
   1) Setup: Authenticated session and valid upload payload.
   2) Action: POST to /config_upload and complete final chunk.
   3) Expected observations:
      - config_write call path executes.
      - generalFlags.config_changed/persisted settings reflect uploaded values.
5. Command power call-out
   1) Setup: Authenticated session; POST action setup_advan with charger setting.
   2) Action: Submit command request.
   3) Expected observations:
      - configReg.battery_chargerate updates.
      - power_set_chargerate(...) is invoked.
6. Client timeout cadence
   1) Setup: Tracked clients exist and timeout interval elapses.
   2) Action: Invoke/allow wifi_check_client_timeouts periodic gate.
   3) Expected observations:
      - Inactive client records are dropped per timeout policy.
      - no_users_connected and related flags reflect current tracked set.

#### Failure signatures
- AP started flag and actual route availability diverge.
- Config upload completes but persisted configuration is unchanged.
- WiFi connect/disconnect events do not update tracked client/session state.
- GPIO/power command actions do not produce downstream control effects.

#### Evidence source
- docs/modules/wifi_web_control_plane.md

### Config Store

#### Must preserve
- WHEN config_init() runs at startup, THEN SPIFFS config storage path is initialized for config operations.
- WHEN config_read() opens /config.ini successfully, THEN configReg fields are populated from persisted ordered values.
- WHEN /config.ini is missing or legacy/invalid marker is detected, THEN config_default() and config_write() recovery path executes.
- WHEN config_write() runs, THEN /config.ini is written in the documented ordered newline format with end marker.
- WHEN loop sees generalFlags.config_changed and TickGet gate condition is met, THEN loop clears flag and triggers config_write().
- WHEN button long-hold default flow executes, THEN config_default() then config_write() are called before restart path.
- WHEN HTTP config upload completes final chunk, THEN config_write() is called from upload handler path.
- WHEN modem SIM-apply flow runs sms_apply_esim(), THEN config_write() persists modified SIM-related config.
TODO: add remaining invariants from docs/modules/config_store.md


#### Must not change
- MUST NOT change persisted config file path away from /config.ini without compatibility handling.
- MUST NOT change persisted serialization order/shape unexpectedly; ordered newline snapshot contract must remain compatible.
- MUST NOT change terminal marker compatibility behavior (current marker and legacy marker handling) without preserving migration outcome.
- MUST NOT remove config_default+config_write recovery on invalid/missing persisted config.
- MUST NOT remove loop deferred persistence gate based on generalFlags.config_changed and TickGet interval.

#### Observable acceptance checks
1. Startup read path
   1) Setup: Valid /config.ini present on SPIFFS.
   2) Action: Run startup PREINIT_GO config path.
   3) Expected observations:
      - config_read succeeds and populates configReg values.
      - No fallback default-write path is entered.
2. Missing file recovery
   1) Setup: Delete /config.ini before startup.
   2) Action: Run startup config path.
   3) Expected observations:
      - config_default executes to repopulate configReg.
      - config_write recreates /config.ini with valid marker.
3. Loop deferred write
   1) Setup: Set generalFlags.config_changed=true and elapsed TickGet >= CONFIG_CHECK_PERIOD.
   2) Action: Run loop iteration covering config gate.
   3) Expected observations:
      - config_write is called once for that gate.
      - generalFlags.config_changed is cleared by loop path.
4. HTTP upload persistence
   1) Setup: Authenticated request and valid config upload payload.
   2) Action: POST /config_upload to completion.
   3) Expected observations:
      - config_write is invoked from handler flow.
      - Updated config values are present on subsequent config readback.
5. Button reset persistence
   1) Setup: Hold WIFI button through default period in button_task path.
   2) Action: Allow button_task flow to complete.
   3) Expected observations:
      - config_default and config_write are both called.
      - Config resets are observable on next startup read.
6. Marker compatibility
   1) Setup: Provide legacy-marker config file input.
   2) Action: Run config_read migration path.
   3) Expected observations:
      - Migration/default branch sets config_changed as documented.
      - Resulting config_write emits current marker format.

#### Failure signatures
- Config changes appear in RAM but do not survive reboot.
- /config.ini exists but config_read does not hydrate expected fields.
- Missing/invalid config file does not trigger fallback regeneration.
- Periodic config_changed gate causes repeated writes without clearing condition.

#### Evidence source
- docs/modules/config_store.md

### Audio Runtime

#### Must preserve
- WHEN audio_thread is created, THEN it continuously executes audio_task() with its task delay cadence.
- WHEN audio_playback(file) is called, THEN audio.connecttoFS(SPIFFS, file, true) playback path is engaged.
- WHEN audio_playback_stop() is called, THEN playback stop/force-stop behavior is applied to the Audio object path.
- WHEN audio_dtmf(digit) is requested, THEN dtmf_task emits tone output and clears active playing status after tone window.
- WHEN dtmf_task tone window reaches 200ms gate, THEN tone playback stop path executes.
- WHEN audio_test_request flow starts, THEN audio_test_task transitions through calibration/tone/measurement windows based on TickGet gates.
- WHEN audio_test_task determines result, THEN statusReg.flags.audio_test_passed and statusReg.flags.audio_test_complete are updated.
- WHEN modem provides audio_test_tone_tick/tone_active fields, THEN audio_test_task consumes them for tone-start synchronization.
TODO: add remaining invariants from docs/modules/audio_runtime.md


#### Must not change
- MUST NOT remove audio_thread ownership of continuous audio_task execution.
- MUST NOT remove SPIFFS-backed connecttoFS playback behavior for audio_playback path.
- MUST NOT change dtmf_task 200ms tone gate semantics without preserving observable stop timing behavior.
- MUST NOT remove audio_test shared-flag handshake fields used between audio and modem paths.
- MUST NOT remove DS4420/ES8388 control call paths used by runtime and test flows.

#### Observable acceptance checks
1. Audio task liveness
   1) Setup: Audio thread created during init.
   2) Action: Observe multiple scheduler cycles.
   3) Expected observations:
      - audio_task executes repeatedly in task: audio context.
      - No starvation of playback/DTMF/test sub-tasks.
2. Playback path
   1) Setup: SPIFFS contains target audio file.
   2) Action: Invoke audio_playback('/file').
   3) Expected observations:
      - connecttoFS path is entered.
      - Playback-active state/log indicators transition to active.
3. Playback stop path
   1) Setup: Playback currently active.
   2) Action: Invoke audio_playback_stop().
   3) Expected observations:
      - Audio stop/force-stop path executes.
      - Playback-active indicator clears.
4. DTMF tone window
   1) Setup: Call audio_dtmf with a valid digit.
   2) Action: Allow dtmf_task to run past 200ms gate.
   3) Expected observations:
      - statusReg.flags.audio_digit_playing becomes true then false.
      - Tone output stops after gate expiry.
5. Audio test completion
   1) Setup: Set generalFlags.audio_test_request true with valid test preconditions.
   2) Action: Run audio_test_task through full window.
   3) Expected observations:
      - statusReg.flags.audio_test_complete is set.
      - statusReg.flags.audio_test_passed reflects measured threshold outcome.
6. Cross-module tone sync
   1) Setup: Modem path sets statusReg.flags.audio_test_tone_active and statusReg.audio_test_tone_tick.
   2) Action: Run audio_test_task tone-start wait stage.
   3) Expected observations:
      - audio_test_task observes provided tone markers.
      - Tone wait timeout path or success path matches gate conditions.

#### Failure signatures
- Audio thread runs but playback never starts despite valid file/action.
- DTMF tone remains stuck active beyond expected 200ms window.
- Audio test never reaches complete flag under valid trigger conditions.
- Cross-task audio-test handshake flags drift out of expected sequence.

#### Evidence source
- docs/modules/audio_runtime.md

### Power and Physical IO

#### Must preserve
- WHEN loop iterates in run mode, THEN power_task(), adc_task(), button_task(), and gpio_task() are called.
- WHEN power_status_request is set or 5-second TickGet gate elapses, THEN power_task executes charger/power telemetry cycle.
- WHEN external_voltage_request is set, THEN adc_task samples ADC path and updates statusReg.external_voltage.
- WHEN button_task reaches WIFI_BUTTON_DEFAULT_PERIOD hold condition, THEN config_default() and config_write() are called.
- WHEN button_task detects reboot trigger condition, THEN generalFlags.modem_reboot_request is set.
- WHEN gpio_task 10ms gate elapses, THEN button/keypad/IO scan and debounced statusReg flag updates execute.
- WHEN gpio_controls are changed and gpio_update is called, THEN gpio expander output ports are updated from gpio_controls.val.
- WHEN HTTP setup_advan command calls power_set_chargerate, THEN charger configuration path is applied to BQ25622 control flow.
TODO: add remaining invariants from docs/modules/power_and_physical_io.md


#### Must not change
- MUST NOT remove loop ownership of power/adc/button/gpio task call sequence.
- MUST NOT change power_task 5-second gate semantics without preserving observed status update cadence.
- MUST NOT change gpio_task 10ms gate semantics without preserving scan/debounce behavior.
- MUST NOT remove config_default/config_write long-hold side effect in button reset path.
- MUST NOT remove gpio_update side effect of writing gpio2 port outputs from gpio_controls value.

#### Observable acceptance checks
1. Loop task fan-out
   1) Setup: System in normal run loop.
   2) Action: Observe one loop cycle with instrumentation on task calls.
   3) Expected observations:
      - power_task/adc_task/button_task/gpio_task all execute.
      - No expected task call is skipped in steady state.
2. Power 5-second gate
   1) Setup: Clear power_status_request and record power_tick baseline.
   2) Action: Advance TickGet beyond 5-second threshold.
   3) Expected observations:
      - power_task enters telemetry branch.
      - statusReg.vbus_voltage/battery_voltage and completion flags update.
3. ADC request path
   1) Setup: Set generalFlags.external_voltage_request=true.
   2) Action: Run loop through adc_task call.
   3) Expected observations:
      - ADC read conversion runs.
      - statusReg.external_voltage updates and request flag clears as documented.
4. Button default hold
   1) Setup: Hold WIFI button input active beyond default period.
   2) Action: Run button_task through hold window.
   3) Expected observations:
      - config_default then config_write call sequence occurs.
      - Restart/reboot trigger side effects are observable.
5. GPIO update pattern
   1) Setup: Write gpio_controls.io1/io2/load bits then request/update path.
   2) Action: Execute loop consumer or direct gpio_update call.
   3) Expected observations:
      - gpio2.write_port0/1 receives updated value.
      - External IO state reflects requested control bits.
6. Cross-module modem reboot request
   1) Setup: Trigger button path that sets modem reboot request.
   2) Action: Run modem task consumer path subsequently.
   3) Expected observations:
      - generalFlags.modem_reboot_request is observed by modem core.
      - Request flag is consumed/cleared in modem handling flow.

#### Failure signatures
- Power telemetry flags never refresh despite elapsed 5-second gate.
- GPIO output controls change in state but expander outputs do not update.
- Button long-hold does not trigger config reset/persist behavior.
- Modem reboot request flag set but never consumed downstream.

#### Evidence source
- docs/modules/power_and_physical_io.md

### Sensor and Visual UI

#### Must preserve
- WHEN loop runs, THEN imu_task() and leddisplay_task() are executed in loop context.
- WHEN imu_task TickGet gate reaches TICK_SECOND, THEN IMU sampling/processing updates pitch/roll and tilt flags.
- WHEN leddisplay_task TickGet gate reaches TICK_MILLIS*10, THEN LED compositor updates and FastLED.show() flush occur.
- WHEN system enters documented mode transitions, THEN tft_init/tft_logo/tft_message/tft_deinit are called from loop path.
- WHEN tft_init succeeds, THEN statusReg.flags.tft_status is set true.
- WHEN tft_deinit runs, THEN statusReg.flags.tft_status is set false.
- WHEN WiFi/modem status flags change, THEN leddisplay_task reflects them in LED decision logic on subsequent loop cycles.
- WHEN force_led_update is set, THEN LED refresh consumes and clears generalFlags.force_led_update in compositor flow.
TODO: add remaining invariants from docs/modules/sensor_and_visual_ui.md


#### Must not change
- MUST NOT remove loop-owned invocation of imu_task and leddisplay_task.
- MUST NOT change IMU 1-second gate behavior without preserving observable update cadence.
- MUST NOT change LED 10ms gate behavior without preserving observable flash/composite cadence.
- MUST NOT remove tft_status writes in tft_init/tft_deinit state transitions.
- MUST NOT remove FastLED.show or TFT draw/SPIFFS logo rendering side effects used by current UI paths.

#### Observable acceptance checks
1. IMU periodic update
   1) Setup: IMU enabled config flags set and imu_active true.
   2) Action: Run loop beyond one TICK_SECOND interval.
   3) Expected observations:
      - statusReg.imu_pitch and/or statusReg.imu_roll update.
      - Tilt flags/logtext flags change per threshold logic.
2. LED periodic compositor
   1) Setup: LED status feature enabled and relevant status flags set.
   2) Action: Run loop beyond multiple 10ms gates.
   3) Expected observations:
      - FastLED.show is called repeatedly.
      - LED state transitions follow modem/wifi/system status flags.
3. TFT init/logo path
   1) Setup: Enter startup path that initializes TFT.
   2) Action: Run INITIAL_GO branch through tft_init/tft_logo.
   3) Expected observations:
      - statusReg.flags.tft_status becomes true.
      - Logo rendering path opens /logo.bmp via SPIFFS and draws.
4. TFT deinit path
   1) Setup: Enter sleep transition path.
   2) Action: Run branch that calls tft_deinit.
   3) Expected observations:
      - statusReg.flags.tft_status becomes false.
      - Display clear/off behavior is observable.
5. WiFi/modem flag coupling
   1) Setup: Toggle wifi_server_started/wifi_update_in_progress/modem_registered flags.
   2) Action: Run leddisplay_task for several cycles.
   3) Expected observations:
      - LED output pattern changes with those flags.
      - sms_command_processed and force_led_update consume/clear behavior is observed.
6. Loop confinement
   1) Setup: No external task invokes ui entrypoints except loop branches.
   2) Action: Run full system scenario with instrumentation.
   3) Expected observations:
      - imu_task and leddisplay_task call origin remains loop.
      - No ISR/task-owned direct invocation is observed for documented entrypoints.

#### Failure signatures
- IMU task runs but pitch/roll/tilt flags never update under motion.
- LED updates lag/freeze despite status flag changes.
- TFT status flag does not match init/deinit state transitions.
- Startup logo/message render path fails while SPIFFS assets are present.

#### Evidence source
- docs/modules/sensor_and_visual_ui.md

### In-call DTMF Control

#### Must preserve
- WHEN loop reaches IDLE_RUN or submode_task call sites, THEN dtmf_decode_task() is invoked.
- WHEN statusReg.modem_response equals ATRQTONEDET, THEN dtmf_decode_task consumes modem_dtmf_digit and clears modem_response.
- WHEN '#' starts compare mode and four digits match configReg.PIN, THEN generalFlags.load_activate_request is set true.
- WHEN compare/program mode is active and timeout exceeds TICK_SECOND*DTMF_TIMEOUT_PERIOD, THEN digit state resets and gain restore path runs.
- WHEN '*' command branch receives immediate '*' deactivation sequence, THEN gpio_controls.load_control is set to 0 and gpio_update() is called.
- WHEN call_connected is true at mute points, THEN ds4420_mute/ds4420_set_gain paths apply speaker mute/restore behavior.
- WHEN command '5' receives final '*' in program branch, THEN dtmf_decode_task returns true to request on-hook transition.
- WHEN keypad loop path writes statusReg.modem_dtmf_digit, THEN dtmf_decode_task evaluates it within same loop-owned DTMF control flow.
TODO: add remaining invariants from docs/modules/in_call_dtmf_control.md


#### Must not change
- MUST NOT remove loop call sites that invoke dtmf_decode_task in IDLE_RUN and submode_task flows.
- MUST NOT change ATRQTONEDET trigger check semantics for starting DTMF decode handling.
- MUST NOT change DTMF timeout constant usage (DTMF_TIMEOUT_PERIOD with TickGet gate) without preserving observed timeout behavior.
- MUST NOT remove load-control side effects (gpio_controls.load_control write and gpio_update call) from deactivation path.
- MUST NOT remove ds4420 mute/gain call-outs used for in-call audio gating during DTMF handling.

#### Observable acceptance checks
1. Loop entry invocation
   1) Setup: System in IDLE_RUN and in call submode path.
   2) Action: Run loop through both call sites.
   3) Expected observations:
      - dtmf_decode_task executes at IDLE_RUN site.
      - dtmf_decode_task executes via submode_task site.
2. ATRQTONEDET trigger
   1) Setup: Set statusReg.modem_response=ATRQTONEDET and statusReg.modem_dtmf_digit to a test digit.
   2) Action: Run dtmf_decode_task once.
   3) Expected observations:
      - Digit is consumed by decode branch.
      - statusReg.modem_response is cleared/reset by handler.
3. PIN compare activation
   1) Setup: Feed '#' then four PIN-matching digits through modem_dtmf_digit path.
   2) Action: Run dtmf_decode_task over sequence.
   3) Expected observations:
      - Compare state progresses and resets after 4 digits.
      - generalFlags.load_activate_request becomes true on successful match.
4. Timeout reset
   1) Setup: Enter compare/program state but stop feeding digits.
   2) Action: Advance TickGet beyond TICK_SECOND*DTMF_TIMEOUT_PERIOD.
   3) Expected observations:
      - digit_compare/digit_program/digit_pointer reset state executes.
      - Gain restore call path executes when call_connected is true.
5. Load deactivation command
   1) Setup: Feed '*' then '*' sequence in program mode.
   2) Action: Run dtmf_decode_task sequence.
   3) Expected observations:
      - gpio_controls.load_control is set to 0.
      - gpio_update() call path executes.
6. On-hook command
   1) Setup: Feed program command digit '5' then '*'.
   2) Action: Run dtmf_decode_task sequence.
   3) Expected observations:
      - Function returns true for on-hook request.
      - Caller submode transitions to hang-up path.

#### Failure signatures
- ATRQTONEDET is present but DTMF handler never consumes digit or clears response.
- Correct PIN sequence does not assert load activation request flag.
- DTMF compare/program state never times out under no-input condition.
- Deactivation/program command sequence fails to update load control outputs.

#### Evidence source
- docs/modules/in_call_dtmf_control.md

## Golden flows

1. Timebase sleep compensation
   - Trigger: System exits light sleep with rtc_dif computed.
   - Steps:
     - Run wake path including Platform_jumpTick(rtc_dif).
     - Resume loop and timing-gated module execution.
   - Expected observations:
     - Tick value advances by compensation delta.
     - Post-wake timeout-driven branches behave consistently with resumed time.
2. WiFi AP lifecycle + client timeout
   - Trigger: Run mode enables AP lifecycle.
   - Steps:
     - Call wifi_start_ap and receive client connect/disconnect events.
     - Allow 5000ms timeout gate and call wifi_check_client_timeouts.
     - Call wifi_stop_ap.
   - Expected observations:
     - wifi_server_started flag toggles true then false.
     - client tracking and connected/disconnected flags update through events and timeout pass.
3. Config upload persistence
   - Trigger: Authenticated web session and valid config payload.
   - Steps:
     - POST /config_upload through final chunk.
     - Trigger config_write path.
     - Restart/re-read config path.
   - Expected observations:
     - Persisted /config.ini reflects new values.
     - Subsequent config_read hydrates matching configReg fields.
4. Modem polling and decode
   - Trigger: Modem thread created and transport chain initialized.
   - Steps:
     - Run modem_thread -> modem_task -> modem_run polling cycle.
     - Feed modem response payload into decode callback path.
   - Expected observations:
     - modem->run is repeatedly called.
     - statusReg modem response fields update according to decoded payload.
5. Audio test handshake
   - Trigger: Set generalFlags.audio_test_request and required preconditions.
   - Steps:
     - Run audio_task/audio_test_task loop.
     - Allow modem tone handshake fields to update.
     - Observe completion branch.
   - Expected observations:
     - audio_test_complete flag sets.
     - audio_test_passed reflects measured threshold result.
6. In-call DTMF load activation
   - Trigger: Feed ATRQTONEDET with '#' then valid 4-digit PIN sequence.
   - Steps:
     - Run dtmf_decode_task from loop call sites.
     - Complete compare sequence.
   - Expected observations:
     - generalFlags.load_activate_request sets true.
     - modem_response consumed/cleared in handler path.
7. In-call DTMF load deactivation
   - Trigger: Feed ATRQTONEDET with '*' command deactivation sequence.
   - Steps:
     - Run dtmf_decode_task program branch.
     - Apply load control output update.
   - Expected observations:
     - gpio_controls.load_control set to 0.
     - gpio_update call path executes and output state changes.
8. Button hold reset + persist
   - Trigger: Hold WIFI button input beyond default period.
   - Steps:
     - Run button_task periodic checks.
     - Enter config_default + config_write branch.
   - Expected observations:
     - Reset defaults are persisted.
     - restart/request side effects from button flow are observable.

## Logging requirements

- Standard rewrite log tag set: `TMB`, `SRT`, `MOD`, `WEB`, `CFG`, `AUD`, `PWR`, `UI`, `DTMF`.
- Log matching is pattern-based; exact wording may vary as long as acceptance checks remain evaluable.
- Timebase (`TMB`):
  - `TMB` + pattern: `tick increment path active`
  - `TMB` + pattern: `sleep jump applied`
  - `TMB` + pattern: `timer attach/restart configured`
- Serial Transport (`SRT`):
  - `SRT` + pattern: `transport created`
  - `SRT` + pattern: `serial send len=`
  - `SRT` + pattern: `callback dispatch count=`
- Modem Core (`MOD`):
  - `MOD` + pattern: `modem task cycle`
  - `MOD` + pattern: `modem decode response=`
  - `MOD` + pattern: `sms/gps timer gate`
- WiFi/Web Control Plane (`WEB`):
  - `WEB` + pattern: `AP start/stop`
  - `WEB` + pattern: `route hit path=`
  - `WEB` + pattern: `update/upload state change`
- Config Store (`CFG`):
  - `CFG` + pattern: `config read path`
  - `CFG` + pattern: `config write path`
  - `CFG` + pattern: `default recovery path`
- Audio Runtime (`AUD`):
  - `AUD` + pattern: `audio task cycle`
  - `AUD` + pattern: `playback start/stop`
  - `AUD` + pattern: `audio test result`
- Power and Physical IO (`PWR`):
  - `PWR` + pattern: `power task gate`
  - `PWR` + pattern: `gpio update applied`
  - `PWR` + pattern: `button hold action`
- Sensor and Visual UI (`UI`):
  - `UI` + pattern: `imu task update`
  - `UI` + pattern: `led compositor update`
  - `UI` + pattern: `tft init/message/deinit`
- In-call DTMF Control (`DTMF`):
  - `DTMF` + pattern: `dtmf trigger consumed`
  - `DTMF` + pattern: `pin compare/program state`
  - `DTMF` + pattern: `load control action`

## Open questions and evidence gaps

- TODO [Timebase]: Exclusion behavior between SysTimer ISR writes and Platform_jumpTick loop write is uncertain. Evidence to prove: Inspect src/main.cpp around SysTimer and Platform_jumpTick callsite plus timer/interrupt disable evidence.
- TODO [Serial Transport]: Repository callsite for ThingstreamTransport::register_callback is not visible. Evidence to prove: Inspect vendor Thingstream transport implementation where register_callback is invoked.
- TODO [Serial Transport]: Registration timing overlap vs serial_run polling is uncertain. Evidence to prove: Inspect callback registration call chain and runtime scheduling context in vendor transport stack.
- TODO [Modem Core]: Callback thread provenance for Thingstream callback dispatch remains uncertain. Evidence to prove: Inspect vendor implementations behind Thingstream_createModem2Transport and Thingstream_Modem2_sendLine.
- TODO [Modem Core]: Overlap behavior between callback mutation path and modem_task mutation path is uncertain. Evidence to prove: Inspect callback dispatch thread model and synchronization policy in vendor transport/runtime.
- TODO [WiFi/Web Control Plane]: AsyncWebServer route callback execution thread identity is uncertain. Evidence to prove: Inspect AsyncWebServer runtime/task context configuration used in this firmware build.
- TODO [WiFi/Web Control Plane]: WiFi.onEvent callback execution thread identity is uncertain. Evidence to prove: Inspect Arduino WiFi event callback dispatch model for this target/runtime.
- TODO [WiFi/Web Control Plane]: No explicit synchronization evidence for shared auth/client state across loop/event/http contexts. Evidence to prove: Inspect runtime threading model and any hidden locking wrappers around wifi_handler state.
- TODO [Config Store]: Interleaving order among loop/http/modem config_write callers is uncertain. Evidence to prove: Inspect runtime thread model and call serialization around config_write invocations.
- TODO [Config Store]: HTTP callback thread identity relative to loop writes is uncertain. Evidence to prove: Inspect AsyncWebServer callback task context and file_handler call sites.
- TODO [Audio Runtime]: Audio object thread-safety under loop/test mutations vs task audio.loop() is uncertain. Evidence to prove: Inspect src/audio/Audio.cpp synchronization guarantees and all call-in scheduling contexts.
- TODO [Audio Runtime]: Production inclusion of test_task call path depends on compile branch selection. Evidence to prove: Inspect build flags around MODEM_DISABLED and resulting compiled control flow in main.cpp.
- TODO [Audio Runtime]: Ordering of HTTP-driven config writes vs task-audio reads is uncertain. Evidence to prove: Inspect cross-context scheduling and any synchronization around configReg access in audio paths.
- TODO [Power and Physical IO]: HTTP callback thread model relative to loop-driven IO writes is uncertain. Evidence to prove: Inspect AsyncWebServer runtime context and gpio/power shared-field access interleaving.
- TODO [Power and Physical IO]: No explicit synchronization shown for loop/task/http shared-field interleaving. Evidence to prove: Inspect runtime lock/serialization policy outside module files.
- TODO [Sensor and Visual UI]: tft_task scheduling is uncertain because no call site is present in src/. Evidence to prove: Inspect build/runtime dispatch mechanism for tft_task or confirm dead entrypoint status.
- TODO [Sensor and Visual UI]: Producer callback thread contexts for WiFi/modem flags consumed by LED path are uncertain. Evidence to prove: Inspect callback dispatch contexts in WiFi and modem vendor/runtime paths.
- TODO [In-call DTMF Control]: Execution context provenance of modem decode writes at cited modem_handler sites is uncertain under vendor callback dispatch. Evidence to prove: Inspect vendor Thingstream callback dispatch implementation reaching modem_decode writer sites in src/src/modem_handler.cpp.
