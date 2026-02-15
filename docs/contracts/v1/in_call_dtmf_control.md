# In-call DTMF Control
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: `CR04` In-call DTMF command decode and actuator control.
- Non goals: none stated in code.

## Owner execution context
- `loop` (proven).
- Evidence:
  - `loop` calls `dtmf_decode_task()` in `IDLE_RUN` (`src/main.cpp:885`).
  - `loop` calls `submode_task()` in `HANDSET`/`HANDSFREE` (`src/main.cpp:1080`, `src/main.cpp:1165`), and `submode_task()` calls `dtmf_decode_task()` (`src/main.cpp:2635`).

## Entry points
- `bool dtmf_decode_task(void)` (`src/src/dtmf_handler.cpp:60`, declared in `src/inc/dtmf_handler.h:4`).
  - Called from `src/main.cpp:885` (`loop` -> `IDLE_RUN`).
  - Called from `src/main.cpp:2635` (`loop` -> `submode_task`).

## Inputs
- Trigger read: `statusReg.modem_response == ATRQTONEDET` in `dtmf_decode_task()` (`src/src/dtmf_handler.cpp:65`).
- Digit read: `statusReg.modem_dtmf_digit` (`src/src/dtmf_handler.cpp:68`).
- PIN read: `configReg.PIN[digit_pointer++]` (`src/src/dtmf_handler.cpp:87`).
- Call-state/config reads used for restore path:
  - `statusReg.flags.call_connected` (`src/src/dtmf_handler.cpp:80`, `src/src/dtmf_handler.cpp:95`, `src/src/dtmf_handler.cpp:130`, `src/src/dtmf_handler.cpp:148`, `src/src/dtmf_handler.cpp:193`)
  - `statusReg.telephone_mode` (`src/src/dtmf_handler.cpp:96`, `src/src/dtmf_handler.cpp:131`, `src/src/dtmf_handler.cpp:149`, `src/src/dtmf_handler.cpp:194`)
  - `configReg.handset_PGA_speaker_gain` / `configReg.handsfree_PGA_speaker_gain` (`src/src/dtmf_handler.cpp:97`, `src/src/dtmf_handler.cpp:99`, `src/src/dtmf_handler.cpp:132`, `src/src/dtmf_handler.cpp:134`, `src/src/dtmf_handler.cpp:150`, `src/src/dtmf_handler.cpp:152`, `src/src/dtmf_handler.cpp:195`, `src/src/dtmf_handler.cpp:197`)

## Outputs and side effects
- Shared-state writes:
  - `statusReg.modem_response = 0` (consume/clear trigger) (`src/src/dtmf_handler.cpp:69`).
  - `generalFlags.load_activate_request = true` (`src/src/dtmf_handler.cpp:102`).
  - `gpio_controls.load_control = 0` (`src/src/dtmf_handler.cpp:126`).
- Cross-module calls:
  - `ds4420_mute(...)` (`src/src/dtmf_handler.cpp:81`, `src/src/dtmf_handler.cpp:113`) -> implemented in `src/src/audio_handler.cpp:294`.
  - `ds4420_set_gain(...)` (`src/src/dtmf_handler.cpp:97`, `src/src/dtmf_handler.cpp:99`, `src/src/dtmf_handler.cpp:132`, `src/src/dtmf_handler.cpp:134`, `src/src/dtmf_handler.cpp:150`, `src/src/dtmf_handler.cpp:152`, `src/src/dtmf_handler.cpp:195`, `src/src/dtmf_handler.cpp:197`) -> implemented in `src/src/audio_handler.cpp:332`.
  - `gpio_update()` (`src/src/dtmf_handler.cpp:127`) -> implemented in `src/src/gpio_handler.cpp:575`.

## Owned state
- File-scope persistent decoder state in `src/src/dtmf_handler.cpp`:
  - `static uint32_t dtmf_tick` (`:29`)
  - `static bool digit_compare` (`:30`)
  - `static bool digit_program` (`:31`)
  - `static bool digit_okay` (`:32`)
  - `static uint8_t digit_pointer` (`:33`)
  - `static uint8_t digit_command` (`:34`)
- Reset points:
  - successful compare/program completion (`src/src/dtmf_handler.cpp:93-94`, `src/src/dtmf_handler.cpp:128-129`, `src/src/dtmf_handler.cpp:146-147`)
  - timeout reset (`src/src/dtmf_handler.cpp:190-192`)

## Timing and triggering
- Invocation timing: polled from `loop` call paths (`src/main.cpp:885`, `src/main.cpp:2635`).
- Tick source: `TickGet()` used for event timestamp and timeout comparison (`src/src/dtmf_handler.cpp:75`, `src/src/dtmf_handler.cpp:189`).
- Timeout constant: `DTMF_TIMEOUT_PERIOD` defined as `10u` in `src/inc/dac_defines.h:138`; used in `TICK_SECOND * DTMF_TIMEOUT_PERIOD` gate (`src/src/dtmf_handler.cpp:189`).

## Concurrency and ordering rules
- Producer/consumer ordering:
  - consumer: `dtmf_decode_task()` reads `statusReg.modem_response` / `statusReg.modem_dtmf_digit` and clears `statusReg.modem_response` (`src/src/dtmf_handler.cpp:65`, `src/src/dtmf_handler.cpp:68-69`).
  - producer(s): modem decode path writes both fields (`src/src/modem_handler.cpp:5094-5095`, `src/src/modem_handler.cpp:5028-5029`); loop also writes `statusReg.modem_dtmf_digit` from keypad (`src/main.cpp:2629`).
  - loop keypad path writes `statusReg.modem_dtmf_digit` as well; semantics may differ.
- Synchronisation:
  - no `xSemaphore`, mutex, `portENTER_CRITICAL`, or atomic operation around these fields in `dtmf_handler.cpp`, `modem_handler.cpp`, or `main.cpp` for this path (search evidence).

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Audio Runtime | call | `src/src/dtmf_handler.cpp:81,97,99,113,132,134,150,152,195,197` calls `ds4420_mute`/`ds4420_set_gain`; implementations in `src/src/audio_handler.cpp:294,332` |
| Power and Physical IO | shared-state | `src/src/dtmf_handler.cpp:126` writes `gpio_controls.load_control` |
| Power and Physical IO | call | `src/src/dtmf_handler.cpp:127` calls `gpio_update()` implemented in `src/src/gpio_handler.cpp:575` |
| Timebase | call | `src/src/dtmf_handler.cpp:75,189` calls `TickGet()` for DTMF timestamp and timeout gate |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| System Orchestrator | call | `src/main.cpp:885` (`loop` IDLE_RUN) and `src/main.cpp:2635` (`submode_task`) call `dtmf_decode_task()` |
| Modem Core | shared-state | `src/src/modem_handler.cpp:5094-5095` writes `statusReg.modem_dtmf_digit`/`statusReg.modem_response=ATRQTONEDET`; `src/src/dtmf_handler.cpp:65-69` consumes them |

## Uncertainties to verify
- Execution-context provenance for modem decode writer sites `src/src/modem_handler.cpp:5028-5029` and `src/src/modem_handler.cpp:5094-5095` is UNCERTAIN when those writes are reached via vendor Thingstream callback dispatch not fully visible in this repository.

## Evidence sources
- `src/src/dtmf_handler.cpp` (`dtmf_decode_task`, static decoder state, TickGet timeout gate, DS4420/gpio calls)
- `src/main.cpp` (`loop` call sites at `IDLE_RUN` and `submode_task` path)
- `src/src/audio_handler.cpp` (`ds4420_mute`, `ds4420_set_gain` implementations)
- `src/src/gpio_handler.cpp` (`gpio_update` implementation)
- `src/src/modem_handler.cpp` (`modem_decode` writes to `statusReg.modem_response` and `statusReg.modem_dtmf_digit`)
- `src/inc/dac_defines.h` (`DTMF_TIMEOUT_PERIOD`)
- `docs/analysis/13_system_recomposition.md`
- `docs/analysis/13_dependency_edges.csv`
- `docs/refactor_analysis/agents/03_entry_points.md` (canonical `docs/analysis/03_entry_points.md` not present)
- `docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md` (canonical `docs/analysis/A_calls_cluster_all_cpp.md` not present)
- `docs/refactor_analysis/agents/05_bus_access_union_fn_refined.md` (canonical `docs/analysis/05_bus_access_union_fn_refined.md` not present)
