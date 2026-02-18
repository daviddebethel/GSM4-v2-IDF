# Legacy GSM4 Functional and Feature Requirements Baseline

## 0) Scope and intent
- What this doc is for
  - This document defines an evidence-based baseline of legacy GSM4 functional and feature requirements for Slices A-E (Audio path, Modem + transport, WiFi + HTTP + file/config, Timing + IO + UI glue, and Test harness), so ESP-IDF rewrite behaviour can be preserved.
- What it excludes
  - This document excludes redesign proposals, performance improvements, and any behaviour not evidenced in the allowed source set.
  - This pass excludes no Step 6 slices.
  - `docs/behaviour/*` artefacts were not present in this repository snapshot, so no `EVID:behaviour:*` tags are used.
  - In this repository snapshot, `A_calls_cluster_all_cpp.md` is sourced from `docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md`; `03_entry_points.md` and `05_bus_access_union_fn_refined.md` are also sourced from `docs/refactor_analysis/agents/`; `08_interaction_clusters_v2_fanout.md` and `09_core_subclusters.md` are at repository root.
- Definitions: “legacy”, “rewrite”, “slice”, “invariant”, “evidence”
  - legacy: Existing GSM4 firmware behaviour implemented in the current codebase.
  - rewrite: Target ESP-IDF implementation that must preserve evidenced legacy behaviour.
  - slice: The Step 6 file grouping; this pass covers Slices A-E from `11_step6_slices.md`.
  - invariant: Acceptance property intended to remain true across legacy and rewrite; IDs are `TBD` where no mapped invariant ID is available.
  - evidence: Verifiable source artefact backing each requirement, expressed with machine-searchable `EVID:*` tags.

## 1) System overview (evidence-based only)
- Execution contexts present (tasks/ISR/callbacks) with evidence
  - `setup` context exists and configures the hardware timer alarm path: `EVID:entry:setup`.
  - `task: audio` context exists via `AudioThread` (`audio_thread -> audio_task`): `EVID:entry:task:AudioThread`, `EVID:call:src/main.cpp→src/src/audio_handler.cpp`.
  - `loop` context exists and directly drives DTMF decode and call submodes: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/dtmf_handler.cpp`.
  - `ISR` context exists via timer ISR entry `SysTimer()`: `EVID:entry:ISR:SysTimer`.
  - `test harness` context exists in `test_task()` and is called from `loop` in the test build path: `EVID:call:src/main.cpp→src/src/test_handler.cpp`, `EVID:bus:statusReg.flags.call_inprogress`, `EVID:code:src/src/test_handler.cpp:test_task`.
  - `task: modem` context coordinates audio-test tone and cleanup handshakes: `EVID:entry:task:ModemThread`, `EVID:bus:generalFlags.audio_test_tone_start_request`, `EVID:bus:generalFlags.audio_test_cleanup_request`.
  - `Thingstream callback` context writes modem response/digit fields consumed by Slice A decode: `EVID:entry:callback:Thingstream_Application_modemCallback`, `EVID:bus:statusReg.modem_response`, `EVID:bus:statusReg.modem_dtmf_digit`.
  - `wifi event callback` context updates AP client state from STA connect/disconnect events: `EVID:entry:callback:WiFi AP STA connected`, `EVID:entry:callback:WiFi AP STA disconnected`, `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`.
  - `http route handler` context serves authenticated web routes, uploads, and config payloads: `EVID:entry:callback:HTTP route /config.json (GET)`, `EVID:entry:callback:HTTP route /config_upload (POST body)`, `EVID:entry:callback:HTTP not-found route`.
- High-level feature areas (bullet list) with evidence
  - Slice A is the audio/DTMF execution group in Step 6 (`src/audio/Audio.cpp`, `src/audio/mp3_decoder/mp3_decoder.cpp`, `src/src/audio_handler.cpp`, `src/src/dtmf_handler.cpp`) and sits inside Core Subcluster 1 (`09_core_subclusters.md`).
  - Audio playback and codec control path is tightly coupled with `main.cpp` and `Audio.cpp`: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:call:src/audio/Audio.cpp→src/src/audio_handler.cpp`, `EVID:call:src/audio/Audio.cpp→src/audio/mp3_decoder/mp3_decoder.cpp`.
  - In-call DTMF decode drives load/telephone control via shared bus fields: `EVID:bus:statusReg.modem_response`, `EVID:bus:statusReg.modem_dtmf_digit`, `EVID:bus:generalFlags.load_activate_request`, `EVID:bus:gpio_controls.load_control`.
  - DTMF decode is cross-coupled to audio gain/mute controls through cross-file calls: `EVID:call:src/src/dtmf_handler.cpp→src/src/audio_handler.cpp`.
  - Audio path self-test is coordinated between loop, audio task, and modem task through one-shot flags and status bits: `EVID:bus:generalFlags.audio_test_request`, `EVID:bus:generalFlags.audio_test_tone_start_request`, `EVID:bus:statusReg.flags.audio_test_complete`, `EVID:bus:statusReg.flags.audio_test_tone_active`.
  - Slice B is the modem/transport execution group in Step 6 (`src/main.cpp`, `src/src/common.cpp`, `src/src/modem_handler.cpp`, `src/src/serial_transport.cpp`) and sits inside Core Subcluster 1 (`09_core_subclusters.md`).
  - Slice B control and transport coupling is dominated by loop-to-modem and modem-to-serial edges: `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:call:src/src/modem_handler.cpp→src/src/serial_transport.cpp`, `EVID:call:src/src/modem_handler.cpp→src/src/common.cpp`.
  - Slice B behaviour is orchestrated by shared one-shot request flags and completion/status flags across loop, modem task, and Thingstream callback contexts: `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:generalFlags.ftp_connect_request`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:bus:statusReg.modem_response`.
  - Slice C is the WiFi/web/config execution group in Step 6 (`src/src/file_handler.cpp`, `src/src/wifi_handler.cpp`) and sits inside Core Subcluster 1 (`09_core_subclusters.md`).
  - Slice C is strongly coupled through loop-to-web startup calls and shared config state: `EVID:call:src/main.cpp→src/src/wifi_handler.cpp`, `EVID:call:src/main.cpp→src/src/file_handler.cpp`, `EVID:bus:generalFlags.wifi_request`, `EVID:bus:generalFlags.config_changed`.
  - Slice C web paths bridge HTTP callbacks to config persistence and update flows: `EVID:entry:callback:HTTP route /config_upload (POST body)`, `EVID:entry:callback:HTTP route /config (POST body)`, `EVID:call:src/src/wifi_handler.cpp→src/src/file_handler.cpp`, `EVID:bus:statusReg.flags.wifi_update_in_progress`.
  - Slice D is the timing/IO/UI glue execution group and spans Core Subcluster 1 (`src/src/gpio_handler.cpp`, `src/src/power_handler.cpp`, `src/src/tft_handler.cpp`, `src/src/tick.cpp`) plus singleton subclusters (`src/src/adc_handler.cpp`, `src/src/button_handler.cpp`, `src/src/imu_handler.cpp`, `src/src/leddisplay_handler.cpp`) in `09_core_subclusters.md`.
  - Slice D execution is loop-centric with timer ISR support, and is call-coupled from `main.cpp` into all Slice D handlers: `EVID:entry:loop`, `EVID:entry:setup`, `EVID:entry:ISR:SysTimer`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`, `EVID:call:src/main.cpp→src/src/button_handler.cpp`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`, `EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`, `EVID:call:src/main.cpp→src/src/tft_handler.cpp`, `EVID:call:src/main.cpp→src/src/tick.cpp`.
  - Slice D shares control via request/status fields for power, IO, IMU, display, and TFT lifecycle: `EVID:bus:generalFlags.external_voltage_request`, `EVID:bus:statusReg.external_voltage`, `EVID:bus:generalFlags.power_status_request`, `EVID:bus:statusReg.keypad_keypress`, `EVID:bus:statusReg.flags.imu_tilt_exceeded`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:bus:statusReg.flags.tft_status`.
  - Slice E is the test-harness execution group (`src/src/test_handler.cpp`) and sits inside Core Subcluster 1 in `09_core_subclusters.md`.
  - Slice E couples loop test flow to modem/audio/GPIO handlers and shared state flags: `EVID:call:src/main.cpp→src/src/test_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:generalFlags.call_answer_request`, `EVID:bus:generalFlags.call_hangup_request`, `EVID:bus:generalFlags.sms_request_user`.

## 2) Functional requirements by slice/module

### Slice A (Audio path) / Core Subcluster 1
#### 2.1.1 Feature list
- FTR-SA-01: Dedicated audio service task initialises and runs continuous audio processing. Evidence: `EVID:entry:task:AudioThread`, `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/main.cpp:audio_thread`
- FTR-SA-02: Local mimic playback control (start/stop/volume) for telephony UX tones and prompts. Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/audio_handler.cpp:audio_playback`, `EVID:code:src/src/audio_handler.cpp:audio_playback_stop`, `EVID:code:src/src/audio_handler.cpp:audio_set_volume`
- FTR-SA-03: Audio-thread DTMF tone generation from SPIFFS WAV assets with playback state tracking. Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_digit_playing`, `EVID:code:src/src/audio_handler.cpp:dtmf_task`, `EVID:code:src/src/audio_handler.cpp:audio_dtmf`
- FTR-SA-04: In-call modem DTMF command decode for PIN compare, load control, and on-hook signalling. Evidence: `EVID:call:src/main.cpp→src/src/dtmf_handler.cpp`, `EVID:call:src/src/dtmf_handler.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.modem_response`, `EVID:bus:statusReg.modem_dtmf_digit`, `EVID:bus:generalFlags.load_activate_request`, `EVID:bus:gpio_controls.load_control`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
- FTR-SA-05: Two-phase (PRIME then REAL) audio path self-test with modem tone handshake and pass/fail reporting. Evidence: `EVID:bus:generalFlags.audio_test_request`, `EVID:bus:generalFlags.audio_test_tone_start_request`, `EVID:bus:statusReg.flags.audio_test_tone_active`, `EVID:bus:statusReg.flags.audio_test_passed`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
- FTR-SA-06: Audio-test orchestration across loop/audio/modem contexts including cleanup and retry handling. Evidence: `EVID:bus:generalFlags.audio_test_cleanup_request`, `EVID:bus:statusReg.flags.audio_test_cleanup_complete`, `EVID:entry:task:ModemThread`, `EVID:code:src/main.cpp:loop`, `EVID:code:src/src/modem_handler.cpp:audio_test_cleanup`

#### 2.1.2 Functional requirements
- REQ-SA-01: The firmware shall start the Slice A audio service in `audio_thread` by calling `audio_init()` once and then calling `audio_task()` continuously with a one-tick delay.
  - Rationale: Entry-point and task code show a dedicated audio thread with init-then-loop behaviour.
  - Inputs: `xTaskCreate(... audio_thread ...)` task start event.
  - Outputs: `audio_init()` call; repeated `audio_task()` calls.
  - Execution context: task: audio
  - Evidence: `EVID:entry:task:AudioThread`, `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/main.cpp:audio_thread`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-02: `audio_task()` shall execute `audio.loop()`, `dtmf_task()`, and `audio_test_task()` on each audio-thread iteration.
  - Rationale: Slice A runtime behaviour is composed explicitly from these three calls.
  - Inputs: Audio thread iteration tick.
  - Outputs: Playback progression, DTMF processing, and audio-test state progression.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/audio_handler.cpp:audio_task`, `EVID:entry:task:AudioThread`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-03: When `audio_playback(file)` is invoked, the firmware shall set I2S sample rate to 16000 and start playback of the requested SPIFFS file.
  - Rationale: Legacy playback setup explicitly configures sample rate and filesystem source before playback.
  - Inputs: Loop/submode call into `audio_playback`.
  - Outputs: I2S rate update and active file playback.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/audio_handler.cpp:audio_playback`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-04: When `audio_playback_stop()` is invoked, the firmware shall force-stop the current playback stream.
  - Rationale: Legacy code uses a direct force-stop path to terminate active mimic audio.
  - Inputs: Loop/submode stop request.
  - Outputs: Playback terminated.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/audio_handler.cpp:audio_playback_stop`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-05: `audio_set_volume(volume)` shall apply legacy scaling of `volume * 6` before writing the audio engine volume.
  - Rationale: Legacy mimic volume mapping is encoded as fixed linear scaling.
  - Inputs: Mimic volume value from loop/submode logic.
  - Outputs: Scaled volume written to audio engine.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/audio_handler.cpp:audio_set_volume`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SA-06: `audio_dtmf(digit)` shall queue the requested digit for audio-thread handling rather than opening files in loop context.
  - Rationale: Legacy implementation separates request production (loop) and file/I2S operations (audio task).
  - Inputs: Keypad or dialling digit event routed from loop.
  - Outputs: Pending DTMF digit queued for `dtmf_task`.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/audio_handler.cpp:audio_dtmf`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-07: `dtmf_task()` shall map digits `0-9`, `*`, `#`, and `t` to their legacy SPIFFS WAV files and set `statusReg.flags.audio_digit_playing` true when tone playback starts.
  - Rationale: Legacy DTMF tone generation is file-backed with explicit digit-to-file mapping and state flagging.
  - Inputs: Queued pending DTMF digit.
  - Outputs: WAV file open/read path and `statusReg.flags.audio_digit_playing` update.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_digit_playing`, `EVID:code:src/src/audio_handler.cpp:dtmf_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-08: While `statusReg.flags.audio_digit_playing` is true, `dtmf_task()` shall stream WAV chunks to I2S and clear playback state when tone data ends.
  - Rationale: Legacy DTMF output loops over file chunks and terminates by clearing active state.
  - Inputs: Active DTMF file stream and playback flag.
  - Outputs: I2S writes; playback-flag clear on completion.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_digit_playing`, `EVID:code:src/src/audio_handler.cpp:dtmf_read_file`, `EVID:code:src/src/audio_handler.cpp:dtmf_tone_output`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-09: The firmware shall stop local DTMF playback after 200 ms and clear DTMF buffers/DMA state.
  - Rationale: Legacy behaviour imposes a fixed DTMF duration cap and immediate cleanup.
  - Inputs: DTMF playback elapsed time (`TickGet() - dtmf_tick`).
  - Outputs: File close, `statusReg.flags.audio_digit_playing = false`, DMA zeroing.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_digit_playing`, `EVID:code:src/src/audio_handler.cpp:dtmf_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-10: In manual dial flow, loop logic shall only submit a new local DTMF request when key state changed and `statusReg.flags.audio_digit_playing` is false.
  - Rationale: Legacy loop gating avoids overlapping local DTMF playback requests.
  - Inputs: Keypad keypress/release events and `statusReg.flags.audio_digit_playing`.
  - Outputs: Calls to `audio_dtmf_stop()` and `audio_dtmf(...)` only when gate conditions pass.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.audio_digit_playing`, `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-11: `dtmf_decode_task()` shall process an incoming DTMF command only when `statusReg.modem_response == ATRQTONEDET`, then read `statusReg.modem_dtmf_digit` and clear `statusReg.modem_response`.
  - Rationale: Legacy decode entry is keyed off modem response state and consumes that one-shot response.
  - Inputs: `statusReg.modem_response`, `statusReg.modem_dtmf_digit`.
  - Outputs: Cleared modem response state and internal digit-processing state updates.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/dtmf_handler.cpp`, `EVID:bus:statusReg.modem_response`, `EVID:bus:statusReg.modem_dtmf_digit`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SA-12: When `#` is received outside compare/program modes, `dtmf_decode_task()` shall enter PIN-compare mode and compare the next four digits against `configReg.PIN`.
  - Rationale: Legacy DTMF access control flow is explicitly implemented as hash-initiated 4-digit PIN comparison.
  - Inputs: Incoming DTMF digit stream and `configReg.PIN`.
  - Outputs: Compare-mode state and pass/fail compare result state.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/dtmf_handler.cpp`, `EVID:bus:configReg.PIN`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SA-13: On completion of PIN compare during an active call, `dtmf_decode_task()` shall restore modem speaker gain using handset or handsfree configured gain according to the current telephone mode.
  - Rationale: Legacy decode path mutes during secure entry and restores gain by mode-specific configuration.
  - Inputs: PIN-compare completion, `statusReg.flags.call_connected`, `statusReg.telephone_mode`, speaker gain config.
  - Outputs: `ds4420_set_gain(...)` with configured handset or handsfree speaker gain.
  - Execution context: loop
  - Evidence: `EVID:call:src/src/dtmf_handler.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.call_connected`, `EVID:bus:configReg.handset_PGA_speaker_gain`, `EVID:bus:configReg.handsfree_PGA_speaker_gain`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-14: If PIN comparison succeeds, `dtmf_decode_task()` shall set `generalFlags.load_activate_request` true.
  - Rationale: Legacy DTMF authorisation path raises a one-shot load activation request.
  - Inputs: Successful four-digit PIN comparison.
  - Outputs: `generalFlags.load_activate_request = true`.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.load_activate_request`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-15: In DTMF program mode, command sequence `*` then `*` shall set `gpio_controls.load_control` to `0` and invoke GPIO update.
  - Rationale: Legacy in-call DTMF programming supports explicit load deactivation command.
  - Inputs: DTMF program-mode digit sequence.
  - Outputs: `gpio_controls.load_control` write and GPIO refresh call.
  - Execution context: loop
  - Evidence: `EVID:bus:gpio_controls.load_control`, `EVID:call:src/src/dtmf_handler.cpp→src/src/gpio_handler.cpp`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-16: In DTMF program mode, command `'5'` followed by `'*'` shall return `true` from `dtmf_decode_task()` to request on-hook handling.
  - Rationale: Legacy DTMF command handling reserves this command pair for on-hook request signalling.
  - Inputs: DTMF program-mode command and terminator digit.
  - Outputs: `dtmf_decode_task()` return value used by loop caller.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/dtmf_handler.cpp`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-17: On `generalFlags.audio_test_request` in idle state, `audio_test_task()` shall configure microphone input/gain, clear `statusReg.flags.audio_test_passed`, reset test accumulators, and enter calibration state.
  - Rationale: Legacy audio self-test initialisation explicitly sets known audio-test input state and clears previous result state.
  - Inputs: `generalFlags.audio_test_request`.
  - Outputs: Mic config writes, accumulator resets, `statusReg.flags.audio_test_passed` reset.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:generalFlags.audio_test_request`, `EVID:bus:statusReg.flags.audio_test_passed`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-18: After approximately one second of calibration, `audio_test_task()` shall set `generalFlags.audio_test_tone_start_request`, clear tone-active state, and wait for modem tone start.
  - Rationale: Legacy self-test requires a calibrated baseline before requesting modem tone generation.
  - Inputs: Calibration elapsed time and calibration samples.
  - Outputs: `generalFlags.audio_test_tone_start_request = true`, tone state cleared.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:generalFlags.audio_test_tone_start_request`, `EVID:bus:statusReg.flags.audio_test_tone_active`, `EVID:bus:statusReg.audio_test_tone_tick`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-19: When modem tone active state is received, `audio_test_task()` shall sample microphone levels in the configured window and set `statusReg.flags.audio_test_passed` only when REAL-phase level exceeds calibration baseline by the legacy threshold.
  - Rationale: Legacy pass/fail decision is based on measured delta against calibration in REAL phase.
  - Inputs: `statusReg.flags.audio_test_tone_active`, `statusReg.audio_test_tone_tick`, microphone sample stream.
  - Outputs: `statusReg.flags.audio_test_passed` update.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_test_tone_active`, `EVID:bus:statusReg.audio_test_tone_tick`, `EVID:bus:statusReg.flags.audio_test_passed`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-20: The audio self-test flow shall run PRIME phase then REAL phase by internally reasserting `generalFlags.audio_test_request` after PRIME completion.
  - Rationale: Legacy implementation explicitly chains PRIME to REAL without returning to loop-level scheduling.
  - Inputs: PRIME phase completion.
  - Outputs: Internal request reassertion and REAL phase start.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:generalFlags.audio_test_request`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-21: On REAL phase completion, `audio_test_task()` shall set `statusReg.flags.audio_test_complete` true.
  - Rationale: Legacy loop orchestration depends on this completion flag to run cleanup/result handling.
  - Inputs: Completion of REAL test tone tail.
  - Outputs: `statusReg.flags.audio_test_complete = true`.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_test_complete`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-22: In AUDIO_TEST mode, loop logic shall assert `generalFlags.audio_test_request` only after modem setup completion, non-busy SMS state, and the legacy start delay.
  - Rationale: Legacy loop gates test start to avoid modem command overlap and premature sampling.
  - Inputs: `statusReg.flags.modem_process_complete`, `modem_sms_busy()`, elapsed test-mode time.
  - Outputs: `generalFlags.audio_test_request` assertion.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:bus:generalFlags.audio_test_request`, `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-23: When `statusReg.flags.audio_test_complete` is true, loop logic shall request modem cleanup and, after cleanup, restore downstream gains and disable temporary audio-path GPIO selections.
  - Rationale: Legacy post-test path restores persistent runtime audio settings before returning to idle.
  - Inputs: `statusReg.flags.audio_test_complete`, `statusReg.flags.audio_test_cleanup_complete`.
  - Outputs: `generalFlags.audio_test_cleanup_request`, restored PGA settings, GPIO control reset.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.audio_test_complete`, `EVID:bus:statusReg.flags.audio_test_cleanup_complete`, `EVID:bus:generalFlags.audio_test_cleanup_request`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: HIL

#### 2.1.3 Failure and recovery requirements (if evidenced)
- REQ-SA-24: If `generalFlags.audio_test_abort_request` is set, `audio_test_task()` shall immediately clear pending audio-test one-shot requests and return to idle.
  - Rationale: Legacy abort handling prevents stale test requests continuing after mode interruption.
  - Inputs: `generalFlags.audio_test_abort_request`.
  - Outputs: Cleared `audio_test_request` and `audio_test_tone_start_request`; idle state.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:generalFlags.audio_test_abort_request`, `EVID:bus:generalFlags.audio_test_request`, `EVID:bus:generalFlags.audio_test_tone_start_request`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-25: In tone-start wait state, if tone activation is not observed within the legacy timeout window, `audio_test_task()` shall set test result to failed, set complete, and return to idle.
  - Rationale: Legacy test path has explicit timeout-based fail-fast behaviour when modem tone cannot be started.
  - Inputs: `statusReg.flags.audio_test_tone_active`, elapsed tone-start wait time.
  - Outputs: `statusReg.flags.audio_test_passed = false`, `statusReg.flags.audio_test_complete = true`, idle state.
  - Execution context: task: audio
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.audio_test_tone_active`, `EVID:bus:statusReg.flags.audio_test_passed`, `EVID:bus:statusReg.flags.audio_test_complete`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SA-26: If DTMF compare/program input exceeds `DTMF_TIMEOUT_PERIOD`, `dtmf_decode_task()` shall abort command state and restore speaker gain when call-connected.
  - Rationale: Legacy DTMF control includes timeout-based recovery to normal in-call audio.
  - Inputs: DTMF command state active and timeout expiry.
  - Outputs: Compare/program state reset and speaker gain restoration.
  - Execution context: loop
  - Evidence: `EVID:call:src/src/dtmf_handler.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.flags.call_connected`, `EVID:bus:configReg.handset_PGA_speaker_gain`, `EVID:bus:configReg.handsfree_PGA_speaker_gain`, `EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-27: If audio-test cleanup does not complete inside the loop cleanup wait window, loop logic shall clear pending audio-test one-shot request flags.
  - Rationale: Legacy loop recovers from cleanup timeout to avoid stale state leaking into later call paths.
  - Inputs: Cleanup requested and elapsed cleanup wait time.
  - Outputs: Cleared `audio_test_request`, `test_audio_request`, `audio_test_tone_start_request`, `audio_test_cleanup_request`, and `audio_test_abort_request`.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.audio_test_request`, `EVID:bus:generalFlags.test_audio_request`, `EVID:bus:generalFlags.audio_test_tone_start_request`, `EVID:bus:generalFlags.audio_test_cleanup_request`, `EVID:bus:generalFlags.audio_test_abort_request`, `EVID:code:src/main.cpp:audio_test_clear_pending_requests`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-28: In AUDIO_TEST_ABORT mode, loop logic shall request test abort and cleanup, and after timeout shall place audio outputs in a safe muted/restored state before exiting abort mode.
  - Rationale: Legacy abort sequence explicitly prioritises safe audio state restoration before resuming normal modes.
  - Inputs: Transition into `AUDIO_TEST_ABORT`.
  - Outputs: `generalFlags.audio_test_abort_request`, `generalFlags.audio_test_cleanup_request`, muted/restored downstream audio settings.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.audio_test_abort_request`, `EVID:bus:generalFlags.audio_test_cleanup_request`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SA-29: On failed audio-test result, loop logic shall schedule up to three retry attempts with the legacy delay, and on third consecutive failure shall request an `AUDFAIL` SMS log.
  - Rationale: Legacy result policy retries transient failures before emitting final failure telemetry.
  - Inputs: `statusReg.flags.audio_test_complete`, `statusReg.flags.audio_test_passed`, attempt counter.
  - Outputs: Retry scheduling or `statusReg.sms_log_type = LOGTYPE_AUDFAIL` with `generalFlags.sms_request_log = true`.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.audio_test_complete`, `EVID:bus:statusReg.flags.audio_test_passed`, `EVID:bus:statusReg.sms_log_type`, `EVID:bus:generalFlags.sms_request_log`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-30: If a previously failed audio test later passes, loop logic shall request an `AUDPASS` SMS log and clear the previous-failure latch.
  - Rationale: Legacy behaviour reports recovery after prior failure, not only failure itself.
  - Inputs: Passing result with prior-failure latch set.
  - Outputs: `statusReg.sms_log_type = LOGTYPE_AUDPASS`, `generalFlags.sms_request_log = true`, failure latch clear.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.sms_log_type`, `EVID:bus:generalFlags.sms_request_log`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SA-31: During AUDIO_TEST mode, detection of incoming ring, user call-button events, or `modem_sms_busy()` shall force transition to AUDIO_TEST_ABORT.
  - Rationale: Legacy loop prioritises incoming telephony/SMS events over continuing audio-test execution.
  - Inputs: Ring flag, hook/handsfree button flags, SMS busy state.
  - Outputs: `statusReg.telephone_mode = AUDIO_TEST_ABORT` and deferred retry handling.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:code:src/main.cpp:loop`, `EVID:bus:statusReg.telephone_mode`
  - Linked invariants: TBD
  - Test strategy: integration

### Slice B (Modem + transport) / Core Subcluster 1
#### 2.2.1 Feature list
- FTR-SB-01: Main loop run-process lifecycle controls modem/service startup phases and transition into normal RUN operation. Evidence: `EVID:entry:setup`, `EVID:entry:loop`, `EVID:code:src/main.cpp:setup`, `EVID:code:src/main.cpp:loop`
- FTR-SB-02: Dedicated modem task executes modem initialisation, modem query cycles, and one-shot request handling. Evidence: `EVID:entry:task:ModemThread`, `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:code:src/main.cpp:modem_thread`, `EVID:code:src/src/modem_handler.cpp:modem_task`
- FTR-SB-03: Telephony submode logic bridges loop-context call events into modem task requests via shared bus flags. Evidence: `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:generalFlags.call_answer_request`, `EVID:bus:generalFlags.call_hangup_request`, `EVID:code:src/main.cpp:submode_task`
- FTR-SB-04: FTP connect/download/update/disconnect flow is implemented as staged loop logic coordinated with modem completion flags. Evidence: `EVID:bus:generalFlags.ftp_connect_request`, `EVID:bus:generalFlags.ftp_update_download_request`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/main.cpp:loop`, `EVID:code:src/src/modem_handler.cpp:modem_ftp_connect`
- FTR-SB-05: SMS inbound command decode and outbound queued send pipeline run in modem context with request/event queues. Evidence: `EVID:bus:statusReg.flags.incoming_sms_ready`, `EVID:bus:generalFlags.sms_request_user`, `EVID:bus:statusReg.flags.sms_inprogress`, `EVID:code:src/src/modem_handler.cpp:sms_decode`, `EVID:code:src/src/modem_handler.cpp:sms_task`
- FTR-SB-06: Thingstream callback bridge and response decoder update modem status, call state, and FTP state from modem URCs. Evidence: `EVID:entry:callback:Thingstream_Application_modemCallback`, `EVID:bus:statusReg.modem_response`, `EVID:bus:statusReg.modem_dtmf_digit`, `EVID:bus:statusReg.flags.modem_ftp_connected`, `EVID:code:src/src/modem_handler.cpp:Thingstream_Application_modemCallback`, `EVID:code:src/src/modem_handler.cpp:modem_decode`
- FTR-SB-07: Serial transport provides buffered send/receive, timeout-driven reads, callback dispatch, and line-based overflow handling. Evidence: `EVID:call:src/src/modem_handler.cpp→src/src/serial_transport.cpp`, `EVID:code:src/src/serial_transport.cpp:serial_send`, `EVID:code:src/src/serial_transport.cpp:serial_run`
- FTR-SB-08: Common parsing and normalisation helpers support modem response decode and SMS/config number handling. Evidence: `EVID:call:src/src/modem_handler.cpp→src/src/common.cpp`, `EVID:code:src/src/common.cpp:normalize_phone_number`, `EVID:code:src/src/common.cpp:getValue`, `EVID:code:src/src/common.cpp:comma_get_string`

#### 2.2.2 Functional requirements
- REQ-SB-01: In `setup()`, the firmware shall configure the 1 ms hardware timer ISR path and set `run_process` to `PREINIT_GO`.
  - Rationale: Legacy startup explicitly arms timer0/`SysTimer` and enters pre-initialisation mode in `setup`.
  - Inputs: Power-on reset entering `setup()`.
  - Outputs: Timer semaphore/timer alarm initialised; `run_process = PREINIT_GO`.
  - Execution context: setup
  - Evidence: `EVID:entry:setup`, `EVID:code:src/main.cpp:setup`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-02: In `PREINIT_GO`, loop logic shall initialise ADC, buttons, configuration storage/readback, and GPIO, then transition `run_process` to `PREINIT`.
  - Rationale: Legacy pre-initialisation stage executes these initialisers before normal bring-up can continue.
  - Inputs: `run_process == PREINIT_GO`.
  - Outputs: Initialiser calls completed; `run_process = PREINIT`.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/file_handler.cpp`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-03: In `INITIAL_GO`, loop logic shall create `ModemThread` and transition startup processing toward `INITIAL`.
  - Rationale: Legacy runtime depends on modem task creation during `INITIAL_GO`.
  - Inputs: `run_process == INITIAL_GO`.
  - Outputs: `xTaskCreate(... modem_thread ...)` called and run-process state advanced.
  - Execution context: loop
  - Evidence: `EVID:entry:task:ModemThread`, `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-04: In `RUN_GO`, loop logic shall set `generalFlags.wifi_request` true and transition to `RUN`.
  - Rationale: Legacy code requests initial Web UI availability at RUN entry.
  - Inputs: `run_process == RUN_GO`.
  - Outputs: `generalFlags.wifi_request = true`; `run_process = RUN`.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-05: `modem_thread` shall run `modem_task()` continuously with a 10-tick delay between iterations.
  - Rationale: Legacy modem processing is scheduled as a periodic dedicated task.
  - Inputs: `ModemThread` task start.
  - Outputs: Repeated `modem_task()` execution with `vTaskDelay(10)`.
  - Execution context: task: modem
  - Evidence: `EVID:entry:task:ModemThread`, `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:code:src/main.cpp:modem_thread`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-06: `modem_task()` shall execute modem power-up and initialisation stages and set modem active state before entering `MDM_RUN`.
  - Rationale: Legacy modem state machine performs staged bring-up with AT provisioning before run-mode servicing.
  - Inputs: `modem_process` state progression from power stages.
  - Outputs: Initialisation AT command sequence and transition into `MDM_RUN`.
  - Execution context: task: modem
  - Evidence: `EVID:entry:task:ModemThread`, `EVID:code:src/src/modem_handler.cpp:modem_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-07: `modem_init()` shall initialise UART1 and build the modem transport chain (`serial_transport_create`, ring-buffer transport, modem2 transport), and shall fail if any stage returns null.
  - Rationale: Legacy modem communication depends on this chained transport construction.
  - Inputs: `MDM_INIT_STAGE1` modem initialisation request.
  - Outputs: Valid modem transport pointer or failure return.
  - Execution context: task: modem
  - Evidence: `EVID:call:src/src/modem_handler.cpp→src/src/serial_transport.cpp`, `EVID:code:src/src/modem_handler.cpp:modem_init`
  - Linked invariants: TBD
  - Test strategy: mock

- REQ-SB-08: In `MDM_RUN`, `modem_task()` shall enter query stages periodically, setting `statusReg.flags.modem_process_inprogress` true before `MDM_QUERY_STAGE1` based on registration-dependent intervals.
  - Rationale: Legacy modem supervision alternates fast and normal query intervals using registration state.
  - Inputs: Query interval expiry and `statusReg.flags.modem_registered` state.
  - Outputs: `statusReg.flags.modem_process_inprogress = true`; `modem_process = MDM_QUERY_STAGE1`.
  - Execution context: task: modem
  - Evidence: `EVID:bus:statusReg.flags.modem_process_inprogress`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:call:src/src/modem_handler.cpp→src/src/tick.cpp`, `EVID:code:src/src/modem_handler.cpp:modem_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-09: In `MDM_RUN`, `modem_task()` shall service call one-shot requests (`call_initialisation`, `call_answer`, `call_hangup`, `call_audio`, `call_dtmf`, `call_audio_volume`) and clear each request flag after service.
  - Rationale: Legacy call control handshake between loop and modem task is flag-driven and one-shot.
  - Inputs: `generalFlags.call_initialisation_request`, `generalFlags.call_answer_request`, `generalFlags.call_hangup_request`, `generalFlags.call_audio_request`, `generalFlags.call_dtmf_request`, `generalFlags.call_audio_volume_request`.
  - Outputs: Corresponding modem call function calls and cleared request flags.
  - Execution context: task: modem
  - Evidence: `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:generalFlags.call_answer_request`, `EVID:bus:generalFlags.call_hangup_request`, `EVID:bus:generalFlags.call_audio_request`, `EVID:bus:generalFlags.call_dtmf_request`, `EVID:code:src/src/modem_handler.cpp:modem_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-10: In `CALL_INITIALISATION`, `submode_task()` shall set `generalFlags.call_initialisation_request` true, clear `statusReg.flags.modem_process_complete`, and transition to `CALL_CONTINUATION`.
  - Rationale: Legacy loop-to-modem call-start handshake is explicitly implemented in this submode.
  - Inputs: `statusReg.telephone_sub_mode == CALL_INITIALISATION`.
  - Outputs: One-shot request flag set; modem completion flag cleared; submode transition.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-11: In `CALL_ANSWER`, `submode_task()` shall set `generalFlags.call_answer_request` true, clear `statusReg.flags.modem_process_complete`, and transition to `CALL_CONTINUATION`.
  - Rationale: Legacy answer path uses the same one-shot modem completion handshake as call initialisation.
  - Inputs: `statusReg.telephone_sub_mode == CALL_ANSWER`.
  - Outputs: Answer request raised, completion flag cleared, continuation submode entered.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:bus:generalFlags.call_answer_request`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-12: In `CALL_CONTINUATION`, loop logic shall request modem call-audio setup by asserting `generalFlags.call_audio_request` and clearing `statusReg.flags.modem_process_complete` when CONNECT status is observed or fallback timeout expires.
  - Rationale: Legacy call audio setup is triggered either by DSCI CONNECT URC or by a delayed fallback path.
  - Inputs: `statusReg.modem_call_status_counter` updates and fallback timer state.
  - Outputs: `generalFlags.call_audio_request = true`; `statusReg.flags.modem_process_complete = false`.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.call_audio_request`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/main.cpp:set_call_audio_request`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-13: `call_audio_setup()` shall configure call-path codec registers and complete by setting `statusReg.flags.modem_process_complete` true.
  - Rationale: Legacy modem-side audio setup writes fixed codec registers and mode-dependent gains before signalling completion.
  - Inputs: `generalFlags.call_audio_request`.
  - Outputs: AT codec configuration commands and completion flag set.
  - Execution context: task: modem
  - Evidence: `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/src/modem_handler.cpp:call_audio_setup`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SB-14: In `CALL_CONTINUATION`, keypad digit changes shall be copied into `statusReg.modem_dtmf_digit` and shall assert `generalFlags.call_dtmf_request`.
  - Rationale: Legacy in-call keypad transmit path is implemented via shared DTMF digit and one-shot flag.
  - Inputs: Keypad press state change during active call continuation.
  - Outputs: `statusReg.modem_dtmf_digit` write and `generalFlags.call_dtmf_request = true`.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.modem_dtmf_digit`, `EVID:bus:generalFlags.call_dtmf_request`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-15: `call_volume_update(volume)` shall send mute when `volume == 0`, and for volumes `1..10` shall map to modem `ATCLVL` range `0..5`.
  - Rationale: Legacy modem speaker volume is represented as a constrained user value mapped onto modem command scale.
  - Inputs: `generalFlags.call_audio_volume_request` with `statusReg.modem_call_volume`.
  - Outputs: `ATCMUT` or mapped `ATCLVL` modem command.
  - Execution context: task: modem
  - Evidence: `EVID:code:src/src/modem_handler.cpp:call_volume_update`, `EVID:code:src/src/modem_handler.cpp:modem_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SB-16: In `FTP_GO`, loop logic shall reset FTP status flags, assert `generalFlags.ftp_connect_request`, clear modem completion state, and set the FTP sub-process to await connect.
  - Rationale: Legacy FTP mode initialisation establishes a known clean state before FTP stage progression.
  - Inputs: `statusReg.telephone_mode == FTP_GO`.
  - Outputs: FTP status flags reset; `generalFlags.ftp_connect_request = true`; `statusReg.flags.modem_process_complete = false`; `ftp_process = FTP_AWAIT_CONNECT`.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.ftp_connect_request`, `EVID:bus:statusReg.flags.modem_ftp_connected`, `EVID:bus:statusReg.flags.ftp_open_failed`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-17: In `FTP` mode, loop logic shall sequence FTP requests (`directory`, `download`, `update_size`, `update_info`, conditional `update_download`, `disconnect`) using modem completion gates and legacy inter-stage delays.
  - Rationale: Legacy FTP behaviour is a deterministic staged state machine in loop context.
  - Inputs: `ftp_process` state, `statusReg.flags.modem_process_complete`, elapsed stage timers.
  - Outputs: One-shot FTP request flags set/cleared and FTP stage transitions.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.ftp_directory_request`, `EVID:bus:generalFlags.ftp_download_request`, `EVID:bus:generalFlags.ftp_update_size_request`, `EVID:bus:generalFlags.ftp_update_info_request`, `EVID:bus:generalFlags.ftp_update_download_request`, `EVID:bus:generalFlags.ftp_disconnect_request`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-18: In `MDM_RUN`, `modem_task()` shall service each FTP one-shot request (`connect`, `directory`, `download`, `update_size`, `update_info`, `update_download`, `disconnect`) and clear the serviced flag.
  - Rationale: Legacy loop-to-modem FTP handshake is one-shot and acknowledged by flag clear.
  - Inputs: FTP request flags raised by loop logic.
  - Outputs: Corresponding `modem_ftp_*` calls and cleared request flags.
  - Execution context: task: modem
  - Evidence: `EVID:bus:generalFlags.ftp_connect_request`, `EVID:bus:generalFlags.ftp_directory_request`, `EVID:bus:generalFlags.ftp_download_request`, `EVID:bus:generalFlags.ftp_update_size_request`, `EVID:bus:generalFlags.ftp_update_info_request`, `EVID:bus:generalFlags.ftp_update_download_request`, `EVID:bus:generalFlags.ftp_disconnect_request`, `EVID:code:src/src/modem_handler.cpp:modem_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-19: `modem_ftp_connect()` shall compose FTP account/server arguments from `configReg` FTP fields and set `statusReg.flags.modem_process_complete` true after issuing connect commands.
  - Rationale: Legacy FTP connection setup uses configured credentials/server endpoint and explicit completion signalling.
  - Inputs: `generalFlags.ftp_connect_request`.
  - Outputs: `ATQFTPCFGAC` and `ATQFTPOPEN` modem commands; completion flag set.
  - Execution context: task: modem
  - Evidence: `EVID:bus:configReg.ftp_user`, `EVID:bus:configReg.ftp_password`, `EVID:bus:configReg.ftp_server`, `EVID:bus:configReg.ftp_port`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/src/modem_handler.cpp:modem_ftp_connect`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-20: `modem_ftp_directory()` shall construct the directory path using `configReg.ftp_directory` and the phone serial suffix, then set `statusReg.flags.modem_process_complete` true.
  - Rationale: Legacy FTP directory selection is derived from configured base directory plus device identity.
  - Inputs: `generalFlags.ftp_directory_request`.
  - Outputs: `ATQFTPCWD` directory command; completion flag set.
  - Execution context: task: modem
  - Evidence: `EVID:bus:configReg.ftp_directory`, `EVID:bus:statusReg.flags.modem_process_complete`, `EVID:code:src/src/modem_handler.cpp:modem_ftp_directory`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-21: When software update download succeeds, loop logic shall copy `statusReg.ftp_file_date` into `configReg.ftp_last_update_date` and set `generalFlags.config_changed` true.
  - Rationale: Legacy behaviour persists the downloaded update date to suppress redundant update downloads.
  - Inputs: FTP update-download completion with success status.
  - Outputs: Updated `configReg.ftp_last_update_date` and config-write request flag.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.ftp_download_complete`, `EVID:bus:statusReg.ftp_file_date`, `EVID:bus:configReg.ftp_last_update_date`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-22: `Thingstream_Application_modemCallback()` shall forward modem callback payloads to `modem_callback()`.
  - Rationale: Legacy callback bridge is a direct pass-through to the internal modem callback handler.
  - Inputs: Thingstream callback payload and length.
  - Outputs: Invocation of `modem_callback(NULL, response, len)`.
  - Execution context: Thingstream callback
  - Evidence: `EVID:entry:callback:Thingstream_Application_modemCallback`, `EVID:code:src/src/modem_handler.cpp:Thingstream_Application_modemCallback`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SB-23: `modem_callback()` shall route inbound text payloads by request context: SMS payloads to the SMS receive buffer, FTP text payloads to the FTP buffer (non-update path), and all other payloads to `modem_decode()`.
  - Rationale: Legacy callback logic uses request flags to demultiplex payload handling.
  - Inputs: Callback payload plus `generalFlags.sms_receive_request` / `generalFlags.ftp_file_request` state.
  - Outputs: `statusReg.flags.incoming_sms_ready` or `statusReg.flags.ftp_data_ready` updates, or modem decode call.
  - Execution context: Thingstream callback
  - Evidence: `EVID:bus:generalFlags.ftp_file_request`, `EVID:bus:statusReg.flags.incoming_sms_ready`, `EVID:code:src/src/modem_handler.cpp:modem_callback`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-24: `modem_decode()` shall convert `ATR_CMT` and `ATR_RING` URCs into legacy bus updates for inbound SMS/ring signalling.
  - Rationale: Legacy loop and modem task logic consume these decoded flags to start SMS processing and incoming-call handling.
  - Inputs: URC lines matching `ATR_CMT` or `ATR_RING`.
  - Outputs: SMS originator capture with `generalFlags.sms_receive_request`, and ring detection flag updates.
  - Execution context: Thingstream callback
  - Evidence: `EVID:bus:statusReg.flags.incoming_call_ring_detected`, `EVID:bus:statusReg.modem_response`, `EVID:code:src/src/modem_handler.cpp:modem_decode`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SB-25: `modem_decode()` shall parse network/call quality responses and update modem response and level fields used by loop call-control logic.
  - Rationale: Legacy no-network and low-signal branches depend on parsed modem response and RX/signal metrics.
  - Inputs: URCs/responses such as `ATR_CSQ`, `ATR_CREG`, and `ATR_QENG`.
  - Outputs: `statusReg.modem_response`, `statusReg.modem_signal_level`, and `statusReg.modem_rx_level` updates.
  - Execution context: Thingstream callback
  - Evidence: `EVID:bus:statusReg.modem_response`, `EVID:bus:statusReg.modem_signal_level`, `EVID:bus:statusReg.modem_rx_level`, `EVID:code:src/src/modem_handler.cpp:modem_decode`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SB-26: `modem_decode()` shall parse FTP URCs and update legacy FTP connection/result fields (`modem_ftp_connected`, open/directory/file failure flags, download complete, file size, and file date).
  - Rationale: Loop FTP stage transitions and error handling are driven entirely by these decoded status fields.
  - Inputs: FTP URCs including `+QFTPOPEN`, `+QFTPCWD`, `+QFTPGET`, `+QFTPCLOSE`, `+QFTPSIZE`, and `+QFTPMDTM`.
  - Outputs: FTP status flags and metadata fields on the shared bus.
  - Execution context: Thingstream callback
  - Evidence: `EVID:bus:statusReg.flags.modem_ftp_connected`, `EVID:bus:statusReg.flags.ftp_open_failed`, `EVID:bus:statusReg.flags.ftp_directory_not_found`, `EVID:bus:statusReg.flags.ftp_file_not_found`, `EVID:bus:statusReg.flags.ftp_download_complete`, `EVID:bus:statusReg.ftp_file_size`, `EVID:bus:statusReg.ftp_file_date`, `EVID:code:src/src/modem_handler.cpp:modem_decode`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-27: `sms_decode()` shall authenticate inbound commands by validating legacy PIN position (offset 3, or 4 for ESIM commands) and whitelist eligibility before command execution.
  - Rationale: Legacy SMS command acceptance explicitly gates on both PIN and whitelist checks.
  - Inputs: Received SMS command payload and originator number.
  - Outputs: Command processing allowed or rejected as `INVALID`.
  - Execution context: task: modem
  - Evidence: `EVID:bus:configReg.PIN`, `EVID:code:src/src/modem_handler.cpp:sms_decode`, `EVID:code:src/src/modem_handler.cpp:sms_whitelist_allowed`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SB-28: For accepted SMS commands, `sms_decode()` shall append sender/time to SMS logs, set `generalFlags.config_changed`, set `statusReg.flags.sms_command_processed`, and apply command side effects (including FTP and reset requests).
  - Rationale: Legacy SMS decode both records provenance and mutates runtime/config request flags per command.
  - Inputs: Authenticated SMS command payload.
  - Outputs: Log entry updates, config-write request, command-processed flag, and command-specific request flags.
  - Execution context: task: modem
  - Evidence: `EVID:bus:generalFlags.config_changed`, `EVID:bus:generalFlags.ftp_check_request`, `EVID:bus:statusReg.flags.sms_command_processed`, `EVID:code:src/src/modem_handler.cpp:sms_decode`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-29: `sms_task()` shall collect SMS request flags/events, build outbound queue entries, and drive modem send attempts while maintaining SMS in-progress state.
  - Rationale: Legacy outbound SMS delivery is queue-based and mediated by periodic `sms_task` execution.
  - Inputs: SMS request flags/events and outbound queue contents.
  - Outputs: Outbound queue/message state progression and SMS transmission attempts.
  - Execution context: task: modem
  - Evidence: `EVID:bus:generalFlags.sms_request_user`, `EVID:bus:generalFlags.sms_request_log`, `EVID:bus:statusReg.flags.sms_inprogress`, `EVID:code:src/src/modem_handler.cpp:sms_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-30: `serial_run()` shall wait up to the requested timeout for the first byte, then use a 10 ms rolling timeout for subsequent bytes, and shall invoke the registered callback when bytes are received.
  - Rationale: Legacy serial transport timing semantics are explicitly implemented for modem response collection.
  - Inputs: `serial_run(..., millis)` call and incoming serial stream bytes.
  - Outputs: Callback invocation with buffered bytes and transport return status.
  - Execution context: task: modem
  - Evidence: `EVID:call:src/src/modem_handler.cpp→src/src/serial_transport.cpp`, `EVID:code:src/src/serial_transport.cpp:serial_run`
  - Linked invariants: TBD
  - Test strategy: mock

- REQ-SB-31: In line-based mode, `serial_run()` shall treat partial-line timeout/overflow as overflow state and shall discard the immediate subsequent remainder line before resuming normal callback delivery.
  - Rationale: Legacy line-based transport logic preserves line boundaries by explicit overflow latching/discard.
  - Inputs: Line-based receive path with timeout or buffer overflow before newline.
  - Outputs: `TRANSPORT_READ_OVERFLOW` returns and discarded partial-line data.
  - Execution context: task: modem
  - Evidence: `EVID:code:src/src/serial_transport.cpp:serial_run`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SB-32: `normalize_phone_number()` shall normalise phone strings by retaining digits, permitting one leading plus, converting leading-zero 11+ digit numbers to `+44`, and returning `"0"` for invalid/overflow inputs.
  - Rationale: Legacy SMS/FTP/config number updates route through this canonicalisation logic.
  - Inputs: Raw phone number strings from SMS/JSON/config processing.
  - Outputs: Normalised number string in output buffer with legacy fallback behaviour.
  - Execution context: task: modem
  - Evidence: `EVID:call:src/src/modem_handler.cpp→src/src/common.cpp`, `EVID:code:src/src/common.cpp:normalize_phone_number`
  - Linked invariants: TBD
  - Test strategy: unit

#### 2.2.3 Failure and recovery requirements (if evidenced)
- REQ-SB-33: `modem_send()` shall retry timeout/invalid command results up to `MAX_AT_RETRIES`, and on retry exhaustion shall mark modem not-ready and force modem process restart.
  - Rationale: Legacy modem command path includes explicit retry ceiling and forced modem recovery on repeated transport failure.
  - Inputs: AT command send result status (`TIMEOUT`/`INVALID`) and retry count.
  - Outputs: Retry attempts, then `statusReg.flags.modem_ready = false` and `modem_process = MDM_RESTART`.
  - Execution context: task: modem
  - Evidence: `EVID:code:src/src/modem_handler.cpp:modem_send`
  - Linked invariants: TBD
  - Test strategy: mock

- REQ-SB-34: In FTP mode, loop logic shall abort FTP progression when FTP open/directory/file failures or stage timeout occur; on open failure it shall return to `IDLE_RUN`, otherwise it shall request FTP disconnect.
  - Rationale: Legacy FTP process has explicit error/timeout abort behaviour to recover to a safe run mode.
  - Inputs: `statusReg.flags.ftp_open_failed`, `statusReg.flags.ftp_directory_not_found`, `statusReg.flags.ftp_file_not_found`, FTP stage timeout.
  - Outputs: Either `statusReg.telephone_mode = IDLE_RUN` or `generalFlags.ftp_disconnect_request = true`.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.ftp_open_failed`, `EVID:bus:statusReg.flags.ftp_directory_not_found`, `EVID:bus:statusReg.flags.ftp_file_not_found`, `EVID:bus:generalFlags.ftp_disconnect_request`, `EVID:bus:statusReg.telephone_mode`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-35: `call_initialisation()` shall force `statusReg.telephone_sub_mode` to `HANG_UP` when modem dial command response is not `MDMOK`.
  - Rationale: Legacy call-start error path fails safe by immediately routing to hang-up.
  - Inputs: Non-OK response from modem `ATD` command.
  - Outputs: `statusReg.telephone_sub_mode = HANG_UP`.
  - Execution context: task: modem
  - Evidence: `EVID:bus:statusReg.telephone_sub_mode`, `EVID:code:src/src/modem_handler.cpp:call_initialisation`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-36: In `CALL_CONTINUATION`, loop logic shall transition to `NO_NETWORK` on pre-connect `ATRNOCARRIER` when no-network response is enabled, and shall transition to `NO_NETWORK_WITH_HANGUP` when low-signal call-disable is enabled and RX level is below configured threshold.
  - Rationale: Legacy call recovery includes separate no-network and low-signal protective submodes before full call connection.
  - Inputs: `statusReg.modem_response`, `configReg.flags.no_network_response_enabled`, `configReg.flags.low_signal_call_disable_enabled`, `statusReg.modem_rx_level`, `configReg.call_disable_low_signal_level`.
  - Outputs: `statusReg.telephone_sub_mode` transition to `NO_NETWORK` or `NO_NETWORK_WITH_HANGUP`.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.modem_response`, `EVID:bus:configReg.flags.no_network_response_enabled`, `EVID:bus:configReg.flags.low_signal_call_disable_enabled`, `EVID:bus:statusReg.modem_rx_level`, `EVID:bus:configReg.call_disable_low_signal_level`, `EVID:bus:statusReg.telephone_sub_mode`, `EVID:code:src/main.cpp:submode_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-37: In `MDM_QUERY_STAGE1`, if modem was previously registered but is now unregistered, `modem_task()` shall request modem reboot and return to modem initialisation stages.
  - Rationale: Legacy network recovery path explicitly reboots modem on de-registration after first successful registration.
  - Inputs: `statusReg.flags.modem_first_registration == true` and `statusReg.flags.modem_registered == false`.
  - Outputs: Modem reboot command and transition back to init stages.
  - Execution context: task: modem
  - Evidence: `EVID:bus:statusReg.flags.modem_registered`, `EVID:code:src/src/modem_handler.cpp:modem_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-38: `sim_switch_guard_handle()` shall revert SIM selection and raise modem reboot requests on first no-SIM result during pending SIM switch, and on fallback no-SIM shall set both-SIM-failed state and clear the guard.
  - Rationale: Legacy SIM-switch guard provides explicit fallback and terminal-failure handling to avoid endless retries.
  - Inputs: `sim_switch_guard.pending`, `statusReg.flags.modem_no_sim`, fallback state.
  - Outputs: SIM control revert and reboot request flags, or both-failed latch with guard clear.
  - Execution context: task: modem
  - Evidence: `EVID:bus:statusReg.flags.modem_no_sim`, `EVID:bus:generalFlags.sim_switch_reboot_request`, `EVID:bus:generalFlags.modem_reboot_request`, `EVID:bus:generalFlags.gpio_update_request`, `EVID:bus:configReg.flags.expansion_sim_enabled`, `EVID:code:src/src/modem_handler.cpp:sim_switch_guard_handle`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SB-39: `sms_task()` shall retry failed or timed-out SMS sends up to `SMS_MAX_RETRIES`, and after retry exhaustion shall drop the active message and remaining messages in the same multipart batch.
  - Rationale: Legacy SMS sender includes bounded retry with batch drop to recover from persistent send failures.
  - Inputs: SMS send timeout/error states and retry counters.
  - Outputs: Retry attempts, then queue/message drop and sender state reset.
  - Execution context: task: modem
  - Evidence: `EVID:bus:statusReg.flags.sms_inprogress`, `EVID:code:src/src/modem_handler.cpp:sms_task`
  - Linked invariants: TBD
  - Test strategy: integration

### Slice C (WiFi + HTTP + file/config) / Core Subcluster 1
#### 2.3.1 Feature list
- FTR-SC-01: SPIFFS-backed configuration persistence with defaulting, legacy migration, and full snapshot write/read. Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/file_handler.cpp`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/src/file_handler.cpp:config_read`, `EVID:code:src/src/file_handler.cpp:config_write`
- FTR-SC-02: WiFi AP lifecycle and client presence tracking driven by loop control and WiFi station callbacks. Evidence: `EVID:entry:loop`, `EVID:entry:callback:WiFi AP STA connected`, `EVID:entry:callback:WiFi AP STA disconnected`, `EVID:call:src/main.cpp→src/src/wifi_handler.cpp`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`
- FTR-SC-03: Session-cookie web authentication with role-gated access and staged login lockout handling. Evidence: `EVID:entry:callback:HTTP route /login (POST)`, `EVID:entry:callback:HTTP route /logout (GET)`, `EVID:bus:configReg.wifi_web_user1`, `EVID:bus:configReg.wifi_web_password1`, `EVID:bus:configReg.wifi_web_salt1`, `EVID:code:src/src/wifi_handler.cpp:authenticate_credentials`
- FTR-SC-04: HTTP command mutation plane updates runtime/config state, GPIO controls, whitelist state, and SIM-switch signalling. Evidence: `EVID:entry:callback:HTTP not-found route`, `EVID:bus:generalFlags.config_changed`, `EVID:bus:generalFlags.gpio_update_request`, `EVID:bus:generalFlags.sms_request_user`, `EVID:bus:statusReg.sms_log_type`, `EVID:code:src/src/wifi_handler.cpp:handle_command_requests`
- FTR-SC-05: XML/JSON configuration and telemetry transfer endpoints with authenticated config export/import. Evidence: `EVID:entry:callback:HTTP route /values.xml`, `EVID:entry:callback:HTTP route /setup.xml`, `EVID:entry:callback:HTTP route /config.xml`, `EVID:entry:callback:HTTP route /permissions.xml`, `EVID:entry:callback:HTTP route /config.json (GET)`, `EVID:entry:callback:HTTP route /config_upload (POST body)`, `EVID:code:src/src/wifi_handler.cpp:handleConfigJSON`, `EVID:code:src/src/wifi_handler.cpp:handleConfigUpload`
- FTR-SC-06: OTA and SPIFFS upload handlers stream payloads through authenticated HTTP callbacks with update-state signalling. Evidence: `EVID:entry:callback:HTTP route /upload (POST body)`, `EVID:entry:callback:HTTP route /config (POST body)`, `EVID:bus:statusReg.flags.wifi_update_in_progress`, `EVID:bus:generalFlags.force_led_update`, `EVID:code:src/src/wifi_handler.cpp:handle_upload`, `EVID:code:src/src/wifi_handler.cpp:handleDoUpdate`

#### 2.3.2 Functional requirements
- REQ-SC-01: `config_init()` shall mount SPIFFS and return `true` only when mount succeeds.
  - Rationale: Legacy config initialisation gates all subsequent read/write operations on SPIFFS mount success.
  - Inputs: Loop initialisation call into `config_init()`.
  - Outputs: SPIFFS mount attempt and boolean success/failure result.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/file_handler.cpp`, `EVID:code:src/src/file_handler.cpp:config_init`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-02: When `/config.ini` is missing or opened as a directory, `config_read()` shall call `config_default()`, call `config_write()`, and return without further parse.
  - Rationale: Legacy config path auto-creates persistent defaults on first boot or invalid file state.
  - Inputs: `config_read()` call with absent/invalid `/config.ini`.
  - Outputs: Defaulted `configReg` and newly written `/config.ini`.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/file_handler.cpp`, `EVID:code:src/src/file_handler.cpp:config_read`, `EVID:code:src/src/file_handler.cpp:config_default`, `EVID:code:src/src/file_handler.cpp:config_write`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-03: `config_read()` shall parse newline-ordered legacy fields into `configReg` and clamp `configReg.no_sms_logs` and `configReg.no_web_logs` to the range `0..10`.
  - Rationale: Legacy parser applies positional field decode and explicit bounds checks for persisted log counts.
  - Inputs: `/config.ini` line stream.
  - Outputs: Updated `configReg` fields with bounded log-count values.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.no_sms_logs`, `EVID:bus:configReg.no_web_logs`, `EVID:code:src/src/file_handler.cpp:config_read`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SC-04: On legacy marker value `5846324`, `config_read()` shall apply factory slot defaults, migrate legacy plaintext web passwords to salted hashes, initialise whitelist fields to disabled/empty, and set `generalFlags.config_changed` true.
  - Rationale: Legacy config migration path upgrades authentication and whitelist fields when older file format is detected.
  - Inputs: Marker line read from `/config.ini`.
  - Outputs: Updated `configReg.wifi_web_*` and whitelist fields plus `generalFlags.config_changed = true`.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.wifi_web_password1`, `EVID:bus:configReg.wifi_web_salt1`, `EVID:bus:configReg.whitelist_enabled`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/src/file_handler.cpp:config_read`, `EVID:code:src/src/file_handler.cpp:migrate_legacy_passwords`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-05: For newer-format config files, `config_read()` shall require terminal marker `5846325`; when absent, it shall recreate configuration using `config_default()` and `config_write()`.
  - Rationale: Legacy parser uses terminal marker validation to recover from stale or truncated persisted config.
  - Inputs: Terminal marker parse result.
  - Outputs: Either accepted config load or recreated default config file.
  - Execution context: loop
  - Evidence: `EVID:code:src/src/file_handler.cpp:config_read`, `EVID:code:src/src/file_handler.cpp:config_default`, `EVID:code:src/src/file_handler.cpp:config_write`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-06: After config parse, `config_read()` shall normalise `configReg.whitelist_no1..3`, and when slot-1 credentials are missing/invalid it shall regenerate factory slot username/hash/salt and set `generalFlags.config_changed` true.
  - Rationale: Legacy load path enforces canonical whitelist values and guarantees a valid factory account.
  - Inputs: Parsed config values for whitelist and slot-1 credentials.
  - Outputs: Normalised whitelist numbers and regenerated factory credentials when required.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.whitelist_no1`, `EVID:bus:configReg.wifi_web_user1`, `EVID:bus:configReg.wifi_web_password1`, `EVID:bus:configReg.wifi_web_salt1`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/src/file_handler.cpp:config_read`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-07: `config_write()` shall serialise the active `configReg` snapshot, including auth and whitelist extensions, and append terminal marker `5846325`.
  - Rationale: Legacy persistence writes a complete ordered text snapshot with explicit version terminator.
  - Inputs: Current `configReg` values.
  - Outputs: Updated `/config.ini` content with terminal marker.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.UIN.val`, `EVID:bus:configReg.wifi_web_enabled1`, `EVID:bus:configReg.whitelist_enabled`, `EVID:code:src/src/file_handler.cpp:config_write`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-08: `config_default()` shall set factory defaults, generate salted/hash factory credentials for slot 1, clear credentials for slots 2/3, disable whitelist, and clear SMS/web/auth log counters.
  - Rationale: Legacy default path establishes deterministic baseline values for runtime and web access control.
  - Inputs: Defaulting request.
  - Outputs: Defaulted `configReg` fields and cleared log counters.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.wifi_web_user1`, `EVID:bus:configReg.wifi_web_password1`, `EVID:bus:configReg.wifi_web_salt1`, `EVID:bus:configReg.wifi_web_enabled1`, `EVID:bus:configReg.wifi_web_role1`, `EVID:bus:configReg.whitelist_enabled`, `EVID:bus:configReg.no_sms_logs`, `EVID:bus:configReg.no_web_logs`, `EVID:bus:configReg.no_auth_logs`, `EVID:code:src/src/file_handler.cpp:config_default`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SC-09: In `loop`, when `generalFlags.wifi_request` is set, web server is not started, and call/off-hook guards are clear, firmware shall call `wifi_start_ap()` and reset WiFi session timing trackers.
  - Rationale: Legacy loop state machine gates AP startup on explicit request plus telephony safety conditions.
  - Inputs: `generalFlags.wifi_request`, `statusReg.flags.wifi_server_started`, call/off-hook state flags.
  - Outputs: AP startup call and reset of `wifi_tick`/`wifi_session_start_tick`/`wifi_debug_tick`.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/wifi_handler.cpp`, `EVID:bus:generalFlags.wifi_request`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-10: While web server is started, loop logic shall call `wifi_check_client_timeouts()` and shall stop AP on no-client startup timeout, disconnected-client timeout, or manual deactivate request.
  - Rationale: Legacy loop contains explicit timeout and user-request shutdown paths for AP lifecycle control.
  - Inputs: `statusReg.flags.wifi_server_started`, `statusReg.flags.wifi_server_connected`, `statusReg.flags.wifi_server_disconnected`, `generalFlags.wifi_request`, `generalFlags.wifi_deactivate_request`.
  - Outputs: `wifi_stop_ap()` call and reset of WiFi timing trackers.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/wifi_handler.cpp`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`, `EVID:bus:generalFlags.wifi_deactivate_request`, `EVID:code:src/main.cpp:loop`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-11: `wifi_start_ap()` shall derive AP SSID from `configReg.phone_loc` when `configReg.flags.location_text_setup` is true, otherwise from legacy base SSID plus SoftAP MAC suffix.
  - Rationale: Legacy AP naming path conditionally uses configured location text with MAC-based fallback naming.
  - Inputs: `configReg.flags.location_text_setup`, `configReg.phone_loc`, device MAC reads.
  - Outputs: SSID string used for `WiFi.softAP(...)`.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.flags.location_text_setup`, `EVID:bus:configReg.phone_loc`, `EVID:code:src/src/wifi_handler.cpp:wifi_start_ap`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SC-12: `wifi_start_ap()` shall register WiFi station callbacks and HTTP routes, call `server.begin()`, and set `statusReg.flags.wifi_server_started` true only after AP IP validation and server start completion.
  - Rationale: Legacy AP bring-up explicitly stages callback/route registration, server start, and final started-flag assertion.
  - Inputs: AP start request from loop.
  - Outputs: Registered callbacks/routes, server start, and `statusReg.flags.wifi_server_started`.
  - Execution context: loop
  - Evidence: `EVID:entry:callback:WiFi AP STA connected`, `EVID:entry:callback:WiFi AP STA disconnected`, `EVID:entry:callback:HTTP route /values.xml`, `EVID:entry:callback:HTTP route /config_upload (POST body)`, `EVID:entry:callback:HTTP not-found route`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:code:src/src/wifi_handler.cpp:wifi_start_ap`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-13: `wifi_stop_ap()` shall switch WiFi to `WIFI_OFF` and clear `wifi_server_started`, `wifi_server_connected`, and `wifi_server_disconnected` status flags.
  - Rationale: Legacy AP shutdown path uses WiFi-off mode and explicit flag reset for deterministic web-state teardown.
  - Inputs: Loop-triggered AP shutdown request.
  - Outputs: WiFi mode off and cleared web-state flags.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/wifi_handler.cpp`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`, `EVID:code:src/src/wifi_handler.cpp:wifi_stop_ap`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-14: `wifi_check_client_timeouts()` shall periodically reconcile tracked clients with driver station list and update connected/disconnected flags according to combined tracking and station counts.
  - Rationale: Legacy disconnect detection uses multi-layer client-state reconciliation rather than a single source.
  - Inputs: Periodic time check, `esp_wifi_ap_get_sta_list`, `WiFi.softAPgetStationNum`.
  - Outputs: `statusReg.flags.wifi_server_connected`, `statusReg.flags.wifi_server_disconnected`, and internal client tracking updates.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`, `EVID:code:src/src/wifi_handler.cpp:wifi_check_client_timeouts`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-15: `wifi_connect_event()` shall update connected/disconnected flags, refresh activity tracking for newly observed clients, append web connection logs with depth capped at ten, and set `generalFlags.config_changed` true.
  - Rationale: Legacy station-connect callback both updates runtime connectivity flags and records persistent web access logs.
  - Inputs: WiFi AP STA connected event.
  - Outputs: Updated connection flags, `configReg.logs[].web_log` update, `configReg.no_web_logs` update, and `generalFlags.config_changed = true`.
  - Execution context: wifi event callback
  - Evidence: `EVID:entry:callback:WiFi AP STA connected`, `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`, `EVID:bus:configReg.no_web_logs`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/src/wifi_handler.cpp:wifi_connect_event`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-16: `wifi_disconnect_event()` shall call `wifi_check_client_timeouts()` and set disconnected state when station count reaches zero, otherwise keep connected state with updated user count.
  - Rationale: Legacy disconnect callback immediately re-evaluates station tracking and branches behaviour on remaining client count.
  - Inputs: WiFi AP STA disconnected event.
  - Outputs: Updated connected/disconnected flags and user-count bookkeeping.
  - Execution context: wifi event callback
  - Evidence: `EVID:entry:callback:WiFi AP STA disconnected`, `EVID:bus:statusReg.flags.wifi_server_connected`, `EVID:bus:statusReg.flags.wifi_server_disconnected`, `EVID:code:src/src/wifi_handler.cpp:wifi_disconnect_event`, `EVID:code:src/src/wifi_handler.cpp:wifi_check_client_timeouts`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-17: `handle_web_requests()` shall track request activity, permit direct `/login.html` access, require valid session cookie for other paths, and dispatch authenticated requests through command then file handling.
  - Rationale: Legacy not-found route handler is the central authenticated dispatcher for dynamic commands and static asset delivery.
  - Inputs: HTTP not-found route callback request.
  - Outputs: Login redirect, command handling, file response, or 404 response.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP not-found route`, `EVID:code:src/src/wifi_handler.cpp:handle_web_requests`, `EVID:code:src/src/wifi_handler.cpp:track_request_activity`, `EVID:code:src/src/wifi_handler.cpp:handle_command_requests`, `EVID:code:src/src/wifi_handler.cpp:load_file`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-18: `ensure_authenticated()` shall return true only when `session_cookie_valid()` succeeds; otherwise it shall redirect the request to `/login.html` and return false.
  - Rationale: Legacy helper enforces cookie-gated route access with uniform redirect behaviour.
  - Inputs: HTTP request requiring authentication.
  - Outputs: Boolean auth gate result and optional redirect response.
  - Execution context: http route handler
  - Evidence: `EVID:code:src/src/wifi_handler.cpp:ensure_authenticated`, `EVID:code:src/src/wifi_handler.cpp:session_cookie_valid`, `EVID:code:src/src/wifi_handler.cpp:redirect_to_login`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SC-19: `/login` POST handling shall enforce lockout response behaviour, authenticate submitted credentials, and on success shall start session state, set auth cookie, and redirect to `/index.html`.
  - Rationale: Legacy login callback implements staged lockout handling and explicit session-cookie establishment on successful authentication.
  - Inputs: HTTP `/login` POST credentials and lockout state.
  - Outputs: HTTP `429`/`401` failure or `302` success with `authgen` cookie.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /login (POST)`, `EVID:code:src/src/wifi_handler.cpp:authenticate_credentials`, `EVID:code:src/src/wifi_handler.cpp:start_session`, `EVID:code:src/src/wifi_handler.cpp:attach_session_cookie`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-20: `authenticate_credentials()` shall authenticate only enabled slots, apply slot/unknown lockout checks, verify password hash using stored salt/hash, and set role/user state only on successful verification.
  - Rationale: Legacy auth flow requires matching enabled user slot and hash-based password validation before privilege assignment.
  - Inputs: Username/password pair and lockout counters/state.
  - Outputs: Authentication result plus `web_level`/`web_user` update on success.
  - Execution context: http route handler
  - Evidence: `EVID:bus:configReg.wifi_web_enabled1`, `EVID:bus:configReg.wifi_web_user1`, `EVID:bus:configReg.wifi_web_password1`, `EVID:bus:configReg.wifi_web_salt1`, `EVID:code:src/src/wifi_handler.cpp:authenticate_credentials`, `EVID:code:src/src/wifi_handler.cpp:password_matches`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SC-21: `/logout` handling shall force session reset, clear lockout tracking state, expire the auth cookie, and redirect to `/login.html`.
  - Rationale: Legacy logout path deliberately invalidates current session and challenge state to force re-authentication.
  - Inputs: HTTP `/logout` GET request.
  - Outputs: Session state clear and HTTP redirect with cookie expiration header.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /logout (GET)`, `EVID:code:src/src/wifi_handler.cpp:force_logout`, `EVID:code:src/src/wifi_handler.cpp:clear_session`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-22: `load_file()` shall block `.ini` access for non-factory users, block `/config.html` and `/permissions.html` for non-factory users, and serve SPIFFS assets with no-store headers for dynamic HTML and long-lived caching for static assets.
  - Rationale: Legacy file-serving path enforces role-gated sensitive pages and explicit cache policy by content type.
  - Inputs: Authenticated HTTP file request path and active web role.
  - Outputs: `403` for restricted pages or SPIFFS file response with route-specific cache headers.
  - Execution context: http route handler
  - Evidence: `EVID:code:src/src/wifi_handler.cpp:load_file`, `EVID:code:src/src/wifi_handler.cpp:attach_session_cookie`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-23: `handle_command_requests()` shall treat requests as actionable commands only when argument 0 is named `id`; recognised actions shall mutate target config/status/GPIO fields and set `generalFlags.config_changed` and/or `generalFlags.gpio_update_request` as implemented by each action.
  - Rationale: Legacy command endpoint uses `id` dispatch with direct side effects on shared state and downstream one-shot flags.
  - Inputs: HTTP command request arguments (`id` + action parameters).
  - Outputs: Updated `configReg`/`statusReg`/`gpio_controls` and mutation flags.
  - Execution context: http route handler
  - Evidence: `EVID:bus:generalFlags.config_changed`, `EVID:bus:generalFlags.gpio_update_request`, `EVID:bus:configReg.UIN.val`, `EVID:code:src/src/wifi_handler.cpp:handle_command_requests`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-24: For `whitelist_save`, `handle_command_requests()` shall normalise and store whitelist values, apply inherit options, set whitelist enable/inherit fields, set `statusReg.sms_log_type` to whitelist log type, set `statusReg.modem_originator_number`, and assert `generalFlags.sms_request_user`.
  - Rationale: Legacy whitelist save action triggers both persistence updates and immediate outbound user-notification request state.
  - Inputs: `whitelist_save` action and whitelist/inherit parameters.
  - Outputs: Updated whitelist fields plus SMS-log/request state fields.
  - Execution context: http route handler
  - Evidence: `EVID:bus:configReg.whitelist_enabled`, `EVID:bus:configReg.whitelist_no1`, `EVID:bus:configReg.whitelist_inherit1`, `EVID:bus:statusReg.sms_log_type`, `EVID:bus:statusReg.modem_originator_number`, `EVID:bus:generalFlags.sms_request_user`, `EVID:code:src/src/wifi_handler.cpp:handle_command_requests`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-25: `handleConfigJSON()` shall require authenticated factory role, build JSON export from current config state, and return it as an attachment response with no-store headers and session cookie.
  - Rationale: Legacy JSON export endpoint is explicitly role-gated and serialises live config state for download.
  - Inputs: HTTP `/config.json` GET request.
  - Outputs: JSON attachment response or access-denied response.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /config.json (GET)`, `EVID:code:src/src/wifi_handler.cpp:handleConfigJSON`, `EVID:code:src/src/wifi_handler.cpp:build_config_json`, `EVID:code:src/src/wifi_handler.cpp:attach_session_cookie`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-26: `handleConfigUpload()` shall accept only `config.json`, require authenticated factory role, enforce upload size limit, parse JSON, apply settings via `apply_config_from_json()`, set `generalFlags.config_changed`, and persist with `config_write()`.
  - Rationale: Legacy config-import endpoint enforces strict auth/role/size gates and immediate persistence on successful parse/apply.
  - Inputs: HTTP `/config_upload` POST body stream.
  - Outputs: JSON success/error response and persisted config on success.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /config_upload (POST body)`, `EVID:call:src/src/wifi_handler.cpp→src/src/file_handler.cpp`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/src/wifi_handler.cpp:handleConfigUpload`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-27: `apply_config_from_json()` shall enforce type/range checks for supported keys, normalise phone-number fields, record rejected and unknown fields, and ignore sensitive keys (`wifi_web_*`, `ftp_user`, `ftp_password`) without applying them.
  - Rationale: Legacy JSON apply logic is validation-driven and deliberately excludes sensitive credential fields from import.
  - Inputs: Parsed JSON config document.
  - Outputs: Mutated accepted fields, rejection/unknown lists, and unchanged sensitive credential fields.
  - Execution context: http route handler
  - Evidence: `EVID:bus:configReg.PIN`, `EVID:bus:configReg.ftp_port`, `EVID:bus:configReg.whitelist_enabled`, `EVID:code:src/src/wifi_handler.cpp:apply_config_from_json`
  - Linked invariants: TBD
  - Test strategy: unit

#### 2.3.3 Failure and recovery requirements (if evidenced)
- REQ-SC-28: `wifi_start_ap()` shall perform one retry after AP start failure, and on repeated failure shall leave `statusReg.flags.wifi_server_started` false and return.
  - Rationale: Legacy AP startup contains explicit retry-and-abort logic for initial bring-up failure.
  - Inputs: Failure result from initial and retry `WiFi.softAP(...)` calls.
  - Outputs: `statusReg.flags.wifi_server_started = false` and aborted startup flow.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:code:src/src/wifi_handler.cpp:wifi_start_ap`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-29: `wifi_start_ap()` shall abort startup and keep `statusReg.flags.wifi_server_started` false when AP IP remains `0.0.0.0` after readiness retries.
  - Rationale: Legacy AP bring-up guards against partial startup by requiring a valid SoftAP IP before declaring success.
  - Inputs: SoftAP IP check loop result.
  - Outputs: Startup abort with started flag false.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:code:src/src/wifi_handler.cpp:wifi_start_ap`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SC-30: `handle_upload()` shall reject uploads that exceed available SPIFFS free space by returning HTTP 413 and terminating upload initialisation.
  - Rationale: Legacy file-upload path protects SPIFFS from over-allocation using content-length vs free-space check.
  - Inputs: HTTP upload content length and SPIFFS free-space values.
  - Outputs: HTTP 413 response and no upload-file write path initialisation.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /upload (POST body)`, `EVID:code:src/src/wifi_handler.cpp:handle_upload`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-31: `handleConfigUpload()` shall return error responses and stop processing for invalid filename, unauthenticated or non-factory access, oversize payload, JSON parse error, or `config_write()` failure.
  - Rationale: Legacy config-upload path applies explicit error exits for invalid, unauthorised, and persistence-failure conditions.
  - Inputs: Upload metadata/auth state, streamed payload size, JSON parse result, config-write result.
  - Outputs: Error JSON response and terminated import flow.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /config_upload (POST body)`, `EVID:code:src/src/wifi_handler.cpp:handleConfigUpload`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-32: `handleDoUpdate()` shall reject or abort update flow on missing payload, missing target partition, streamed size overflow, `Update.begin()` failure, or `Update.end()` failure, and on rejected/failed stream shall clear `wifi_update_in_progress` and assert `force_led_update`.
  - Rationale: Legacy OTA path implements explicit defensive abort paths and LED/update-state rollback on failed update stream.
  - Inputs: Upload metadata, partition lookup result, stream write size, Update begin/end results.
  - Outputs: Error/redirect response, aborted update path, and update-state flag adjustments.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /config (POST body)`, `EVID:bus:statusReg.flags.wifi_update_in_progress`, `EVID:bus:generalFlags.force_led_update`, `EVID:code:src/src/wifi_handler.cpp:handleDoUpdate`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SC-33: Authentication lockout handling shall escalate lock windows after repeated slot-specific or unknown-user failures, and active lockout state shall cause login attempts to fail with locked-response metadata.
  - Rationale: Legacy login protection escalates lockout stages and returns lock responses while lockout remains active.
  - Inputs: Repeated failed login attempts and current lockout state.
  - Outputs: Updated lockout state and locked failure responses on subsequent login attempts.
  - Execution context: http route handler
  - Evidence: `EVID:entry:callback:HTTP route /login (POST)`, `EVID:code:src/src/wifi_handler.cpp:authenticate_credentials`, `EVID:code:src/src/wifi_handler.cpp:record_failed_attempt`, `EVID:code:src/src/wifi_handler.cpp:record_unknown_failure`, `EVID:code:src/src/wifi_handler.cpp:is_login_locked`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SC-34: Permissions actions shall return explicit error responses for invalid slot, insufficient role, slot-lockout, current-password mismatch, password mismatch, or weak new password, without applying credential updates.
  - Rationale: Legacy permissions handlers gate credential mutation on role, lock state, and password-policy checks with immediate error return on failure.
  - Inputs: `permissions_save`/`permissions_delete` action parameters and current auth/lock state.
  - Outputs: HTTP/XML error response and no credential mutation for rejected operations.
  - Execution context: http route handler
  - Evidence: `EVID:code:src/src/wifi_handler.cpp:handle_command_requests`, `EVID:code:src/src/wifi_handler.cpp:is_slot_locked`, `EVID:code:src/src/wifi_handler.cpp:password_matches`, `EVID:code:src/src/wifi_handler.cpp:validate_password_strength`
  - Linked invariants: TBD
  - Test strategy: integration

### Slice D (Timing + IO + UI glue) / Core Subclusters 1-5
#### 2.4.1 Feature list
- FTR-SD-01: Loop-driven initialisation and periodic execution of ADC, button, GPIO, IMU, power, LED display, and TFT handlers. Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`, `EVID:call:src/main.cpp→src/src/button_handler.cpp`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`, `EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`, `EVID:call:src/main.cpp→src/src/tft_handler.cpp`
- FTR-SD-02: Setup-time timer alarm and ISR entry provide the timing backbone used by loop-side handlers. Evidence: `EVID:entry:setup`, `EVID:entry:ISR:SysTimer`, `EVID:call:src/main.cpp→src/src/tick.cpp`
- FTR-SD-03: External-voltage request/response handshake links power and ADC handling through shared bus fields. Evidence: `EVID:bus:generalFlags.external_voltage_request`, `EVID:bus:statusReg.external_voltage`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`
- FTR-SD-04: Physical IO scan and state latching track keypad, hook, handsfree, volume, door, and auxiliary input status. Evidence: `EVID:bus:statusReg.keypad_keypress`, `EVID:bus:statusReg.flags.hook_button_pressed`, `EVID:bus:statusReg.flags.handsfree_button1_pressed`, `EVID:bus:statusReg.flags.volume_button_up_pressed`, `EVID:bus:statusReg.flags.door_open`, `EVID:bus:statusReg.flags.io1_input_status`, `EVID:bus:statusReg.flags.io2_input_status`
- FTR-SD-05: IMU tilt monitoring uses config gates and thresholds and writes tilt/fault/reset state into status fields. Evidence: `EVID:bus:configReg.flags.imu_enabled`, `EVID:bus:configReg.flags.imu_vertical_swap_enabled`, `EVID:bus:configReg.imu_tip_threshold`, `EVID:bus:statusReg.imu_roll`, `EVID:bus:statusReg.imu_pitch`, `EVID:bus:statusReg.flags.imu_tilt_exceeded`
- FTR-SD-06: LED status-board handling combines board enable, brightness, and multiple runtime status flags with explicit force-update signalling. Evidence: `EVID:bus:configReg.flags.led_board_status_enabled`, `EVID:bus:configReg.status_led_brightness`, `EVID:bus:generalFlags.force_led_update`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:bus:statusReg.flags.modem_registered`
- FTR-SD-07: Power handling reads battery and charger config fields and drives charging output control. Evidence: `EVID:bus:configReg.battery_chargerate`, `EVID:bus:configReg.battery_low_level`, `EVID:bus:configReg.flags.charger_temperature_enable`, `EVID:bus:gpio_controls.charge_enable`
- FTR-SD-08: TFT lifecycle sets and clears TFT status through a dedicated status flag. Evidence: `EVID:bus:statusReg.flags.tft_status`, `EVID:call:src/main.cpp→src/src/tft_handler.cpp`

#### 2.4.2 Functional requirements
- REQ-SD-01: In `setup()`, the firmware shall configure the timer alarm path and register `SysTimer()` as the ISR entry.
  - Rationale: Entry-point evidence shows setup-time timer registration and a dedicated timer ISR entry.
  - Inputs: Arduino `setup()` entry event.
  - Outputs: Active timer alarm configuration and ISR registration.
  - Execution context: setup
  - Evidence: `EVID:entry:setup`, `EVID:entry:ISR:SysTimer`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SD-02: During loop initialisation, the firmware shall call `adc_init()`, `button_init()`, `gpio_initialise()`, `power_init()`, `leddisplay_init()`, `imu_initialise()`, and `tft_init()`.
  - Rationale: Entry-point fan-out shows these initialisation calls in the loop startup path.
  - Inputs: Loop startup phase before steady-state run.
  - Outputs: Initialised Slice D modules.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`, `EVID:call:src/main.cpp→src/src/button_handler.cpp`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`, `EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp`, `EVID:call:src/main.cpp→src/src/tft_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-03: In runtime loop processing, the firmware shall invoke `power_task()` on the loop path.
  - Rationale: Loop fan-out and inter-file call evidence show loop-driven power task execution.
  - Inputs: Loop iteration tick.
  - Outputs: Power-task state progression.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-04: In runtime loop processing, the firmware shall invoke `adc_task()` on the loop path.
  - Rationale: Loop fan-out and inter-file call evidence show loop-driven ADC task execution.
  - Inputs: Loop iteration tick.
  - Outputs: ADC-task state progression.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-05: In runtime loop processing, the firmware shall invoke `gpio_task()` on the loop path.
  - Rationale: Loop fan-out and inter-file call evidence show loop-driven GPIO task execution.
  - Inputs: Loop iteration tick.
  - Outputs: GPIO task state progression and bus updates.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-06: In runtime loop processing, the firmware shall invoke `button_task()` on the loop path.
  - Rationale: Loop fan-out and inter-file call evidence show loop-driven button task execution.
  - Inputs: Loop iteration tick.
  - Outputs: Button task state progression and request-flag updates.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/button_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-07: In runtime loop processing, the firmware shall invoke `imu_task()` on the loop path.
  - Rationale: Loop fan-out and inter-file call evidence show loop-driven IMU task execution.
  - Inputs: Loop iteration tick.
  - Outputs: IMU task state progression and status updates.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-08: In runtime loop processing, the firmware shall invoke `leddisplay_task()` on the loop path.
  - Rationale: Loop fan-out and inter-file call evidence show loop-driven LED display task execution.
  - Inputs: Loop iteration tick.
  - Outputs: LED display state progression and status updates.
  - Execution context: loop
  - Evidence: `EVID:entry:loop`, `EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-09: `power_task()` shall raise `generalFlags.external_voltage_request` when requesting an external voltage acquisition cycle.
  - Rationale: Shared bus evidence shows power task writes the request flag consumed by ADC processing.
  - Inputs: Power task cycle and voltage-acquisition demand.
  - Outputs: `generalFlags.external_voltage_request` write.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.external_voltage_request`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-10: `adc_task()` shall service `generalFlags.external_voltage_request` and update `statusReg.external_voltage`.
  - Rationale: Shared bus evidence shows ADC task read-modify-write on request and external-voltage state.
  - Inputs: `generalFlags.external_voltage_request`.
  - Outputs: Updated `statusReg.external_voltage` and updated request-flag state.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.external_voltage_request`, `EVID:bus:statusReg.external_voltage`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-11: `power_task()` shall read and update `generalFlags.power_status_request` during power-status handling.
  - Rationale: Shared bus evidence records read-modify-write access to the power-status request flag.
  - Inputs: `generalFlags.power_status_request`.
  - Outputs: Updated `generalFlags.power_status_request` state.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.power_status_request`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SD-12: Power handling shall write `gpio_controls.charge_enable` and shall use `configReg.battery_chargerate`, `configReg.battery_low_level`, and `configReg.flags.charger_temperature_enable` as control inputs.
  - Rationale: Bus evidence links charge output writes to power handlers and battery/charger configuration reads.
  - Inputs: Battery/charger config fields and power-task evaluations.
  - Outputs: `gpio_controls.charge_enable` writes.
  - Execution context: loop
  - Evidence: `EVID:bus:gpio_controls.charge_enable`, `EVID:bus:configReg.battery_chargerate`, `EVID:bus:configReg.battery_low_level`, `EVID:bus:configReg.flags.charger_temperature_enable`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SD-13: `button_task()` shall assert `generalFlags.wifi_request` on the legacy button-triggered WiFi activation path.
  - Rationale: Shared bus evidence shows button-task writes to the WiFi request flag.
  - Inputs: Button-task input events.
  - Outputs: `generalFlags.wifi_request` write.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.wifi_request`, `EVID:call:src/main.cpp→src/src/button_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-14: `button_task()` shall assert `generalFlags.modem_reboot_request` on the legacy button-triggered modem reboot path and shall read modem registration status fields for gating.
  - Rationale: Bus evidence shows reboot-request writes and reads of modem registration indicators in button-task context.
  - Inputs: Button-task input events and modem registration status.
  - Outputs: `generalFlags.modem_reboot_request` write.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.modem_reboot_request`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:statusReg.flags.modem_first_registration`, `EVID:call:src/main.cpp→src/src/button_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-15: `gpio_initialise()` shall set startup states for door, hook, handsfree, and volume status flags before runtime GPIO scanning begins.
  - Rationale: Bus evidence records explicit initial writes for these status fields in `gpio_initialise`.
  - Inputs: Loop initialisation phase.
  - Outputs: Initialised status flags for door/button state tracking.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.door_open`, `EVID:bus:statusReg.flags.hook_button_pressed`, `EVID:bus:statusReg.flags.handsfree_button1_pressed`, `EVID:bus:statusReg.flags.volume_button_up_pressed`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-16: `gpio_task()` shall update `statusReg.keypad_keypress`, `statusReg.flags.io1_input_status`, and `statusReg.flags.io2_input_status` from loop-side IO scanning.
  - Rationale: Bus evidence shows GPIO task read-modify-write on keypad state and writes to both auxiliary input status flags.
  - Inputs: GPIO scan cycle and sampled key/input states.
  - Outputs: Updated keypad and IO input status fields.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.keypad_keypress`, `EVID:bus:statusReg.flags.io1_input_status`, `EVID:bus:statusReg.flags.io2_input_status`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-17: `gpio_task()` shall maintain pressed/released/stuck state flags for hook, handsfree (buttons 1-4), and volume up/down inputs.
  - Rationale: Bus evidence shows read/write maintenance of pressed, released, and stuck fields for these input groups.
  - Inputs: GPIO scan cycle for hook, handsfree, and volume inputs.
  - Outputs: Updated pressed/released/stuck status flags.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.hook_button_pressed`, `EVID:bus:statusReg.flags.hook_button_released`, `EVID:bus:statusReg.flags.hook_button_stuck`, `EVID:bus:statusReg.flags.handsfree_button1_pressed`, `EVID:bus:statusReg.flags.handsfree_button1_released`, `EVID:bus:statusReg.flags.handsfree_button1_stuck`, `EVID:bus:statusReg.flags.volume_button_up_pressed`, `EVID:bus:statusReg.flags.volume_button_up_released`, `EVID:bus:statusReg.flags.volume_button_up_stuck`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-18: `gpio_task()` shall evaluate and update `statusReg.flags.door_open`, `statusReg.flags.door_stuck`, and `statusReg.flags.doorstuck_logtext_done`, using `configReg.flags.door_stuck_message_disable` as an input gate.
  - Rationale: Bus evidence links door state/log latches in GPIO task with the door-stuck message-disable configuration input.
  - Inputs: Door input state and `configReg.flags.door_stuck_message_disable`.
  - Outputs: Updated door-open/stuck/logtext fields.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.door_open`, `EVID:bus:statusReg.flags.door_stuck`, `EVID:bus:statusReg.flags.doorstuck_logtext_done`, `EVID:bus:configReg.flags.door_stuck_message_disable`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-19: `gpio_task()` shall read and update `configReg.telephone_operating_mode` as part of legacy IO control flow.
  - Rationale: Bus evidence records read-modify-write access to the telephone operating mode in GPIO task context.
  - Inputs: GPIO task cycle and active telephone/IO state.
  - Outputs: Updated `configReg.telephone_operating_mode` when mode changes are applied.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.telephone_operating_mode`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-20: `imu_task()` shall gate IMU tilt processing using `configReg.flags.imu_enabled`, `configReg.flags.imu_vertical_swap_enabled`, and `configReg.imu_tip_threshold`.
  - Rationale: Bus evidence shows IMU task reads these configuration fields during processing.
  - Inputs: IMU sample cycle and IMU config fields.
  - Outputs: Gated tilt-processing path execution.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.flags.imu_enabled`, `EVID:bus:configReg.flags.imu_vertical_swap_enabled`, `EVID:bus:configReg.imu_tip_threshold`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-21: `imu_task()` shall write `statusReg.imu_roll` and `statusReg.imu_pitch`, and shall maintain `statusReg.flags.imu_tilt_exceeded`, `statusReg.flags.imu_tilt_reset`, `statusReg.flags.imu_tilt_logtext_done`, and `statusReg.flags.imu_tilt_reset_logtext_done`.
  - Rationale: Bus evidence shows IMU task writes roll/pitch values and updates tilt/fault/reset and associated done-latch flags.
  - Inputs: IMU sample and computed orientation/tilt state.
  - Outputs: Updated IMU orientation and tilt/reset/latch status fields.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.imu_roll`, `EVID:bus:statusReg.imu_pitch`, `EVID:bus:statusReg.flags.imu_tilt_exceeded`, `EVID:bus:statusReg.flags.imu_tilt_reset`, `EVID:bus:statusReg.flags.imu_tilt_logtext_done`, `EVID:bus:statusReg.flags.imu_tilt_reset_logtext_done`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-22: LED display initialisation/update paths shall use `configReg.status_led_brightness` when applying board brightness state.
  - Rationale: Bus evidence shows brightness reads in `leddisplay_init` and read-modify-write behaviour in LED update helpers.
  - Inputs: LED init/update invocation and configured brightness value.
  - Outputs: Updated LED brightness state.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.status_led_brightness`, `EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SD-23: `leddisplay_task()` shall consume board enable/mode/trigger fields and runtime status flags (`call_volte_active`, `incoming_call_ringing`, `modem_first_registration`, `modem_no_sim`, `modem_registered`, `off_hook_active`, `system_booting`, `wifi_server_started`, `wifi_update_in_progress`) and shall read-modify-write `generalFlags.force_led_update` and `statusReg.flags.sms_command_processed`.
  - Rationale: Bus evidence shows LED task reads these status/config fields and performs read-modify-write on force-update and SMS-command-processed flags.
  - Inputs: Loop LED task cycle with current config and runtime status flags.
  - Outputs: Updated LED task state plus updated `generalFlags.force_led_update`/`statusReg.flags.sms_command_processed`.
  - Execution context: loop
  - Evidence: `EVID:bus:configReg.flags.led_board_status_enabled`, `EVID:bus:configReg.telephone_operating_mode`, `EVID:bus:generalFlags.force_led_update`, `EVID:bus:statusReg.flags.call_volte_active`, `EVID:bus:statusReg.flags.incoming_call_ringing`, `EVID:bus:statusReg.flags.modem_first_registration`, `EVID:bus:statusReg.flags.modem_no_sim`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:statusReg.flags.off_hook_active`, `EVID:bus:statusReg.flags.system_booting`, `EVID:bus:statusReg.flags.wifi_server_started`, `EVID:bus:statusReg.flags.wifi_update_in_progress`, `EVID:bus:statusReg.flags.sms_command_processed`, `EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SD-24: TFT lifecycle handlers shall manage `statusReg.flags.tft_status` by setting it during `tft_init()` and updating it during `tft_deinit()`.
  - Rationale: Bus evidence shows `tft_init` writes the TFT status flag and `tft_deinit` performs read-modify-write on the same flag.
  - Inputs: TFT init/deinit calls from loop control flow.
  - Outputs: Updated `statusReg.flags.tft_status`.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.tft_status`, `EVID:call:src/main.cpp→src/src/tft_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

#### 2.4.3 Failure and recovery requirements (if evidenced)
- REQ-SD-25: For door and button anomaly handling, `gpio_task()` shall set and maintain stuck-state flags for door/hook/handsfree/volume inputs and shall maintain `statusReg.flags.doorstuck_logtext_done` as a door-stuck handling latch.
  - Rationale: Bus evidence shows dedicated stuck flags for each input group and a separate door-stuck done latch managed in GPIO task context.
  - Inputs: Sustained GPIO input states indicating stuck conditions.
  - Outputs: Updated stuck flags and door-stuck latch state.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.door_stuck`, `EVID:bus:statusReg.flags.doorstuck_logtext_done`, `EVID:bus:statusReg.flags.hook_button_stuck`, `EVID:bus:statusReg.flags.handsfree_button1_stuck`, `EVID:bus:statusReg.flags.handsfree_button2_stuck`, `EVID:bus:statusReg.flags.handsfree_button3_stuck`, `EVID:bus:statusReg.flags.handsfree_button4_stuck`, `EVID:bus:statusReg.flags.volume_button_up_stuck`, `EVID:bus:statusReg.flags.volume_button_dwn_stuck`, `EVID:call:src/main.cpp→src/src/gpio_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-26: For IMU fault handling, `imu_task()` shall maintain paired fault/reset state fields with paired done-latch fields (`imu_tilt_exceeded` with `imu_tilt_logtext_done`, and `imu_tilt_reset` with `imu_tilt_reset_logtext_done`).
  - Rationale: Bus evidence shows both fault/reset flags and their associated done-latch flags are written in IMU task context.
  - Inputs: IMU tilt-exceeded and tilt-reset state transitions.
  - Outputs: Updated IMU fault/reset flags and corresponding done-latch fields.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.imu_tilt_exceeded`, `EVID:bus:statusReg.flags.imu_tilt_logtext_done`, `EVID:bus:statusReg.flags.imu_tilt_reset`, `EVID:bus:statusReg.flags.imu_tilt_reset_logtext_done`, `EVID:call:src/main.cpp→src/src/imu_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-27: External-voltage recovery flow shall update `generalFlags.external_voltage_request` after ADC servicing so subsequent power requests can be issued.
  - Rationale: Bus evidence shows request-flag writes in power task and read-modify-write handling in ADC task.
  - Inputs: Completed ADC service cycle for an external-voltage request.
  - Outputs: Updated `generalFlags.external_voltage_request` state and retained `statusReg.external_voltage`.
  - Execution context: loop
  - Evidence: `EVID:bus:generalFlags.external_voltage_request`, `EVID:bus:statusReg.external_voltage`, `EVID:call:src/main.cpp→src/src/power_handler.cpp`, `EVID:call:src/main.cpp→src/src/adc_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SD-28: `tft_deinit()` shall update `statusReg.flags.tft_status` to a non-active lifecycle state before control returns to normal loop processing.
  - Rationale: Bus evidence shows `tft_deinit` read-modify-write on the TFT status flag, indicating explicit lifecycle-state transition on deinitialisation.
  - Inputs: TFT deinitialisation request/event.
  - Outputs: Updated `statusReg.flags.tft_status` representing deinitialised TFT state.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.flags.tft_status`, `EVID:call:src/main.cpp→src/src/tft_handler.cpp`
  - Linked invariants: TBD
  - Test strategy: integration

### Slice E (Test harness) / Core Subcluster 1
#### 2.5.1 Feature list
- FTR-SE-01: Loop test-build path executes `test_task()` as the Slice E harness entry. Evidence: `EVID:call:src/main.cpp→src/src/test_handler.cpp`, `EVID:code:src/main.cpp:loop`, `EVID:code:src/src/test_handler.cpp:test_task`
- FTR-SE-02: Keypad-driven mimic playback bridge routes harness key events into audio DTMF playback when enabled. Evidence: `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:bus:statusReg.keypad_keypress`, `EVID:bus:configReg.flags.audio_playback_enable`, `EVID:code:src/src/test_handler.cpp:test_task`
- FTR-SE-03: Test call control from handsfree buttons supports initiate, answer, and hangup by setting modem request flags and call-state flags. Evidence: `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:generalFlags.call_answer_request`, `EVID:bus:generalFlags.call_hangup_request`, `EVID:bus:statusReg.flags.call_inprogress`, `EVID:bus:statusReg.flags.off_hook_active`, `EVID:code:src/src/test_handler.cpp:test_task`
- FTR-SE-04: Test SMS trigger flow writes fixed originator and raises user-SMS request from a handsfree button event. Evidence: `EVID:bus:generalFlags.sms_request_user`, `EVID:bus:statusReg.modem_originator_number`, `EVID:code:src/src/test_handler.cpp:test_task`
- FTR-SE-05: Incoming ring handling updates ring GPIO controls and ring audio playback using one-shot ring requests. Evidence: `EVID:bus:generalFlags.incoming_ring_request`, `EVID:bus:gpio_controls.handsfree_control`, `EVID:bus:gpio_controls.ringer_control`, `EVID:bus:configReg.mimic_volume`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`
- FTR-SE-06: Test harness audio/modem/GPIO integration includes gain updates, modem wake, and handset-handsfree path selection. Evidence: `EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:bus:configReg.handset_PGA_speaker_gain`, `EVID:bus:configReg.handset_PGA_microphone_gain`, `EVID:bus:gpio_controls.hshf_select`

#### 2.5.2 Functional requirements
- REQ-SE-01: In the test-build loop path, the firmware shall invoke `test_task()` as the Slice E execution point.
  - Rationale: Main loop code includes a dedicated branch that calls `test_task()` for test operation.
  - Inputs: Loop iteration in test-build execution path.
  - Outputs: `test_task()` invocation.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/test_handler.cpp`, `EVID:code:src/main.cpp:loop`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-02: `test_task()` shall treat a non-zero change in `statusReg.keypad_keypress` as a new keypad event and, when `configReg.flags.audio_playback_enable` is true, shall request `audio_dtmf(statusReg.keypad_keypress)`.
  - Rationale: Harness code gates keypad mimic playback on key change and the audio-playback-enable configuration flag.
  - Inputs: `statusReg.keypad_keypress` changes and `configReg.flags.audio_playback_enable`.
  - Outputs: Audio DTMF request into the audio handler path.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.keypad_keypress`, `EVID:bus:configReg.flags.audio_playback_enable`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-03: When a keypad release transition is observed (`statusReg.keypad_keypress == 0` and changed), `test_task()` shall update the local keypress latch to zero.
  - Rationale: Harness key handling has explicit release-state latching to avoid repeated trigger behaviour.
  - Inputs: Keypad release transition in `statusReg.keypad_keypress`.
  - Outputs: Internal keypress latch cleared.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.keypad_keypress`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SE-04: When `statusReg.flags.handsfree_button1_pressed` is true and modem is registered, `test_task()` shall start the test call path by waking modem/audio path, applying handset speaker/microphone gains, selecting handsfree GPIO path, setting `statusReg.telephone_mode = HANDSFREE`, writing `statusReg.modem_originator_number`, asserting `generalFlags.call_initialisation_request`, clearing button1 pressed, and setting call/off-hook active flags.
  - Rationale: Harness button1 branch implements the full test call-initialisation sequence through modem, audio, GPIO, and status buses.
  - Inputs: `statusReg.flags.handsfree_button1_pressed`, `statusReg.flags.modem_registered`.
  - Outputs: Updated gains and GPIO controls plus `generalFlags.call_initialisation_request`, `statusReg.flags.call_inprogress`, `statusReg.flags.off_hook_active`, and `statusReg.telephone_mode`.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.handsfree_button1_pressed`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:configReg.handset_PGA_speaker_gain`, `EVID:bus:configReg.handset_PGA_microphone_gain`, `EVID:bus:gpio_controls.hshf_select`, `EVID:bus:gpio_controls.handsfree_control`, `EVID:bus:statusReg.telephone_mode`, `EVID:bus:statusReg.modem_originator_number`, `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:statusReg.flags.call_inprogress`, `EVID:bus:statusReg.flags.off_hook_active`, `EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SE-05: When `statusReg.flags.handsfree_button2_pressed` is true and modem is registered, `test_task()` shall clear button2 pressed, write the harness originator number, and assert `generalFlags.sms_request_user`.
  - Rationale: Harness button2 branch encodes a fixed-number SMS test request flow.
  - Inputs: `statusReg.flags.handsfree_button2_pressed`, `statusReg.flags.modem_registered`.
  - Outputs: Updated `statusReg.modem_originator_number` and `generalFlags.sms_request_user`.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.handsfree_button2_pressed`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:statusReg.modem_originator_number`, `EVID:bus:generalFlags.sms_request_user`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-06: When `statusReg.flags.handsfree_button3_pressed` is true, modem is registered, and ring counter state indicates ringing, `test_task()` shall clear ringing state, assert `generalFlags.call_answer_request`, stop local playback, clear button3 pressed, set call/off-hook active flags, apply handset gains, set handsfree GPIO path, and set `statusReg.telephone_mode = HANDSFREE`.
  - Rationale: Harness button3 branch implements call-answer transition with ring-gate condition and full audio/GPIO status updates.
  - Inputs: `statusReg.flags.handsfree_button3_pressed`, `statusReg.flags.modem_registered`, ring-counter non-zero state.
  - Outputs: `generalFlags.call_answer_request`, `statusReg.flags.incoming_call_ringing`, `statusReg.flags.call_inprogress`, `statusReg.flags.off_hook_active`, gain updates, and handsfree GPIO state.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.handsfree_button3_pressed`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:statusReg.flags.incoming_call_ringing`, `EVID:bus:generalFlags.call_answer_request`, `EVID:bus:statusReg.flags.call_inprogress`, `EVID:bus:statusReg.flags.off_hook_active`, `EVID:bus:configReg.handset_PGA_speaker_gain`, `EVID:bus:configReg.handset_PGA_microphone_gain`, `EVID:bus:gpio_controls.hshf_select`, `EVID:bus:gpio_controls.handsfree_control`, `EVID:bus:statusReg.telephone_mode`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SE-07: When `statusReg.flags.handsfree_button4_pressed` is true, modem is registered, and call is in progress, `test_task()` shall clear ringing state, assert `generalFlags.call_hangup_request`, clear button4 pressed, clear call/off-hook active flags, and disable handsfree GPIO control.
  - Rationale: Harness button4 branch defines explicit test-call hangup and UI/IO state rollback.
  - Inputs: `statusReg.flags.handsfree_button4_pressed`, `statusReg.flags.modem_registered`, `statusReg.flags.call_inprogress`.
  - Outputs: `generalFlags.call_hangup_request`, updated call/ringing/off-hook flags, and updated handsfree GPIO state.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.handsfree_button4_pressed`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:statusReg.flags.call_inprogress`, `EVID:bus:statusReg.flags.incoming_call_ringing`, `EVID:bus:generalFlags.call_hangup_request`, `EVID:bus:statusReg.flags.off_hook_active`, `EVID:bus:gpio_controls.hshf_select`, `EVID:bus:gpio_controls.handsfree_control`, `EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-08: On `generalFlags.incoming_ring_request`, `test_task()` shall clear the request flag, enable ring-related GPIO controls, apply mimic volume, start `/ringing.mp3` only when no audio is currently playing, and increment the local ring counter.
  - Rationale: Harness ring handling is implemented as one-shot request consumption plus gated ring playback and GPIO signalling.
  - Inputs: `generalFlags.incoming_ring_request`, `configReg.mimic_volume`, current audio play state.
  - Outputs: Cleared ring request flag, updated ring/handsfree GPIO controls, playback request, and incremented ring counter.
  - Execution context: test harness
  - Evidence: `EVID:bus:generalFlags.incoming_ring_request`, `EVID:bus:gpio_controls.handsfree_control`, `EVID:bus:gpio_controls.ringer_control`, `EVID:bus:gpio_controls.hshf_select`, `EVID:bus:configReg.mimic_volume`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: HIL

- REQ-SE-09: On `statusReg.flags.call_disconnected`, `test_task()` shall clear disconnect and call-progress flags, reset ring counter, disable ring/handsfree controls, stop local playback, clear off-hook active, and clear incoming-ringing state.
  - Rationale: Harness disconnect branch encodes a full return-to-idle cleanup sequence for call and ring state.
  - Inputs: `statusReg.flags.call_disconnected`.
  - Outputs: Cleared call/ring/off-hook status and disabled ring/handsfree outputs with playback stopped.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.call_disconnected`, `EVID:bus:statusReg.flags.call_inprogress`, `EVID:bus:statusReg.flags.off_hook_active`, `EVID:bus:statusReg.flags.incoming_call_ringing`, `EVID:bus:gpio_controls.handsfree_control`, `EVID:bus:gpio_controls.ringer_control`, `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-10: `test_task()` shall gate call-init, SMS, answer, and hangup harness actions on `statusReg.flags.modem_registered`.
  - Rationale: Each handsfree-button action branch includes modem-registration gating in the harness logic.
  - Inputs: Handsfree button flags and `statusReg.flags.modem_registered`.
  - Outputs: Suppression or execution of harness actions based on registration state.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:statusReg.flags.handsfree_button1_pressed`, `EVID:bus:statusReg.flags.handsfree_button2_pressed`, `EVID:bus:statusReg.flags.handsfree_button3_pressed`, `EVID:bus:statusReg.flags.handsfree_button4_pressed`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SE-11: For test call and SMS trigger flows, `test_task()` shall write `statusReg.modem_originator_number` to the legacy harness destination string before asserting outbound request flags.
  - Rationale: Harness code writes a fixed originator number in both call-init and SMS branches before request signalling.
  - Inputs: Button1 or button2 harness trigger event.
  - Outputs: Updated `statusReg.modem_originator_number` and outbound modem/SMS request flags.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.modem_originator_number`, `EVID:bus:generalFlags.call_initialisation_request`, `EVID:bus:generalFlags.sms_request_user`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-12: In harness answer and call-init flows, `test_task()` shall set `statusReg.telephone_mode` to `HANDSFREE`.
  - Rationale: Harness call-init and answer branches both force telephone mode to handsfree.
  - Inputs: Harness call-init or call-answer branch execution.
  - Outputs: `statusReg.telephone_mode` written to `HANDSFREE`.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.telephone_mode`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

#### 2.5.3 Failure and recovery requirements (if evidenced)
- REQ-SE-13: Ring request handling shall consume `generalFlags.incoming_ring_request` as a one-shot by clearing it inside `test_task()` after ring processing starts.
  - Rationale: Harness ring branch explicitly clears the incoming ring request flag before progressing ring outputs and playback.
  - Inputs: `generalFlags.incoming_ring_request = true`.
  - Outputs: `generalFlags.incoming_ring_request = false` and active ring handling.
  - Execution context: test harness
  - Evidence: `EVID:bus:generalFlags.incoming_ring_request`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: unit

- REQ-SE-14: `test_task()` shall not assert `generalFlags.call_answer_request` unless the harness ring counter indicates an active/previous ring event.
  - Rationale: Harness answer branch includes an explicit non-zero ring-counter gate before issuing answer request.
  - Inputs: `statusReg.flags.handsfree_button3_pressed`, `statusReg.flags.modem_registered`, ring-counter state.
  - Outputs: Conditional `generalFlags.call_answer_request`.
  - Execution context: test harness
  - Evidence: `EVID:bus:statusReg.flags.handsfree_button3_pressed`, `EVID:bus:statusReg.flags.modem_registered`, `EVID:bus:generalFlags.call_answer_request`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-SE-15: During ring handling, the harness shall avoid restarting ring playback when audio is already active by gating `audio_playback("/ringing.mp3")` on current playback state.
  - Rationale: Harness ring branch checks audio play status before issuing ring-playback request.
  - Inputs: Active ring request and current audio play-status state.
  - Outputs: Conditional ring playback request to audio handler.
  - Execution context: test harness
  - Evidence: `EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp`, `EVID:bus:generalFlags.incoming_ring_request`, `EVID:code:src/src/test_handler.cpp:test_task`
  - Linked invariants: TBD
  - Test strategy: HIL

## 3) Cross-cutting requirements (only if evidenced)
### Logging and diagnostics
- REQ-XC-01: The legacy firmware shall emit audio-test diagnostics for test phase progression, pass/fail result, and cleanup/abort timeout conditions.
  - Rationale: Legacy code prints explicit diagnostic events around audio-test control flow and outcomes.
  - Inputs: Audio-test mode transitions and timeout/result events.
  - Outputs: Serial diagnostic log lines.
  - Execution context: loop
  - Evidence: `EVID:call:src/main.cpp→src/src/audio_handler.cpp`, `EVID:code:src/main.cpp:loop`, `EVID:code:src/src/audio_handler.cpp:audio_test_task`
  - Linked invariants: TBD
  - Test strategy: manual

- REQ-XC-03: The legacy firmware shall emit modem/network/FTP/SMS diagnostics for modem state changes, registration checks, FTP stage outcomes, and SMS queue/send outcomes.
  - Rationale: Legacy modem and loop paths contain explicit serial diagnostic outputs for these operational transitions and recovery paths.
  - Inputs: Modem task stage transitions, query outcomes, FTP outcomes, and SMS send outcomes.
  - Outputs: Serial diagnostic log lines.
  - Execution context: task: modem
  - Evidence: `EVID:call:src/main.cpp→src/src/modem_handler.cpp`, `EVID:code:src/src/modem_handler.cpp:modem_task`, `EVID:code:src/src/modem_handler.cpp:modem_decode`, `EVID:code:src/src/modem_handler.cpp:sms_task`
  - Linked invariants: TBD
  - Test strategy: manual

### Configuration persistence behaviour
- REQ-XC-02: Legacy configuration update paths shall persist and expose Slice A audio control fields (`audio_path_test_enable`, `audio_playback_enable`, `ringer_audio_enable`, handset/handsfree speaker gains), and runtime Slice A logic shall consume those fields.
  - Rationale: Shared bus evidence shows write paths (HTTP/file/modem) and read paths (loop/dtmf/audio flows) for these fields.
  - Inputs: Config writes from HTTP route handler, file defaults/read/write, and modem decode paths.
  - Outputs: Runtime decisions and gain settings driven by persisted config fields.
  - Execution context: http route handler
  - Evidence: `EVID:bus:configReg.flags.audio_path_test_enable`, `EVID:bus:configReg.flags.audio_playback_enable`, `EVID:bus:configReg.flags.ringer_audio_enable`, `EVID:bus:configReg.handset_PGA_speaker_gain`, `EVID:bus:configReg.handsfree_PGA_speaker_gain`
  - Linked invariants: TBD
  - Test strategy: integration

- REQ-XC-04: Legacy configuration update paths shall persist and expose Slice B modem/telephony fields (`ftp_server`, `ftp_user`, `ftp_password`, `ftp_port`, `ftp_directory`, `ftp_last_update_date`, `expansion_sim_enabled`, `voice_no1`, `sms_user_no1`, `sms_logtext_no1`) for runtime modem and loop logic.
  - Rationale: Shared bus evidence shows these fields are written by file/HTTP/modem flows and consumed by loop/modem execution paths.
  - Inputs: Config writes from file defaults/read/write, HTTP route handlers, SMS/FTP decode updates.
  - Outputs: Runtime modem/FTP/SMS/call decisions driven by persisted config fields.
  - Execution context: http route handler
  - Evidence: `EVID:bus:configReg.ftp_server`, `EVID:bus:configReg.ftp_user`, `EVID:bus:configReg.ftp_password`, `EVID:bus:configReg.ftp_port`, `EVID:bus:configReg.ftp_directory`, `EVID:bus:configReg.ftp_last_update_date`, `EVID:bus:configReg.flags.expansion_sim_enabled`, `EVID:bus:configReg.voice_no1`, `EVID:bus:configReg.sms_user_no1`, `EVID:bus:configReg.sms_logtext_no1`
  - Linked invariants: TBD
  - Test strategy: integration

### OTA update behaviour (if present in legacy)
- REQ-XC-05: Legacy OTA logic shall compare downloaded FTP update date with persisted `ftp_last_update_date`, request update download only when dates differ, and on successful download shall mark completion and persist the new date.
  - Rationale: Legacy FTP update flow includes explicit date-based gating and post-success persistence.
  - Inputs: FTP update-info result (`statusReg.ftp_file_date`) and persisted last-update date.
  - Outputs: Conditional `generalFlags.ftp_update_download_request`, `statusReg.flags.ftp_download_complete`, and persisted `configReg.ftp_last_update_date`.
  - Execution context: loop
  - Evidence: `EVID:bus:statusReg.ftp_file_date`, `EVID:bus:configReg.ftp_last_update_date`, `EVID:bus:generalFlags.ftp_update_download_request`, `EVID:bus:statusReg.flags.ftp_download_complete`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/main.cpp:loop`, `EVID:code:src/src/modem_handler.cpp:modem_decode`
  - Linked invariants: TBD
  - Test strategy: integration

### Security-related behaviours in legacy (not future intentions)
- REQ-XC-06: Legacy SMS command handling shall require PIN validation and whitelist authorisation before executing command mutations, and rejected attempts shall be logged in authentication history.
  - Rationale: Legacy SMS decode path enforces PIN/whitelist gates and records unauthorised access metadata.
  - Inputs: Inbound SMS command text and originator number.
  - Outputs: Command acceptance/rejection and authentication log updates.
  - Execution context: task: modem
  - Evidence: `EVID:bus:configReg.PIN`, `EVID:bus:generalFlags.config_changed`, `EVID:code:src/src/modem_handler.cpp:sms_decode`, `EVID:code:src/src/modem_handler.cpp:sms_whitelist_allowed`, `EVID:code:src/src/modem_handler.cpp:log_auth_event`
  - Linked invariants: TBD
  - Test strategy: integration

## 4) Traceability matrix
| Requirement ID | Slice | Execution context | Evidence tags | Linked invariants | Test strategy |
|---|---|---|---|---|---|
| REQ-SA-01 | Slice A | task: audio | EVID:entry:task:AudioThread; EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/main.cpp:audio_thread | TBD | integration |
| REQ-SA-02 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/src/audio_handler.cpp:audio_task; EVID:entry:task:AudioThread | TBD | integration |
| REQ-SA-03 | Slice A | loop | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/src/audio_handler.cpp:audio_playback | TBD | HIL |
| REQ-SA-04 | Slice A | loop | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/src/audio_handler.cpp:audio_playback_stop | TBD | HIL |
| REQ-SA-05 | Slice A | loop | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/src/audio_handler.cpp:audio_set_volume | TBD | unit |
| REQ-SA-06 | Slice A | loop | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/src/audio_handler.cpp:audio_dtmf | TBD | integration |
| REQ-SA-07 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.audio_digit_playing; EVID:code:src/src/audio_handler.cpp:dtmf_task | TBD | HIL |
| REQ-SA-08 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.audio_digit_playing; EVID:code:src/src/audio_handler.cpp:dtmf_read_file; EVID:code:src/src/audio_handler.cpp:dtmf_tone_output | TBD | HIL |
| REQ-SA-09 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.audio_digit_playing; EVID:code:src/src/audio_handler.cpp:dtmf_task | TBD | HIL |
| REQ-SA-10 | Slice A | loop | EVID:bus:statusReg.flags.audio_digit_playing; EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SA-11 | Slice A | loop | EVID:call:src/main.cpp→src/src/dtmf_handler.cpp; EVID:bus:statusReg.modem_response; EVID:bus:statusReg.modem_dtmf_digit; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task | TBD | unit |
| REQ-SA-12 | Slice A | loop | EVID:call:src/main.cpp→src/src/dtmf_handler.cpp; EVID:bus:configReg.PIN; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task | TBD | unit |
| REQ-SA-13 | Slice A | loop | EVID:call:src/src/dtmf_handler.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.call_connected; EVID:bus:configReg.handset_PGA_speaker_gain; EVID:bus:configReg.handsfree_PGA_speaker_gain; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task | TBD | HIL |
| REQ-SA-14 | Slice A | loop | EVID:bus:generalFlags.load_activate_request; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task | TBD | integration |
| REQ-SA-15 | Slice A | loop | EVID:bus:gpio_controls.load_control; EVID:call:src/src/dtmf_handler.cpp→src/src/gpio_handler.cpp; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task | TBD | integration |
| REQ-SA-16 | Slice A | loop | EVID:call:src/main.cpp→src/src/dtmf_handler.cpp; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SA-17 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:generalFlags.audio_test_request; EVID:bus:statusReg.flags.audio_test_passed; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | HIL |
| REQ-SA-18 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:generalFlags.audio_test_tone_start_request; EVID:bus:statusReg.flags.audio_test_tone_active; EVID:bus:statusReg.audio_test_tone_tick; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | integration |
| REQ-SA-19 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.audio_test_tone_active; EVID:bus:statusReg.audio_test_tone_tick; EVID:bus:statusReg.flags.audio_test_passed; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | HIL |
| REQ-SA-20 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:generalFlags.audio_test_request; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | integration |
| REQ-SA-21 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.audio_test_complete; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | integration |
| REQ-SA-22 | Slice A | loop | EVID:bus:statusReg.flags.modem_process_complete; EVID:bus:generalFlags.audio_test_request; EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SA-23 | Slice A | loop | EVID:bus:statusReg.flags.audio_test_complete; EVID:bus:statusReg.flags.audio_test_cleanup_complete; EVID:bus:generalFlags.audio_test_cleanup_request; EVID:code:src/main.cpp:loop | TBD | HIL |
| REQ-SA-24 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:generalFlags.audio_test_abort_request; EVID:bus:generalFlags.audio_test_request; EVID:bus:generalFlags.audio_test_tone_start_request; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | integration |
| REQ-SA-25 | Slice A | task: audio | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.audio_test_tone_active; EVID:bus:statusReg.flags.audio_test_passed; EVID:bus:statusReg.flags.audio_test_complete; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | unit |
| REQ-SA-26 | Slice A | loop | EVID:call:src/src/dtmf_handler.cpp→src/src/audio_handler.cpp; EVID:bus:statusReg.flags.call_connected; EVID:bus:configReg.handset_PGA_speaker_gain; EVID:bus:configReg.handsfree_PGA_speaker_gain; EVID:code:src/src/dtmf_handler.cpp:dtmf_decode_task | TBD | integration |
| REQ-SA-27 | Slice A | loop | EVID:bus:generalFlags.audio_test_request; EVID:bus:generalFlags.test_audio_request; EVID:bus:generalFlags.audio_test_tone_start_request; EVID:bus:generalFlags.audio_test_cleanup_request; EVID:bus:generalFlags.audio_test_abort_request; EVID:code:src/main.cpp:audio_test_clear_pending_requests | TBD | integration |
| REQ-SA-28 | Slice A | loop | EVID:bus:generalFlags.audio_test_abort_request; EVID:bus:generalFlags.audio_test_cleanup_request; EVID:code:src/main.cpp:loop | TBD | HIL |
| REQ-SA-29 | Slice A | loop | EVID:bus:statusReg.flags.audio_test_complete; EVID:bus:statusReg.flags.audio_test_passed; EVID:bus:statusReg.sms_log_type; EVID:bus:generalFlags.sms_request_log; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SA-30 | Slice A | loop | EVID:bus:statusReg.sms_log_type; EVID:bus:generalFlags.sms_request_log; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SA-31 | Slice A | loop | EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:code:src/main.cpp:loop; EVID:bus:statusReg.telephone_mode | TBD | integration |
| REQ-XC-01 | Cross-cutting | loop | EVID:call:src/main.cpp→src/src/audio_handler.cpp; EVID:code:src/main.cpp:loop; EVID:code:src/src/audio_handler.cpp:audio_test_task | TBD | manual |
| REQ-XC-02 | Cross-cutting | http route handler | EVID:bus:configReg.flags.audio_path_test_enable; EVID:bus:configReg.flags.audio_playback_enable; EVID:bus:configReg.flags.ringer_audio_enable; EVID:bus:configReg.handset_PGA_speaker_gain; EVID:bus:configReg.handsfree_PGA_speaker_gain | TBD | integration |
| REQ-SB-01 | Slice B | setup | EVID:entry:setup; EVID:code:src/main.cpp:setup | TBD | integration |
| REQ-SB-02 | Slice B | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/file_handler.cpp; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-03 | Slice B | loop | EVID:entry:task:ModemThread; EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-04 | Slice B | loop | EVID:entry:loop; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-05 | Slice B | task: modem | EVID:entry:task:ModemThread; EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:code:src/main.cpp:modem_thread | TBD | integration |
| REQ-SB-06 | Slice B | task: modem | EVID:entry:task:ModemThread; EVID:code:src/src/modem_handler.cpp:modem_task | TBD | integration |
| REQ-SB-07 | Slice B | task: modem | EVID:call:src/src/modem_handler.cpp→src/src/serial_transport.cpp; EVID:code:src/src/modem_handler.cpp:modem_init | TBD | mock |
| REQ-SB-08 | Slice B | task: modem | EVID:bus:statusReg.flags.modem_process_inprogress; EVID:bus:statusReg.flags.modem_registered; EVID:call:src/src/modem_handler.cpp→src/src/tick.cpp; EVID:code:src/src/modem_handler.cpp:modem_task | TBD | integration |
| REQ-SB-09 | Slice B | task: modem | EVID:bus:generalFlags.call_initialisation_request; EVID:bus:generalFlags.call_answer_request; EVID:bus:generalFlags.call_hangup_request; EVID:bus:generalFlags.call_audio_request; EVID:bus:generalFlags.call_dtmf_request; EVID:code:src/src/modem_handler.cpp:modem_task | TBD | integration |
| REQ-SB-10 | Slice B | loop | EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:bus:generalFlags.call_initialisation_request; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SB-11 | Slice B | loop | EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:bus:generalFlags.call_answer_request; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SB-12 | Slice B | loop | EVID:bus:generalFlags.call_audio_request; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/main.cpp:set_call_audio_request; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SB-13 | Slice B | task: modem | EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/src/modem_handler.cpp:call_audio_setup | TBD | HIL |
| REQ-SB-14 | Slice B | loop | EVID:bus:statusReg.modem_dtmf_digit; EVID:bus:generalFlags.call_dtmf_request; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SB-15 | Slice B | task: modem | EVID:code:src/src/modem_handler.cpp:call_volume_update; EVID:code:src/src/modem_handler.cpp:modem_task | TBD | unit |
| REQ-SB-16 | Slice B | loop | EVID:bus:generalFlags.ftp_connect_request; EVID:bus:statusReg.flags.modem_ftp_connected; EVID:bus:statusReg.flags.ftp_open_failed; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-17 | Slice B | loop | EVID:bus:generalFlags.ftp_directory_request; EVID:bus:generalFlags.ftp_download_request; EVID:bus:generalFlags.ftp_update_size_request; EVID:bus:generalFlags.ftp_update_info_request; EVID:bus:generalFlags.ftp_update_download_request; EVID:bus:generalFlags.ftp_disconnect_request; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-18 | Slice B | task: modem | EVID:bus:generalFlags.ftp_connect_request; EVID:bus:generalFlags.ftp_directory_request; EVID:bus:generalFlags.ftp_download_request; EVID:bus:generalFlags.ftp_update_size_request; EVID:bus:generalFlags.ftp_update_info_request; EVID:bus:generalFlags.ftp_update_download_request; EVID:bus:generalFlags.ftp_disconnect_request; EVID:code:src/src/modem_handler.cpp:modem_task | TBD | integration |
| REQ-SB-19 | Slice B | task: modem | EVID:bus:configReg.ftp_user; EVID:bus:configReg.ftp_password; EVID:bus:configReg.ftp_server; EVID:bus:configReg.ftp_port; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/src/modem_handler.cpp:modem_ftp_connect | TBD | integration |
| REQ-SB-20 | Slice B | task: modem | EVID:bus:configReg.ftp_directory; EVID:bus:statusReg.flags.modem_process_complete; EVID:code:src/src/modem_handler.cpp:modem_ftp_directory | TBD | integration |
| REQ-SB-21 | Slice B | loop | EVID:bus:statusReg.flags.ftp_download_complete; EVID:bus:statusReg.ftp_file_date; EVID:bus:configReg.ftp_last_update_date; EVID:bus:generalFlags.config_changed; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-22 | Slice B | Thingstream callback | EVID:entry:callback:Thingstream_Application_modemCallback; EVID:code:src/src/modem_handler.cpp:Thingstream_Application_modemCallback | TBD | unit |
| REQ-SB-23 | Slice B | Thingstream callback | EVID:bus:generalFlags.ftp_file_request; EVID:bus:statusReg.flags.incoming_sms_ready; EVID:code:src/src/modem_handler.cpp:modem_callback | TBD | integration |
| REQ-SB-24 | Slice B | Thingstream callback | EVID:bus:statusReg.flags.incoming_call_ring_detected; EVID:bus:statusReg.modem_response; EVID:code:src/src/modem_handler.cpp:modem_decode | TBD | unit |
| REQ-SB-25 | Slice B | Thingstream callback | EVID:bus:statusReg.modem_response; EVID:bus:statusReg.modem_signal_level; EVID:bus:statusReg.modem_rx_level; EVID:code:src/src/modem_handler.cpp:modem_decode | TBD | unit |
| REQ-SB-26 | Slice B | Thingstream callback | EVID:bus:statusReg.flags.modem_ftp_connected; EVID:bus:statusReg.flags.ftp_open_failed; EVID:bus:statusReg.flags.ftp_directory_not_found; EVID:bus:statusReg.flags.ftp_file_not_found; EVID:bus:statusReg.flags.ftp_download_complete; EVID:bus:statusReg.ftp_file_size; EVID:bus:statusReg.ftp_file_date; EVID:code:src/src/modem_handler.cpp:modem_decode | TBD | integration |
| REQ-SB-27 | Slice B | task: modem | EVID:bus:configReg.PIN; EVID:code:src/src/modem_handler.cpp:sms_decode; EVID:code:src/src/modem_handler.cpp:sms_whitelist_allowed | TBD | unit |
| REQ-SB-28 | Slice B | task: modem | EVID:bus:generalFlags.config_changed; EVID:bus:generalFlags.ftp_check_request; EVID:bus:statusReg.flags.sms_command_processed; EVID:code:src/src/modem_handler.cpp:sms_decode | TBD | integration |
| REQ-SB-29 | Slice B | task: modem | EVID:bus:generalFlags.sms_request_user; EVID:bus:generalFlags.sms_request_log; EVID:bus:statusReg.flags.sms_inprogress; EVID:code:src/src/modem_handler.cpp:sms_task | TBD | integration |
| REQ-SB-30 | Slice B | task: modem | EVID:call:src/src/modem_handler.cpp→src/src/serial_transport.cpp; EVID:code:src/src/serial_transport.cpp:serial_run | TBD | mock |
| REQ-SB-31 | Slice B | task: modem | EVID:code:src/src/serial_transport.cpp:serial_run | TBD | unit |
| REQ-SB-32 | Slice B | task: modem | EVID:call:src/src/modem_handler.cpp→src/src/common.cpp; EVID:code:src/src/common.cpp:normalize_phone_number | TBD | unit |
| REQ-SB-33 | Slice B | task: modem | EVID:code:src/src/modem_handler.cpp:modem_send | TBD | mock |
| REQ-SB-34 | Slice B | loop | EVID:bus:statusReg.flags.ftp_open_failed; EVID:bus:statusReg.flags.ftp_directory_not_found; EVID:bus:statusReg.flags.ftp_file_not_found; EVID:bus:generalFlags.ftp_disconnect_request; EVID:bus:statusReg.telephone_mode; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SB-35 | Slice B | task: modem | EVID:bus:statusReg.telephone_sub_mode; EVID:code:src/src/modem_handler.cpp:call_initialisation | TBD | integration |
| REQ-SB-36 | Slice B | loop | EVID:bus:statusReg.modem_response; EVID:bus:configReg.flags.no_network_response_enabled; EVID:bus:configReg.flags.low_signal_call_disable_enabled; EVID:bus:statusReg.modem_rx_level; EVID:bus:configReg.call_disable_low_signal_level; EVID:bus:statusReg.telephone_sub_mode; EVID:code:src/main.cpp:submode_task | TBD | integration |
| REQ-SB-37 | Slice B | task: modem | EVID:bus:statusReg.flags.modem_registered; EVID:code:src/src/modem_handler.cpp:modem_task | TBD | integration |
| REQ-SB-38 | Slice B | task: modem | EVID:bus:statusReg.flags.modem_no_sim; EVID:bus:generalFlags.sim_switch_reboot_request; EVID:bus:generalFlags.modem_reboot_request; EVID:bus:generalFlags.gpio_update_request; EVID:bus:configReg.flags.expansion_sim_enabled; EVID:code:src/src/modem_handler.cpp:sim_switch_guard_handle | TBD | integration |
| REQ-SB-39 | Slice B | task: modem | EVID:bus:statusReg.flags.sms_inprogress; EVID:code:src/src/modem_handler.cpp:sms_task | TBD | integration |
| REQ-SC-01 | Slice C | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/file_handler.cpp; EVID:code:src/src/file_handler.cpp:config_init | TBD | integration |
| REQ-SC-02 | Slice C | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/file_handler.cpp; EVID:code:src/src/file_handler.cpp:config_read; EVID:code:src/src/file_handler.cpp:config_default; EVID:code:src/src/file_handler.cpp:config_write | TBD | integration |
| REQ-SC-03 | Slice C | loop | EVID:bus:configReg.no_sms_logs; EVID:bus:configReg.no_web_logs; EVID:code:src/src/file_handler.cpp:config_read | TBD | unit |
| REQ-SC-04 | Slice C | loop | EVID:bus:configReg.wifi_web_password1; EVID:bus:configReg.wifi_web_salt1; EVID:bus:configReg.whitelist_enabled; EVID:bus:generalFlags.config_changed; EVID:code:src/src/file_handler.cpp:config_read; EVID:code:src/src/file_handler.cpp:migrate_legacy_passwords | TBD | integration |
| REQ-SC-05 | Slice C | loop | EVID:code:src/src/file_handler.cpp:config_read; EVID:code:src/src/file_handler.cpp:config_default; EVID:code:src/src/file_handler.cpp:config_write | TBD | integration |
| REQ-SC-06 | Slice C | loop | EVID:bus:configReg.whitelist_no1; EVID:bus:configReg.wifi_web_user1; EVID:bus:configReg.wifi_web_password1; EVID:bus:configReg.wifi_web_salt1; EVID:bus:generalFlags.config_changed; EVID:code:src/src/file_handler.cpp:config_read | TBD | integration |
| REQ-SC-07 | Slice C | loop | EVID:bus:configReg.UIN.val; EVID:bus:configReg.wifi_web_enabled1; EVID:bus:configReg.whitelist_enabled; EVID:code:src/src/file_handler.cpp:config_write | TBD | integration |
| REQ-SC-08 | Slice C | loop | EVID:bus:configReg.wifi_web_user1; EVID:bus:configReg.wifi_web_password1; EVID:bus:configReg.wifi_web_salt1; EVID:bus:configReg.wifi_web_enabled1; EVID:bus:configReg.wifi_web_role1; EVID:bus:configReg.whitelist_enabled; EVID:bus:configReg.no_sms_logs; EVID:bus:configReg.no_web_logs; EVID:bus:configReg.no_auth_logs; EVID:code:src/src/file_handler.cpp:config_default | TBD | unit |
| REQ-SC-09 | Slice C | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/wifi_handler.cpp; EVID:bus:generalFlags.wifi_request; EVID:bus:statusReg.flags.wifi_server_started; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SC-10 | Slice C | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/wifi_handler.cpp; EVID:bus:statusReg.flags.wifi_server_started; EVID:bus:statusReg.flags.wifi_server_connected; EVID:bus:statusReg.flags.wifi_server_disconnected; EVID:bus:generalFlags.wifi_deactivate_request; EVID:code:src/main.cpp:loop | TBD | integration |
| REQ-SC-11 | Slice C | loop | EVID:bus:configReg.flags.location_text_setup; EVID:bus:configReg.phone_loc; EVID:code:src/src/wifi_handler.cpp:wifi_start_ap | TBD | HIL |
| REQ-SC-12 | Slice C | loop | EVID:entry:callback:WiFi AP STA connected; EVID:entry:callback:WiFi AP STA disconnected; EVID:entry:callback:HTTP route /values.xml; EVID:entry:callback:HTTP route /config_upload (POST body); EVID:entry:callback:HTTP not-found route; EVID:bus:statusReg.flags.wifi_server_started; EVID:code:src/src/wifi_handler.cpp:wifi_start_ap | TBD | integration |
| REQ-SC-13 | Slice C | loop | EVID:call:src/main.cpp→src/src/wifi_handler.cpp; EVID:bus:statusReg.flags.wifi_server_started; EVID:bus:statusReg.flags.wifi_server_connected; EVID:bus:statusReg.flags.wifi_server_disconnected; EVID:code:src/src/wifi_handler.cpp:wifi_stop_ap | TBD | integration |
| REQ-SC-14 | Slice C | loop | EVID:bus:statusReg.flags.wifi_server_connected; EVID:bus:statusReg.flags.wifi_server_disconnected; EVID:code:src/src/wifi_handler.cpp:wifi_check_client_timeouts | TBD | integration |
| REQ-SC-15 | Slice C | wifi event callback | EVID:entry:callback:WiFi AP STA connected; EVID:bus:statusReg.flags.wifi_server_connected; EVID:bus:statusReg.flags.wifi_server_disconnected; EVID:bus:configReg.no_web_logs; EVID:bus:generalFlags.config_changed; EVID:code:src/src/wifi_handler.cpp:wifi_connect_event | TBD | integration |
| REQ-SC-16 | Slice C | wifi event callback | EVID:entry:callback:WiFi AP STA disconnected; EVID:bus:statusReg.flags.wifi_server_connected; EVID:bus:statusReg.flags.wifi_server_disconnected; EVID:code:src/src/wifi_handler.cpp:wifi_disconnect_event; EVID:code:src/src/wifi_handler.cpp:wifi_check_client_timeouts | TBD | integration |
| REQ-SC-17 | Slice C | http route handler | EVID:entry:callback:HTTP not-found route; EVID:code:src/src/wifi_handler.cpp:handle_web_requests; EVID:code:src/src/wifi_handler.cpp:track_request_activity; EVID:code:src/src/wifi_handler.cpp:handle_command_requests; EVID:code:src/src/wifi_handler.cpp:load_file | TBD | integration |
| REQ-SC-18 | Slice C | http route handler | EVID:code:src/src/wifi_handler.cpp:ensure_authenticated; EVID:code:src/src/wifi_handler.cpp:session_cookie_valid; EVID:code:src/src/wifi_handler.cpp:redirect_to_login | TBD | unit |
| REQ-SC-19 | Slice C | http route handler | EVID:entry:callback:HTTP route /login (POST); EVID:code:src/src/wifi_handler.cpp:authenticate_credentials; EVID:code:src/src/wifi_handler.cpp:start_session; EVID:code:src/src/wifi_handler.cpp:attach_session_cookie | TBD | integration |
| REQ-SC-20 | Slice C | http route handler | EVID:bus:configReg.wifi_web_enabled1; EVID:bus:configReg.wifi_web_user1; EVID:bus:configReg.wifi_web_password1; EVID:bus:configReg.wifi_web_salt1; EVID:code:src/src/wifi_handler.cpp:authenticate_credentials; EVID:code:src/src/wifi_handler.cpp:password_matches | TBD | unit |
| REQ-SC-21 | Slice C | http route handler | EVID:entry:callback:HTTP route /logout (GET); EVID:code:src/src/wifi_handler.cpp:force_logout; EVID:code:src/src/wifi_handler.cpp:clear_session | TBD | integration |
| REQ-SC-22 | Slice C | http route handler | EVID:code:src/src/wifi_handler.cpp:load_file; EVID:code:src/src/wifi_handler.cpp:attach_session_cookie | TBD | integration |
| REQ-SC-23 | Slice C | http route handler | EVID:bus:generalFlags.config_changed; EVID:bus:generalFlags.gpio_update_request; EVID:bus:configReg.UIN.val; EVID:code:src/src/wifi_handler.cpp:handle_command_requests | TBD | integration |
| REQ-SC-24 | Slice C | http route handler | EVID:bus:configReg.whitelist_enabled; EVID:bus:configReg.whitelist_no1; EVID:bus:configReg.whitelist_inherit1; EVID:bus:statusReg.sms_log_type; EVID:bus:statusReg.modem_originator_number; EVID:bus:generalFlags.sms_request_user; EVID:code:src/src/wifi_handler.cpp:handle_command_requests | TBD | integration |
| REQ-SC-25 | Slice C | http route handler | EVID:entry:callback:HTTP route /config.json (GET); EVID:code:src/src/wifi_handler.cpp:handleConfigJSON; EVID:code:src/src/wifi_handler.cpp:build_config_json; EVID:code:src/src/wifi_handler.cpp:attach_session_cookie | TBD | integration |
| REQ-SC-26 | Slice C | http route handler | EVID:entry:callback:HTTP route /config_upload (POST body); EVID:call:src/src/wifi_handler.cpp→src/src/file_handler.cpp; EVID:bus:generalFlags.config_changed; EVID:code:src/src/wifi_handler.cpp:handleConfigUpload | TBD | integration |
| REQ-SC-27 | Slice C | http route handler | EVID:bus:configReg.PIN; EVID:bus:configReg.ftp_port; EVID:bus:configReg.whitelist_enabled; EVID:code:src/src/wifi_handler.cpp:apply_config_from_json | TBD | unit |
| REQ-SC-28 | Slice C | loop | EVID:bus:statusReg.flags.wifi_server_started; EVID:code:src/src/wifi_handler.cpp:wifi_start_ap | TBD | integration |
| REQ-SC-29 | Slice C | loop | EVID:bus:statusReg.flags.wifi_server_started; EVID:code:src/src/wifi_handler.cpp:wifi_start_ap | TBD | HIL |
| REQ-SC-30 | Slice C | http route handler | EVID:entry:callback:HTTP route /upload (POST body); EVID:code:src/src/wifi_handler.cpp:handle_upload | TBD | integration |
| REQ-SC-31 | Slice C | http route handler | EVID:entry:callback:HTTP route /config_upload (POST body); EVID:code:src/src/wifi_handler.cpp:handleConfigUpload | TBD | integration |
| REQ-SC-32 | Slice C | http route handler | EVID:entry:callback:HTTP route /config (POST body); EVID:bus:statusReg.flags.wifi_update_in_progress; EVID:bus:generalFlags.force_led_update; EVID:code:src/src/wifi_handler.cpp:handleDoUpdate | TBD | integration |
| REQ-SC-33 | Slice C | http route handler | EVID:entry:callback:HTTP route /login (POST); EVID:code:src/src/wifi_handler.cpp:authenticate_credentials; EVID:code:src/src/wifi_handler.cpp:record_failed_attempt; EVID:code:src/src/wifi_handler.cpp:record_unknown_failure; EVID:code:src/src/wifi_handler.cpp:is_login_locked | TBD | unit |
| REQ-SC-34 | Slice C | http route handler | EVID:code:src/src/wifi_handler.cpp:handle_command_requests; EVID:code:src/src/wifi_handler.cpp:is_slot_locked; EVID:code:src/src/wifi_handler.cpp:password_matches; EVID:code:src/src/wifi_handler.cpp:validate_password_strength | TBD | integration |
| REQ-SD-01 | Slice D | setup | EVID:entry:setup; EVID:entry:ISR:SysTimer | TBD | HIL |
| REQ-SD-02 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/adc_handler.cpp; EVID:call:src/main.cpp→src/src/button_handler.cpp; EVID:call:src/main.cpp→src/src/gpio_handler.cpp; EVID:call:src/main.cpp→src/src/power_handler.cpp; EVID:call:src/main.cpp→src/src/imu_handler.cpp; EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp; EVID:call:src/main.cpp→src/src/tft_handler.cpp | TBD | integration |
| REQ-SD-03 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/power_handler.cpp | TBD | integration |
| REQ-SD-04 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/adc_handler.cpp | TBD | integration |
| REQ-SD-05 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-06 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/button_handler.cpp | TBD | integration |
| REQ-SD-07 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/imu_handler.cpp | TBD | integration |
| REQ-SD-08 | Slice D | loop | EVID:entry:loop; EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp | TBD | integration |
| REQ-SD-09 | Slice D | loop | EVID:bus:generalFlags.external_voltage_request; EVID:call:src/main.cpp→src/src/power_handler.cpp | TBD | integration |
| REQ-SD-10 | Slice D | loop | EVID:bus:generalFlags.external_voltage_request; EVID:bus:statusReg.external_voltage; EVID:call:src/main.cpp→src/src/adc_handler.cpp | TBD | integration |
| REQ-SD-11 | Slice D | loop | EVID:bus:generalFlags.power_status_request; EVID:call:src/main.cpp→src/src/power_handler.cpp | TBD | unit |
| REQ-SD-12 | Slice D | loop | EVID:bus:gpio_controls.charge_enable; EVID:bus:configReg.battery_chargerate; EVID:bus:configReg.battery_low_level; EVID:bus:configReg.flags.charger_temperature_enable; EVID:call:src/main.cpp→src/src/power_handler.cpp | TBD | HIL |
| REQ-SD-13 | Slice D | loop | EVID:bus:generalFlags.wifi_request; EVID:call:src/main.cpp→src/src/button_handler.cpp | TBD | integration |
| REQ-SD-14 | Slice D | loop | EVID:bus:generalFlags.modem_reboot_request; EVID:bus:statusReg.flags.modem_registered; EVID:bus:statusReg.flags.modem_first_registration; EVID:call:src/main.cpp→src/src/button_handler.cpp | TBD | integration |
| REQ-SD-15 | Slice D | loop | EVID:bus:statusReg.flags.door_open; EVID:bus:statusReg.flags.hook_button_pressed; EVID:bus:statusReg.flags.handsfree_button1_pressed; EVID:bus:statusReg.flags.volume_button_up_pressed; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-16 | Slice D | loop | EVID:bus:statusReg.keypad_keypress; EVID:bus:statusReg.flags.io1_input_status; EVID:bus:statusReg.flags.io2_input_status; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-17 | Slice D | loop | EVID:bus:statusReg.flags.hook_button_pressed; EVID:bus:statusReg.flags.hook_button_released; EVID:bus:statusReg.flags.hook_button_stuck; EVID:bus:statusReg.flags.handsfree_button1_pressed; EVID:bus:statusReg.flags.handsfree_button1_released; EVID:bus:statusReg.flags.handsfree_button1_stuck; EVID:bus:statusReg.flags.volume_button_up_pressed; EVID:bus:statusReg.flags.volume_button_up_released; EVID:bus:statusReg.flags.volume_button_up_stuck; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-18 | Slice D | loop | EVID:bus:statusReg.flags.door_open; EVID:bus:statusReg.flags.door_stuck; EVID:bus:statusReg.flags.doorstuck_logtext_done; EVID:bus:configReg.flags.door_stuck_message_disable; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-19 | Slice D | loop | EVID:bus:configReg.telephone_operating_mode; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-20 | Slice D | loop | EVID:bus:configReg.flags.imu_enabled; EVID:bus:configReg.flags.imu_vertical_swap_enabled; EVID:bus:configReg.imu_tip_threshold; EVID:call:src/main.cpp→src/src/imu_handler.cpp | TBD | integration |
| REQ-SD-21 | Slice D | loop | EVID:bus:statusReg.imu_roll; EVID:bus:statusReg.imu_pitch; EVID:bus:statusReg.flags.imu_tilt_exceeded; EVID:bus:statusReg.flags.imu_tilt_reset; EVID:bus:statusReg.flags.imu_tilt_logtext_done; EVID:bus:statusReg.flags.imu_tilt_reset_logtext_done; EVID:call:src/main.cpp→src/src/imu_handler.cpp | TBD | integration |
| REQ-SD-22 | Slice D | loop | EVID:bus:configReg.status_led_brightness; EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp | TBD | HIL |
| REQ-SD-23 | Slice D | loop | EVID:bus:configReg.flags.led_board_status_enabled; EVID:bus:configReg.telephone_operating_mode; EVID:bus:generalFlags.force_led_update; EVID:bus:statusReg.flags.call_volte_active; EVID:bus:statusReg.flags.incoming_call_ringing; EVID:bus:statusReg.flags.modem_registered; EVID:bus:statusReg.flags.wifi_server_started; EVID:bus:statusReg.flags.wifi_update_in_progress; EVID:bus:statusReg.flags.sms_command_processed; EVID:call:src/main.cpp→src/src/leddisplay_handler.cpp | TBD | HIL |
| REQ-SD-24 | Slice D | loop | EVID:bus:statusReg.flags.tft_status; EVID:call:src/main.cpp→src/src/tft_handler.cpp | TBD | integration |
| REQ-SD-25 | Slice D | loop | EVID:bus:statusReg.flags.door_stuck; EVID:bus:statusReg.flags.doorstuck_logtext_done; EVID:bus:statusReg.flags.hook_button_stuck; EVID:bus:statusReg.flags.handsfree_button1_stuck; EVID:bus:statusReg.flags.handsfree_button2_stuck; EVID:bus:statusReg.flags.handsfree_button3_stuck; EVID:bus:statusReg.flags.handsfree_button4_stuck; EVID:bus:statusReg.flags.volume_button_up_stuck; EVID:bus:statusReg.flags.volume_button_dwn_stuck; EVID:call:src/main.cpp→src/src/gpio_handler.cpp | TBD | integration |
| REQ-SD-26 | Slice D | loop | EVID:bus:statusReg.flags.imu_tilt_exceeded; EVID:bus:statusReg.flags.imu_tilt_logtext_done; EVID:bus:statusReg.flags.imu_tilt_reset; EVID:bus:statusReg.flags.imu_tilt_reset_logtext_done; EVID:call:src/main.cpp→src/src/imu_handler.cpp | TBD | integration |
| REQ-SD-27 | Slice D | loop | EVID:bus:generalFlags.external_voltage_request; EVID:bus:statusReg.external_voltage; EVID:call:src/main.cpp→src/src/power_handler.cpp; EVID:call:src/main.cpp→src/src/adc_handler.cpp | TBD | integration |
| REQ-SD-28 | Slice D | loop | EVID:bus:statusReg.flags.tft_status; EVID:call:src/main.cpp→src/src/tft_handler.cpp | TBD | integration |
| REQ-SE-01 | Slice E | loop | EVID:call:src/main.cpp→src/src/test_handler.cpp; EVID:code:src/main.cpp:loop; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-02 | Slice E | test harness | EVID:bus:statusReg.keypad_keypress; EVID:bus:configReg.flags.audio_playback_enable; EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-03 | Slice E | test harness | EVID:bus:statusReg.keypad_keypress; EVID:code:src/src/test_handler.cpp:test_task | TBD | unit |
| REQ-SE-04 | Slice E | test harness | EVID:bus:statusReg.flags.handsfree_button1_pressed; EVID:bus:statusReg.flags.modem_registered; EVID:bus:configReg.handset_PGA_speaker_gain; EVID:bus:configReg.handset_PGA_microphone_gain; EVID:bus:gpio_controls.hshf_select; EVID:bus:gpio_controls.handsfree_control; EVID:bus:statusReg.telephone_mode; EVID:bus:statusReg.modem_originator_number; EVID:bus:generalFlags.call_initialisation_request; EVID:bus:statusReg.flags.call_inprogress; EVID:bus:statusReg.flags.off_hook_active; EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp; EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp; EVID:code:src/src/test_handler.cpp:test_task | TBD | HIL |
| REQ-SE-05 | Slice E | test harness | EVID:bus:statusReg.flags.handsfree_button2_pressed; EVID:bus:statusReg.flags.modem_registered; EVID:bus:statusReg.modem_originator_number; EVID:bus:generalFlags.sms_request_user; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-06 | Slice E | test harness | EVID:bus:statusReg.flags.handsfree_button3_pressed; EVID:bus:statusReg.flags.modem_registered; EVID:bus:statusReg.flags.incoming_call_ringing; EVID:bus:generalFlags.call_answer_request; EVID:bus:statusReg.flags.call_inprogress; EVID:bus:statusReg.flags.off_hook_active; EVID:bus:configReg.handset_PGA_speaker_gain; EVID:bus:configReg.handset_PGA_microphone_gain; EVID:bus:gpio_controls.hshf_select; EVID:bus:gpio_controls.handsfree_control; EVID:bus:statusReg.telephone_mode; EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp; EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp; EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp; EVID:code:src/src/test_handler.cpp:test_task | TBD | HIL |
| REQ-SE-07 | Slice E | test harness | EVID:bus:statusReg.flags.handsfree_button4_pressed; EVID:bus:statusReg.flags.modem_registered; EVID:bus:statusReg.flags.call_inprogress; EVID:bus:statusReg.flags.incoming_call_ringing; EVID:bus:generalFlags.call_hangup_request; EVID:bus:statusReg.flags.off_hook_active; EVID:bus:gpio_controls.hshf_select; EVID:bus:gpio_controls.handsfree_control; EVID:call:src/src/test_handler.cpp→src/src/modem_handler.cpp; EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-08 | Slice E | test harness | EVID:bus:generalFlags.incoming_ring_request; EVID:bus:gpio_controls.handsfree_control; EVID:bus:gpio_controls.ringer_control; EVID:bus:gpio_controls.hshf_select; EVID:bus:configReg.mimic_volume; EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp; EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp; EVID:code:src/src/test_handler.cpp:test_task | TBD | HIL |
| REQ-SE-09 | Slice E | test harness | EVID:bus:statusReg.flags.call_disconnected; EVID:bus:statusReg.flags.call_inprogress; EVID:bus:statusReg.flags.off_hook_active; EVID:bus:statusReg.flags.incoming_call_ringing; EVID:bus:gpio_controls.handsfree_control; EVID:bus:gpio_controls.ringer_control; EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp; EVID:call:src/src/test_handler.cpp→src/src/gpio_handler.cpp; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-10 | Slice E | test harness | EVID:bus:statusReg.flags.modem_registered; EVID:bus:statusReg.flags.handsfree_button1_pressed; EVID:bus:statusReg.flags.handsfree_button2_pressed; EVID:bus:statusReg.flags.handsfree_button3_pressed; EVID:bus:statusReg.flags.handsfree_button4_pressed; EVID:code:src/src/test_handler.cpp:test_task | TBD | unit |
| REQ-SE-11 | Slice E | test harness | EVID:bus:statusReg.modem_originator_number; EVID:bus:generalFlags.call_initialisation_request; EVID:bus:generalFlags.sms_request_user; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-12 | Slice E | test harness | EVID:bus:statusReg.telephone_mode; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-13 | Slice E | test harness | EVID:bus:generalFlags.incoming_ring_request; EVID:code:src/src/test_handler.cpp:test_task | TBD | unit |
| REQ-SE-14 | Slice E | test harness | EVID:bus:statusReg.flags.handsfree_button3_pressed; EVID:bus:statusReg.flags.modem_registered; EVID:bus:generalFlags.call_answer_request; EVID:code:src/src/test_handler.cpp:test_task | TBD | integration |
| REQ-SE-15 | Slice E | test harness | EVID:call:src/src/test_handler.cpp→src/src/audio_handler.cpp; EVID:bus:generalFlags.incoming_ring_request; EVID:code:src/src/test_handler.cpp:test_task | TBD | HIL |
| REQ-XC-03 | Cross-cutting | task: modem | EVID:call:src/main.cpp→src/src/modem_handler.cpp; EVID:code:src/src/modem_handler.cpp:modem_task; EVID:code:src/src/modem_handler.cpp:modem_decode; EVID:code:src/src/modem_handler.cpp:sms_task | TBD | manual |
| REQ-XC-04 | Cross-cutting | http route handler | EVID:bus:configReg.ftp_server; EVID:bus:configReg.ftp_user; EVID:bus:configReg.ftp_password; EVID:bus:configReg.ftp_port; EVID:bus:configReg.ftp_directory; EVID:bus:configReg.ftp_last_update_date; EVID:bus:configReg.flags.expansion_sim_enabled; EVID:bus:configReg.voice_no1; EVID:bus:configReg.sms_user_no1; EVID:bus:configReg.sms_logtext_no1 | TBD | integration |
| REQ-XC-05 | Cross-cutting | loop | EVID:bus:statusReg.ftp_file_date; EVID:bus:configReg.ftp_last_update_date; EVID:bus:generalFlags.ftp_update_download_request; EVID:bus:statusReg.flags.ftp_download_complete; EVID:bus:generalFlags.config_changed; EVID:code:src/main.cpp:loop; EVID:code:src/src/modem_handler.cpp:modem_decode | TBD | integration |
| REQ-XC-06 | Cross-cutting | task: modem | EVID:bus:configReg.PIN; EVID:bus:generalFlags.config_changed; EVID:code:src/src/modem_handler.cpp:sms_decode; EVID:code:src/src/modem_handler.cpp:sms_whitelist_allowed; EVID:code:src/src/modem_handler.cpp:log_auth_event | TBD | integration |
