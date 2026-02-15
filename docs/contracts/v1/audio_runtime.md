# Audio Runtime
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: audio runtime execution for playback and DTMF output (`audio_task`, `audio_playback`, `dtmf_task`), audio-test measurement state machine (`audio_test_task`), and codec/PGA control (`es8388_*`, `ds4420_*`).
- Purpose: local file decode pipeline driven by `Audio` + MP3 decoder backend (`audio.connecttoFS`, `MP3Decode`).
- Non goals: modem AT-command call audio setup is handled in `src/src/modem_handler.cpp` (`call_audio_setup`, `call_send_dtmf`).

## Owner execution context
- Primary execution context: `task: audio` via `audio_thread(void *param)` in `src/main.cpp`; `xTaskCreate(... audio_thread ...)` at `src/main.cpp:381`; task loop calls `audio_task()` then `vTaskDelay(1)` at `src/main.cpp:2953-2957`.
- External invocation contexts:
- `loop`: `src/main.cpp` calls `audio_playback`, `audio_playback_stop`, `audio_set_volume`, `audio_dtmf`, `audio_dtmf_stop`.
- `task: modem` shared-state producer for audio-test tone fields in `src/src/modem_handler.cpp` (`audio_test_tone_start`, `audio_test_cleanup`, `test_audio_setup`).
- `test harness`: `src/src/test_handler.cpp` calls `audio_dtmf`, `audio_set_volume`, `audio_playback`, `audio_playback_stop` in `test_task()`.

## Entry points
- Task entry:
- `void audio_thread(void *param)` (`src/main.cpp`) -> `audio_init()` then continuous `audio_task()`.
- Public API (`src/inc/audio_handler.h`) with external call sites:
- `void audio_task(void)` called from `audio_thread` (`src/main.cpp:2956`).
- `void audio_playback(String file)` called from `src/main.cpp` and `src/src/test_handler.cpp`.
- `void audio_playback_stop(void)` called from `src/main.cpp` and `src/src/test_handler.cpp`.
- `void audio_set_volume(uint8_t volume)` called from `src/main.cpp` and `src/src/test_handler.cpp`.
- `void audio_dtmf(char digit)` and `void audio_dtmf_stop(void)` called from `src/main.cpp` and `src/src/test_handler.cpp`.

## Inputs
- Calls in:
- `audio_task()` from `audio_thread` (`task: audio`).
- Loop/test-harness wrapper calls into `audio_playback`, `audio_set_volume`, `audio_dtmf`, `audio_dtmf_stop`.
- Events/callback-style shared fields consumed in `audio_test_task`:
- `generalFlags.audio_test_request` (read+clear in `audio_test_task`).
- `generalFlags.audio_test_abort_request` (read+clear in `audio_test_task`).
- `statusReg.flags.audio_test_tone_active` and `statusReg.audio_test_tone_tick` (read in `audio_test_task` tone-start wait).
- Data in:
- `configReg.telephone_operating_mode` read in `audio_test_task`.
- Function arguments `file`, `volume`, and `digit` from external callers.

## Outputs and side effects
- Audio/file pipeline:
- `audio.connecttoFS(SPIFFS, file, true)` in `audio_playback`.
- DTMF wave file opens (`SPIFFS.open("/d0.wav"..."/tone.wav")`) and I2S output via `i2s_write` in `dtmf_tone_output`.
- Audio-test capture via `i2s_read` in `audio_test_task`.
- Codec/PGA programming:
- `es8388_audio.es8388_set_mic_gain`, `es8388_audio.es8388_config_adc_input`.
- `ds4420_set_gain`, `ds4420_mute`.
- Shared state writes:
- `statusReg.flags.audio_digit_playing` written in `dtmf_task`.
- `statusReg.flags.audio_test_passed` and `statusReg.flags.audio_test_complete` written in `audio_test_task`.
- `generalFlags.audio_test_tone_start_request` written in `audio_test_task`.
- `generalFlags.audio_test_request` set internally for PRIME->REAL re-run in `audio_test_task`.

## Owned state
- `src/src/audio_handler.cpp` persistent globals/statics:
- `Audio audio`, `ES8388 es8388_audio`, `DS4420 ds4420_audio1/2/3`, `File dtmf_file`.
- `static volatile char dtmf_pending_digit`, `static volatile bool dtmf_request_stop`.
- `static bool audio_intialised`, `static uint8_t es8388_buffer[]`.
- `static uint8_t audio_test_process`, `audio_test_phase`; `static int32_t audio_test_average`, `audio_test_calibrate_average`; `static uint16_t audio_test_sets`; `static bool audio_test_internal_request`.
- MP3 decoder backend globals are in `src/audio/mp3_decoder/mp3_decoder.cpp`: `m_MP3DecInfo`, `m_FrameHeader` (plus related decoder state), consumed from `src/audio/Audio.cpp` (`MP3Decoder_AllocateBuffers`, `MP3Decode`).

## Timing and triggering
- Audio runtime cadence: `audio_thread` loop with `vTaskDelay(1)` calls `audio_task()` continuously.
- DTMF playback timing: `dtmf_task` starts on queued digit and stops tone after `200 ms` window (`TickGet` comparison in `dtmf_task`).
- Audio-test timing windows in `audio_test_task`:
- calibration duration uses `TickGet` vs `TICK_SECOND`.
- tone start timeout uses `TickGet` vs `TICK_SECOND * 2`.
- measurement start/end windows use `AUDIO_TEST_RECORD_START_MS`, `AUDIO_TEST_TONE_DURATION_MS`, and `AUDIO_TEST_RECORD_END_EARLY_MS`.

## Concurrency and ordering rules
- `dtmf_pending_digit` and `dtmf_request_stop` are `volatile`; writers are `loop`/`test harness` (`audio_dtmf`, `audio_dtmf_stop`), reader/clearer is `task: audio` (`dtmf_task`).
- `generalFlags.audio_test_request`, `generalFlags.audio_test_tone_start_request`, `generalFlags.audio_test_cleanup_request`, and `generalFlags.audio_test_abort_request` are written/cleared across `loop`, `task: audio`, and `task: modem`.
- `statusReg.audio_test_tone_tick` and `statusReg.flags.audio_test_tone_active` are written in `loop` and `task: modem`; read in `task: audio`.
- `Audio audio` object is not task-confined: `audio_thread -> audio_task -> audio.loop()` runs in `task: audio` (`src/main.cpp:2953-2957`, `src/src/audio_handler.cpp:399-405`), while mutating calls `audio.connecttoFS`, `audio.force_stop`, and `audio.setVolume` are reached from `loop`/`test_task` via `audio_playback`, `audio_playback_stop`, and `audio_set_volume` (`src/src/audio_handler.cpp:453-457`, `src/src/audio_handler.cpp:479-483`, `src/src/audio_handler.cpp:504-506`, `src/main.cpp:411-412`, `src/src/test_handler.cpp:123-125`).
- No `xSemaphore`, mutex, `portENTER_CRITICAL`, or atomic operations were found around `Audio audio` wrapper calls in `src/src/audio_handler.cpp` or inside `src/audio/Audio.cpp`.

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Modem Core | shared-state | `src/src/audio_handler.cpp::audio_test_task` writes `generalFlags.audio_test_tone_start_request`; `src/src/modem_handler.cpp::modem_task` consumes it and calls `audio_test_tone_start()`. |
| Timebase | call | `src/src/audio_handler.cpp::audio_test_task` and `src/src/audio_handler.cpp::dtmf_task` call `TickGet()`. |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| System Orchestrator | call | `src/main.cpp` creates `audio_thread` with `xTaskCreate`; `loop()` also calls `audio_playback`, `audio_set_volume`, `audio_dtmf`, and `audio_dtmf_stop`. |
| Config Store | shared-state | `src/src/file_handler.cpp::config_default/config_read` populate audio-related `configReg` fields; `src/main.cpp` consumes them in `audio_set_volume(...)` and `mimic_dial()` timing. |
| WiFi/Web Control Plane | shared-state | `src/src/wifi_handler.cpp::handle_command_requests/apply_config_from_json` write audio-related `configReg` values; `src/main.cpp` and `src/src/audio_handler.cpp` consume those fields. |
| Modem Core | shared-state | `src/src/modem_handler.cpp::audio_test_tone_start/audio_test_cleanup` write `statusReg.audio_test_tone_tick` and `statusReg.flags.audio_test_tone_active`; `src/src/audio_handler.cpp::audio_test_task` reads both. |
| In-call DTMF Control | call | `src/src/dtmf_handler.cpp::dtmf_decode_task` calls `ds4420_mute` and `ds4420_set_gain` provided by `src/src/audio_handler.cpp`. |

## Uncertainties to verify
- `test_task()` call path in `src/main.cpp` is compiled only in the `#else` branch around `MODEM_DISABLED`; production build selection is not proven in this pass.
- Ordering between HTTP-driven `configReg` writes and loop/task-audio reads is not explicitly synchronized in the inspected files.
- `Audio` class internal thread-safety for concurrent `audio.loop()` and loop/test-harness mutation calls is UNCERTAIN from inspected code; explicit synchronization for these call paths was not found in `src/src/audio_handler.cpp` or `src/audio/Audio.cpp`.

## Evidence sources
- `src/main.cpp`: `audio_thread`, `loop` call sites for `audio_*`, audio-test request flag writes, `xTaskCreate(... audio_thread ...)`.
- `src/src/audio_handler.cpp`: `audio_task`, `audio_playback`, `audio_set_volume`, `audio_dtmf`, `dtmf_task`, `audio_test_task`, shared-field reads/writes.
- `src/src/modem_handler.cpp`: `modem_task`, `test_audio_setup`, `audio_test_tone_start`, `audio_test_cleanup`.
- `src/src/dtmf_handler.cpp`: `dtmf_decode_task` calls into `ds4420_mute` and `ds4420_set_gain`.
- `src/src/test_handler.cpp`: `test_task` calls into `audio_*`.
- `src/audio/Audio.cpp`: `audio.connecttoFS`, `MP3Decoder_AllocateBuffers`, `MP3Decode`.
- `src/audio/mp3_decoder/mp3_decoder.cpp`: decoder globals (`m_MP3DecInfo`, `m_FrameHeader`).
- `docs/analysis/13_system_recomposition.md`
- `docs/analysis/13_dependency_edges.csv`
- `docs/modules/timebase.md`
- `docs/modules/modem_core.md`
- `docs/modules/wifi_web_control_plane.md`
- `docs/modules/config_store.md`
