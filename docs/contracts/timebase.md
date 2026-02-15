# Timebase
Contract level: v1.1 (code-verified)

## Purpose and non goals
- Purpose: provide monotonic tick reads (`TickGet()`, `Platform_getTimeMillis()`) and sleep-time compensation (`Platform_jumpTick(uint32_t)`).
- Non goals: define higher-level scheduling policy outside timer/tick APIs.

## Owner execution context
- multi-owner: `ISR` and `loop`.
- Evidence:
  - ISR writer: `void IRAM_ATTR SysTimer()` updates `upTime` (`src/main.cpp:100-106`).
  - Loop writer: `Platform_jumpTick(rtc_dif)` in loop wake path (`src/main.cpp:866`) updates `upTime` (`src/src/tick.cpp:126-128`).

## Entry points
- `uint32_t Platform_getTimeMillis(void)` (`src/src/tick.cpp:41`)
  - direct callers found in `src/`: `TickGet()` and `Platform_getTimeString()` in `tick.cpp` (`src/src/tick.cpp:69`, `src/src/tick.cpp:103`).
- `uint32_t TickGet(void)` (`src/src/tick.cpp:101`)
  - caller files in `src/`: `src/main.cpp`, `src/src/modem_handler.cpp`, `src/src/gpio_handler.cpp`, `src/src/audio_handler.cpp`, `src/src/power_handler.cpp`, `src/src/button_handler.cpp`, `src/src/dtmf_handler.cpp`, `src/src/imu_handler.cpp`, `src/src/leddisplay_handler.cpp`.
- `void Platform_jumpTick(uint32_t jump_period)` (`src/src/tick.cpp:126`)
  - caller: `src/main.cpp:866`.
- `const char* Platform_getTimeString(void)` (`src/src/tick.cpp:65`)
  - internal helper (no external call sites found via `rg -n "\\bPlatform_getTimeString\\s*\\(" src`).

## Inputs
- hardware timer registration to `SysTimer`:
  - setup path: `timerBegin` + `timerAttachInterrupt(..., &SysTimer, ...)` + `timerAlarmWrite(1000, true)` + `timerAlarmEnable` (`src/main.cpp:172-175`)
  - restart path: same registration sequence (`src/main.cpp:314-317`)
- wake compensation input: `rtc_dif` passed to `Platform_jumpTick(rtc_dif)` (`src/main.cpp:864-866`).

## Outputs and side effects
- updates shared `upTime`:
  - periodic `+1` in ISR (`SysTimer`)
  - delta add in loop wake path (`Platform_jumpTick`)
- returns tick value via `Platform_getTimeMillis()` / `TickGet()`.

## Owned state
- `volatile uint32_t upTime` definition: `src/main.cpp:47`.
- declaration: `extern volatile uint32_t upTime` (`src/inc/dac_defines.h:841`).

## Timing and triggering
- periodic tick trigger source: hardware timer ISR `SysTimer` (`IRAM_ATTR`) registered with `timerAttachInterrupt` and alarm set to `1000` ticks (`src/main.cpp:172-175`, `src/main.cpp:314-317`).
- sleep compensation trigger source: immediately after `esp_light_sleep_start()` returns in loop wake path (`src/main.cpp:860-866`).

## Concurrency and ordering rules
- Writer contexts:
  - ISR writer (`SysTimer`) uses `portENTER_CRITICAL_ISR(&timerMux)` / `portEXIT_CRITICAL_ISR(&timerMux)` around `upTime += 1` (`src/main.cpp:101-106`).
  - loop writer (`Platform_jumpTick`) performs `upTime += jump_period` without critical section (`src/src/tick.cpp:126-128`).
- Reader contexts:
  - loop-heavy reads in `src/main.cpp` (`TickGet()` callsites).
  - task reads in modem/audio paths (`src/src/modem_handler.cpp`, `src/src/audio_handler.cpp`).
  - additional loop-handler reads in power/gpio/button/imu/led paths.
- Sequencing evidence for `Platform_jumpTick` vs ISR:
  - `Platform_jumpTick(rtc_dif)` is called in `loop()` wake path (`src/main.cpp:850-867`).
  - no `timerAlarmDisable`, `timerDetachInterrupt`, `timerEnd`, `noInterrupts`, `portDISABLE_INTERRUPTS`, or non-ISR `portENTER_CRITICAL` is present at/around that callsite in `src/main.cpp`.
  - therefore timer/interrupt exclusion for `Platform_jumpTick` at `loop()` wake path is not proven from source.

## Dependencies (verified)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Power and Physical IO | call | 12_sliceD_pass6.md C1 `TickGet` -> 12_sliceD_pass6.md C2 `power_task` and C3 `button_task`/`gpio_task` |
| Sensor and Visual UI | call | 12_sliceD_pass6.md C1 `TickGet` -> 12_sliceD_pass6.md C4 `imu_task` and C5 `leddisplay_task` |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| System Orchestrator | call | `src/main.cpp` calls `TickGet()` in `loop()` (e.g., `src/main.cpp:215`, `src/main.cpp:320`, `src/main.cpp:869`); no direct `Platform_getTimeMillis()` call in `src/main.cpp` via `rg`. |
| Audio Runtime | call | `src/src/audio_handler.cpp` calls `TickGet()` (9 call sites, `A_calls_cluster_all_cpp.md` row to `src/src/tick.cpp`). |
| In-call DTMF Control | call | `src/src/dtmf_handler.cpp` calls `TickGet()` (2 call sites, `A_calls_cluster_all_cpp.md` row to `src/src/tick.cpp`). |
| Modem Core | call | `src/src/modem_handler.cpp` calls `TickGet()` (36 call sites, `A_calls_cluster_all_cpp.md` row to `src/src/tick.cpp`). |
| Power and Physical IO | call | `src/src/gpio_handler.cpp` (18), `src/src/power_handler.cpp` (4), `src/src/button_handler.cpp` (3) call `TickGet()` (`A_calls_cluster_all_cpp.md` rows to `src/src/tick.cpp`). |
| Sensor and Visual UI | call | `src/src/imu_handler.cpp` (2) and `src/src/leddisplay_handler.cpp` (2) call `TickGet()` (`A_calls_cluster_all_cpp.md` rows to `src/src/tick.cpp`). |

## Uncertainties to verify
- Exact exclusion behavior between timer ISR `SysTimer` (`src/main.cpp:100-106`) and `Platform_jumpTick(rtc_dif)` in `loop()` wake path (`src/main.cpp:850-867`) is UNCERTAIN because no timer/interrupt disable or non-ISR critical section is shown at the jump callsite.

## Evidence sources
- `docs/modules/timebase.md` (previous v1 content).
- `docs/analysis/13_system_recomposition.md` (module mapping for incoming edge attribution).
- `src/main.cpp` (`upTime` definition, `SysTimer`, timer register/restart, sleep wake path and `Platform_jumpTick` callsite).
- `src/src/tick.cpp` (`Platform_getTimeMillis`, `TickGet`, `Platform_getTimeString`, `Platform_jumpTick`).
- `docs/refactor_analysis/agents/03_entry_points.md` (`SysTimer` ISR inventory + timer attach evidence).
- `docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md` (`src/src/tick.cpp` inbound call-site counts by caller file).
- `rg -n` verification in `src/` for `Platform_getTimeString`, `TickGet`, `Platform_getTimeMillis`, timer-alarm APIs, and critical-section/interrupt APIs in `main.cpp` + `tick.cpp`.
