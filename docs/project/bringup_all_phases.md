# GSM4 ESP32 (ESP-IDF) bring up plan, phased

This document is intended to be dropped into the repo and used as a reference for Codex in VS Code. It defines a staged bring up that preserves the rewrite architecture, keeps power safe, and provides clear acceptance criteria per phase.

---

## Board facts and constraints (do not regress)

- Programming and logging are via UART0: `RXD0` and `TXD0`.
- `SYS_EN` is active high with an external pull down. Keep low at boot until explicitly enabled.
- Shared I²C bus: `GPIO26` SCL, `GPIO27` SDA (PI2C == I2C).
- `GPIO_INT` is active low and externally pulled up (from KTS1622 expanders).
- `GLOBAL_INT` is active low (AND chain of active low contributors).
- LIS3DH interrupt is active low.
- `EXTPWR_CTL` is on KTS1622 `0x21` P0_7 and **HIGH disables PSU, LOW enables PSU**.
- `CHRG_EN` is on KTS1622 `0x21` P0_4 and is pulled low by default.
- BQ25622 INT is not connected, charger status must be polled.
- Expected I²C devices (7 bit addresses):
  - ES8388 `0x10`
  - DS4420 `0x50`, `0x52`
  - LIS3DH `0x18`
  - BQ25622 `0x6B`
  - KTS1622 `0x20`, `0x21`

---

## Phase 0, Deterministic power and observability

### Goals
1. Stable UART logs and clear boot telemetry.
2. Deterministic safe state at boot.
3. I²C bus proven and all devices enumerated.
4. Immediate control of expander power critical pins, especially `EXTPWR_CTL`.
5. Minimal safe configuration of BQ25622 with read back verification.
6. Clean seams for later services (Power, IO Expander, watchdog gating).

### Deliverables
- `components/bsp_gsm4_rev4/include/board_pins.h` (header source of truth)
- `docs/pins/esp32_pins_1-39_gsm4.md` (pin table)
- `docs/bringup/bringup_phases.md` (this file)
- `components/bsp_gsm4_rev4/board_init.c` and `include/bsp.h` with a single `bsp_init()` entry point
- Minimal drivers:
  - `components/drv_kts1622/` minimal register access and port config
  - `components/drv_bq25622/` minimal register access and key helpers

### Implementation steps (recommended order)
1. **UART logging and boot banner**
   - Print: reset reason, free heap, IDF version, board ID, git hash or version string.
2. **Minimal safe GPIO init (no I²C yet)**
   - `SYS_EN = 0` (keep domains off)
   - `DOOR_LUM` inactive (`1` because active low)
   - `BOOT_BUT` input with internal pull up
   - Avoid driving strap pins early (especially GPIO2 used as `MDM_PWRKEY`)
3. **I²C init and bus scan**
   - Start at 100 kHz
   - Scan and log all responding addresses
   - Compare with expected list and log missing/unexpected devices
4. **KTS1622 minimal init and power critical defaults**
   - Initialise `0x20` and `0x21`
   - Force `EXTPWR_CTL = 0` (PSU enabled) as the first critical action after expander init
   - Set other outputs to safe defaults: `CHRG_EN`, `+12V_EN`, `LOAD_CTL`, `BEACON_CTL`, `IO1_CTL`, `IO2_CTL` to known safe states (typically low unless policy says otherwise)
   - Log expander output and direction registers after applying defaults
5. **Prove `GPIO_INT`**
   - Configure one expander input to produce an interrupt on change
   - ISR reads expander state, clears cause, logs a single event (no app logic)
   - Ensure no interrupt storm
6. **BQ25622 minimal safe boot configuration**
   - Apply conservative limits (input current limit, system min voltage, thresholds)
   - Read back and verify written values
   - Log status and fault registers
   - Only enable charging (`CHRG_EN`) if/when policy says so
7. **Phase 0 health loop (optional, behind config flag)**
   - 1 Hz telemetry task logs: `VEXT_ADC`, charger status summary, `GPIO_INT` and `GLOBAL_INT` levels
   - Guard behind `CONFIG_GSM4_PHASE0_TELEMETRY`

### Acceptance criteria
- System boots repeatedly with stable logs.
- I²C scan reliably finds all expected devices.
- `EXTPWR_CTL` is deterministically driven to PSU enabled state (or your chosen policy) on every boot.
- Charger configuration reads back correctly and status logs are meaningful.
- Expander interrupt handling works without storms or missed events.
- Watchdog remains disabled in Phase 0.

---

## Phase 1, Core services scaffold (power and IO first)

### Goals
1. Promote Phase 0 drivers into services with clear APIs and ownership.
2. Make power behaviour a single authoritative state machine (no scattered register writes).
3. Make IO expander access safe, queued, and interrupt driven.

### Deliverables
- `power_service` component:
  - wraps `drv_bq25622`
  - exposes a small API: set limits, request mode, get telemetry, get faults
  - initial policy state machine: `BOOT_SAFE`, `EXTERNAL_POWER`, `BATTERY`, `CHARGING`, `FAULT`
- `ioexp_service` component:
  - wraps `drv_kts1622`
  - provides thread safe `set`, `get`, `config`, `subscribe` APIs
  - interrupt driven event queue from `GPIO_INT`
- `bsp_init()` updated to start services (but still minimal peripherals)

### Implementation steps
1. Define service interfaces (headers) before writing implementation.
2. Implement `ioexp_service` with a single owning task and a queue for operations.
3. Implement `power_service` with periodic polling of BQ25622 and a state machine.
4. Add structured telemetry objects (no printf spam in core services).
5. Add fault handling and safe recovery actions:
   - if I²C bus errors occur, attempt controlled re init and safe output re apply
   - if charger faults detected, constrain behaviour and log

### Acceptance criteria
- No direct BQ25622 register writes outside `power_service`.
- No direct KTS1622 register writes outside `ioexp_service`.
- `EXTPWR_CTL` and other power critical lines are managed only via `power_service` policy decisions.
- Event queue from `GPIO_INT` works under repeated toggling and does not flood.

---

## Phase 2, Input, UI, and low level peripheral proof points

### Goals
1. Make user inputs reliable (keypad, volume buttons, hook switch, door switch, etc.).
2. Prove any simple outputs (ringer control, beacon control, door lamp, etc.).
3. Establish a consistent event model for the application layer.

### Deliverables
- `input_service`:
  - consumes `ioexp_service` events
  - debouncing, long press, repeat, matrix scan (if keypad scanning is required)
  - publishes events to `event_bus`
- `event_bus` or `app_events` component:
  - central queue for application events with typed payloads
- Simple output drivers (may live in `ioexp_service` or separate `outputs_service`):
  - ringer control, beacon control, load control, +12V enable, etc.

### Acceptance criteria
- All physical inputs generate deterministic events.
- No missed presses under fast interactions.
- Outputs can be toggled safely under policy constraints.
- The application layer only deals with events and high level commands, not GPIO/I²C details.

---

## Phase 3, Modem bring up and connectivity

### Goals
1. Initialise and control the modem reliably under power policy.
2. Establish PPP or your chosen network stack path.
3. Implement robust modem recovery (timeouts, reset, power cycle via policy).

### Deliverables
- `modem_service`:
  - UART driver config, AT command transport layer
  - modem power sequencing using `MDM_PWRKEY` and expander controlled rails if applicable
  - state machine: `OFF`, `BOOTING`, `READY`, `REGISTERED`, `DATA`, `FAULT`
  - retry and backoff policy
- Network integration:
  - PPP bring up, DNS, NTP (if required)
  - connection watchdogs and health metrics

### Acceptance criteria
- Modem can be power sequenced repeatedly.
- Network comes up reliably.
- Recovery from common faults works without manual intervention.

---

## Phase 4, Audio bring up (codec, routing, and testability)

### Goals
1. Prove ES8388 I²C control and I²S audio streaming.
2. Establish routing control and gain staging.
3. Add deterministic audio self tests for manufacturing and debug.

### Deliverables
- `audio_service`:
  - codec init and configuration, sample rate, routing
  - I²S driver init, ring buffers
  - basic tone generation and loopback tests
- Audio calibration hooks (if required):
  - DS4420 control via I²C for gain control
  - stored calibration profiles per unit (NVS)

### Acceptance criteria
- Audio playback and capture works with stable clocks.
- Codec settings can be changed live without glitches beyond acceptable limits.
- A repeatable audio self test exists.

---

## Phase 5, Full system integration, robustness, and watchdogs

### Goals
1. Make the system robust: watchdog, brownout handling, recovery, and logs.
2. Finalise task priorities and memory usage.
3. Create production ready diagnostics.

### Deliverables
- Task watchdog and health model:
  - each service reports heartbeat
  - watchdog enabled only when the system reaches stable steady state
  - config gated: `CONFIG_GSM4_ENABLE_WATCHDOG`
- Persistent logs and crash capture:
  - store last fault reason, reset reason
  - optional ring buffer logs in RAM and dump on crash
- Manufacturing and field diagnostics:
  - I²C scan command
  - charger telemetry dump
  - expander state dump
  - modem state dump
  - audio self test command

### Acceptance criteria
- Watchdog can be enabled without false triggers.
- Device recovers from common fault modes (I²C hiccups, modem hangs, brownouts).
- Diagnostics provide enough data to debug field issues without re flashing.

---

## Phase checklist summary

- Phase 0: deterministic power + I²C proof + minimal charger config + expander power defaults
- Phase 1: services (power and IO expander), enforce ownership boundaries
- Phase 2: inputs and outputs with event model
- Phase 3: modem and connectivity
- Phase 4: audio and calibration testability
- Phase 5: robustness, watchdogs, diagnostics, production readiness
