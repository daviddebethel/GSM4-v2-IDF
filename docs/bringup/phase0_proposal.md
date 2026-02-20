# GSM4 ESP32 (ESP-IDF) Phase 0 Bring-up Proposal

Status: Proposal only (no C code in this change)
Date: 2026-02-19
Scope: Deterministic bring-up, observability, and security-ready foundations

## Reference inputs used

Primary references requested:
- `board_pins_extpwr_update.h` (not found in workspace)
- `phase0_bringup_next_steps.md` (not found in workspace)
- `docs/project/bringup_all_phases.md` (found and used)
- `docs/pins/esp32_pins_1-39_gsm4.md` (not found in workspace)

Resolved source-of-truth inputs used in this repo:
- `firmware/components/bsp_gsm4_rev4/include/board_pins.h` (contains EXTPWR semantics and explicit boot cautions)
- `docs/project/bringup_all_phases.md` (used as Phase 0 step authority)
- `docs/hardware/ESP32_PINOUTS.md` (used as current pin table substitute)
- Additional Phase 0 constraints from docs scan:
  - `docs/requirements/LEGACY_FUNCTIONAL_REQUIREMENTS.md`
  - `docs/contracts/v1/power_and_physical_io.md`
  - `docs/architecture/IDF_TARGET_ARCH.md`
  - `docs/tests/ACCEPTANCE_INVARIANTS.md`
  - `docs/CODEX_RULES.md`
  - `docs/security/security_policy.md`
  - `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`
  - `docs/project/DECISIONS.md`

Conflict resolution policy applied:
- Where older notes conflict with board mapping details, `board_pins.h` wins.
- Noted conflict: `docs/hardware/ESP32_PINOUTS.md` describes `GLOBAL_INT` as active high, while `board_pins.h` and `bringup_all_phases.md` describe active low. Phase 0 implementation will follow `board_pins.h` (active low, idle high).

## A) What I discovered about the repo layout

- Monorepo layout:
  - Firmware project: `firmware/`
  - Project documentation: `docs/`
  - Build/test scripts: `tools/`
- Firmware structure relevant to Phase 0:
  - `firmware/components/bsp_gsm4_rev4/` exists and already hosts `board_pins.h`
  - `firmware/main/main.c` is slim and currently just logs heartbeat
  - `firmware/main/CMakeLists.txt` already depends on `bsp_gsm4_rev4`
- Existing component/test patterns:
  - Component-local tests exist in `firmware/components/gsm4_example/test/`
  - Host Unity test runners exist in:
    - `firmware/tests/host/unit/`
    - `firmware/tests/host/mock/`
  - CI/local scripts already run these patterns:
    - `tools/test_unit.sh`
    - `tools/test_mock.sh`
- Kconfig/profile pattern exists:
  - `firmware/main/Kconfig.projbuild` defines profile and debug gating options
- Docs location and status:
  - `docs/` exists at repo root and is the right location for this proposal
  - `docs/bringup/` exists and now hosts this Phase 0 proposal
  - `docs/security/` exists and now includes `security_policy.md` (draft)

How Phase 0 will fit this structure:
- Extend existing `bsp_gsm4_rev4` for board-safe init sequencing.
- Add low-level driver components for KTS1622 and BQ25622.
- Keep `main/` thin by calling a single `bsp_init()` entry.
- Follow current test style: component-local Unity tests plus host runner integration.

## B) Summary of Phase 0 goals

- Deterministic safe boot power state
- I2C bring-up and bus scan
- KTS1622 minimal init with power critical defaults, especially EXTPWR_CTL
- BQ25622 minimal safe boot config and read-back verification
- Prove GPIO_INT interrupt handling
- Optional telemetry behind config flag

## C) Legacy constraints applied in Phase 0

Only constraints relevant to Phase 0 are applied below. Non-Phase-0 requirements are explicitly deferred.

| Legacy/Docs constraint | Source | Phase 0 action |
|---|---|---|
| UART diagnostics are an expected operational behavior during bring-up | `bringup_all_phases.md`, `LEGACY_FUNCTIONAL_REQUIREMENTS.md` (REQ-XC diagnostics), `ESP_IDF_SETUP_GUIDE.md` serial-first | Use UART0 boot banner + structured bring-up logs for GPIO/I2C/KTS/BQ/INT status |
| Safe pre-init ordering is required before full run behavior | `LEGACY_FUNCTIONAL_REQUIREMENTS.md` (REQ-SB-01/02, REQ-SD-02) | Enforce deterministic sequence: safe GPIO defaults first, then I2C, then expander defaults, then charger config |
| Power/IO behavior is historically loop/poll driven; charger INT is unavailable | `bringup_all_phases.md` (BQ INT not connected), `contracts/v1/power_and_physical_io.md` | Use polling for BQ25622 status and verification in Phase 0 |
| Charger control must be policy-owned and safe by default | `bringup_all_phases.md` (`CHRG_EN` pulled low), `LEGACY_FUNCTIONAL_REQUIREMENTS.md` (REQ-SD-12) | Keep `CHRG_EN` in safe state until explicit charger policy/config step |
| GPIO/expander updates should be centralized, not scattered | `contracts/v1/power_and_physical_io.md`, `IDF_TARGET_ARCH.md` ownership rules | Concentrate Phase 0 hardware writes in BSP + low-level driver APIs |
| Security baseline forbids secret leakage in logs/debug outputs | `ACCEPTANCE_INVARIANTS.md`, `CODEX_RULES.md`, `IDF_TARGET_ARCH.md` | Define strict no-secret logging policy in Phase 0 and avoid credential dumps |
| Project-wide security policy requires secure defaults, debug-surface gating, and production default-off attack-surface controls | `docs/security/security_policy.md` (3.2, 3.3, 13, 15) | In Phase 0 keep network bring-up disabled, keep AT pass-through/debug endpoints off by default, and gate diagnostics by build/config flags |
| Sensitive credential fields must not be blindly ingested/echoed | `LEGACY_FUNCTIONAL_REQUIREMENTS.md` (REQ-SC-27) | No network/config import endpoints in Phase 0; no credential-bearing config paths exposed |
| Watchdog should not gate initial bring-up | `bringup_all_phases.md` | Keep watchdog disabled in Phase 0 |

Explicitly deferred beyond Phase 0:
- WiFi/AP/web routes, modem AT runtime, audio path, IMU/UI behavior, config migration mechanics, OTA, secure boot enablement, flash encryption enablement, NVS encryption enablement.

## D) Component and file layout plan (ESP-IDF)

This plan follows existing repo conventions (`components/<name>/`, thin `main/`, component-local tests).

### BSP

- Reuse and extend existing component: `firmware/components/bsp_gsm4_rev4/`
- Planned files:
  - `firmware/components/bsp_gsm4_rev4/include/bsp.h`
    - Public BSP init APIs (`bsp_init`, GPIO safe defaults, I2C init/scan)
  - `firmware/components/bsp_gsm4_rev4/include/board_pins.h`
    - Board pin/address source of truth (already present)
  - `firmware/components/bsp_gsm4_rev4/src/bsp_init.c`
    - Ordered bring-up sequence orchestrator
  - `firmware/components/bsp_gsm4_rev4/src/bsp_gpio.c`
    - Early safe GPIO policy application
  - `firmware/components/bsp_gsm4_rev4/src/bsp_i2c.c`
    - I2C init and scan helper
  - `firmware/components/bsp_gsm4_rev4/src/bsp_interrupts.c`
    - GPIO_INT setup and minimal ISR/event handling proof

Board header placement decision:
- Repo already has board header placement under BSP include.
- Keep `board_pins.h` in `firmware/components/bsp_gsm4_rev4/include/`.
- If `board_pins_extpwr_update.h` is later provided, merge/rename into this path while preserving the same source-of-truth role.

### Minimal drivers

- `firmware/components/drv_kts1622/`
  - `include/drv_kts1622.h`
  - `src/drv_kts1622.c`
  - Responsibilities: register read/write, direction config, output apply/readback, safe-default application helper
- `firmware/components/drv_bq25622/`
  - `include/drv_bq25622.h`
  - `src/drv_bq25622.c`
  - Responsibilities: register read/write, bitfield helpers, safe boot config, read-back verification

### Mockable I2C boundary

- New thin abstraction component:
  - `firmware/components/i2c_bus/`
    - `include/i2c_bus.h`
    - `src/i2c_bus_idf.c`
- Responsibility:
  - Provide a replaceable function table (`read_reg`, `write_reg`, `read_block`, `write_block`) used by both drivers
  - Allow host/mock tests to replace hardware I2C with fake bus behavior

### Tests

- Component-local tests (Unity style):
  - `firmware/components/drv_kts1622/test/`
  - `firmware/components/drv_bq25622/test/`
  - `firmware/components/bsp_gsm4_rev4/test/` (pure logic helpers only)
- Host runner integration:
  - Extend `firmware/tests/host/unit/main/` and `firmware/tests/host/mock/main/`
  - Keep execution through existing scripts `tools/test_unit.sh` and `tools/test_mock.sh`

## E) Deterministic boot sequence (ordered, exact)

1. UART boot banner
- Print board ID/revision, IDF version, reset reason, build profile, and Phase 0 mode.

2. Safe GPIO defaults before I2C
- Set `SYS_EN` low.
- Set `DOOR_LUM` inactive (high, active-low output).
- Configure `BOOT_BUT` (GPIO0) as input with pull-up.
- Apply strap-pin caution policy:
  - GPIO0: input pull-up only
  - GPIO2 (`MDM_PWRKEY`): do not drive during early boot
  - GPIO5: leave inactive input state unless explicitly needed
  - GPIO12 (`SYS_EN`): hold low through early boot

3. I2C init and scan
- Initialize I2C at 100 kHz on SCL GPIO26 / SDA GPIO27.
- Perform scan and log found/missing/unexpected addresses.

4. KTS1622 init on 0x20 and 0x21
- Validate both devices respond.
- Read baseline direction/output/input registers for diagnostics.

5. Immediately apply expander power critical defaults
- `EXTPWR_CTL` (0x21 P0_7): set to boot level (`LOW` for PSU enabled) immediately after expander init.
- `CHRG_EN` (0x21 P0_4): keep in safe policy state (low) until charger policy applies.
- Set safe defaults for outputs including `+12V_EN`, `LOAD_CTL`, `BEACON_CTL`, `IO1_CTL`, `IO2_CTL`.
- Read-back and log final applied output states.

6. Prove GPIO_INT interrupt handling
- Configure GPIO39 interrupt on active-low event with pull-up assumptions aligned to hardware.
- Trigger one known expander input transition.
- ISR/task path records exactly one event log for one transition and clears source.
- Confirm no interrupt storm in idle state.

7. BQ25622 safe boot config with read-back verification
- Initialize BQ25622 at 0x6B.
- Apply conservative safe boot registers.
- Read back written registers and fail/log if mismatch.
- Poll status/fault registers (INT not connected).

8. Optional telemetry loop behind `CONFIG_GSM4_PHASE0_TELEMETRY`
- When enabled, run 1 Hz telemetry summary: external voltage, charger status summary, GPIO_INT level, GLOBAL_INT level.

## F) Logging and diagnostics plan

### Log tags

- `BOOT`: startup banner and lifecycle
- `BSP`: boot-step transitions
- `I2C`: bus init/scan and bus-level errors
- `KTS`: expander init/defaults/readback
- `BQ`: charger init/config/readback/status
- `INT`: GPIO_INT/GLOBAL_INT state and interrupt proof
- `SEC`: redaction/security policy events

### Exact expected successful-boot log outputs

- `BOOT`: `phase0 boot start board=gsm4_rev4 uart0=ok`
- `I2C`: `init ok port=0 scl=26 sda=27 hz=100000`
- `I2C`: `scan found=[0x10,0x18,0x20,0x21,0x50,0x52,0x6B] missing=[] unexpected=[]`
- `KTS`: `0x20 init ok`
- `KTS`: `0x21 init ok`
- `KTS`: `safe defaults applied: EXTPWR_CTL=0 CHRG_EN=0 12V_EN=0 LOAD_CTL=0 BEACON_CTL=0 IO1_CTL=0 IO2_CTL=0`
- `KTS`: `readback defaults verified`
- `INT`: `idle levels gpio_int=1 global_int=1 active_low=1`
- `INT`: `gpio_int event count=1 source=<expander-pin> storm=0`
- `BQ`: `init ok addr=0x6B`
- `BQ`: `safe cfg applied and readback=match`
- `BQ`: `status summary: <state fields> fault=<none|code>`
- `BOOT`: `phase0 bringup complete`

### Must never be logged

- Passwords, PIN values, salts, keys, certificates, tokens, raw credential blobs
- Raw NVS blobs and full binary config dumps
- Any future identity material contents
- If diagnostics must prove secret presence, log only a key ID or SHA-256 fingerprint, never the secret itself

## G) Security considerations (Phase 0)

### Phase 0 security scope boundary (must be explicit)

- **Do NOT enable Secure Boot, Flash Encryption, or NVS Encryption in Phase 0.**
- In Phase 0 you MUST only:
  - ensure the codebase and storage patterns are compatible with enabling these later,
  - keep secrets out of logs and diagnostics,
  - avoid hardcoded credentials and debug shells/endpoints enabled by default,
  - document a clear plan for enabling Secure Boot / Flash Encryption / NVS Encryption in a later phase (recommended Phase 5).

### Security-ready implementation stance for Phase 0

- No hardcoded credentials in code or defaults.
- No secrets in source control, examples, or test fixtures; placeholders remain clearly non-functional (for example `EXAMPLE_ONLY_DO_NOT_USE`).
- Diagnostic dumps must redact secret-bearing fields; when presence must be proven, emit only key ID or fingerprint metadata.
- No network bring-up in Phase 0.
- No AT command passthrough shell.
- No debug endpoints enabled by default.
- Telemetry/logging must be compile-time gated (`CONFIG_GSM4_PHASE0_TELEMETRY` and existing profile debug flags).
- Storage calls (if any) must route via wrappers that can later switch to encrypted NVS path without API churn.

### Strapping pin handling plan

- GPIO0 (`BOOT_BUT`): input + pull-up only; no active driving at boot.
- GPIO2 (`MDM_PWRKEY`): avoid driving during early boot to prevent strap side effects.
- GPIO5: leave inactive during Phase 0 bring-up.
- GPIO12 (`SYS_EN`): hold low until safe state complete; only then permit controlled transitions.

### Security docs availability

- `docs/security/security_policy.md` now exists (draft) and is treated as a Phase 0 security constraint reference.
- Security references used for this proposal: `docs/security/security_policy.md`, `docs/architecture/IDF_TARGET_ARCH.md`, `docs/CODEX_RULES.md`, `docs/project/DECISIONS.md`, `docs/setup guides/ESP_IDF_SETUP_GUIDE.md`.

### Forward plan alignment (post-Phase 0)

- Prepare for later enablement of:
  - Secure Boot
  - Flash Encryption
  - NVS Encryption
- Recommended enablement phase per current roadmap context: later hardening phase (targeting Phase 5 in bring-up roadmap execution planning).

## H) Testing plan (unit, mock, bench)

### 1. Unit tests for pure logic (to implement in Phase 0)

- `drv_bq25622`: bitfield pack/unpack helpers round-trip expected register values.
- `drv_kts1622`: expander output safe-default application logic sets correct bit states for `EXTPWR_CTL`, `CHRG_EN`, `12V_EN`, `LOAD_CTL`, `BEACON_CTL`, `IO1_CTL`, `IO2_CTL`.
- `bsp_i2c_scan`: compare expected device-address set against found-address set and produce deterministic missing/unexpected results.
- `bsp_gpio`: strap-pin safe-state policy helper rejects unsafe early-drive requests.
- `bsp_log_sanitize`: log-redaction helper removes secret-bearing fields and permits only key ID/fingerprint proof output.

### 2. Mockable HAL boundaries

- Drivers consume an `i2c_bus` interface rather than direct IDF I2C calls.
- Interface includes at minimum:
  - `read_reg(addr, reg, *val)`
  - `write_reg(addr, reg, val)`
  - `read_multi(addr, reg, *buf, len)`
  - `write_multi(addr, reg, *buf, len)`
- Mock/fake implementation for tests:
  - deterministic register map backing store
  - injectable I2C error conditions
  - call-count/assertion hooks

### 3. ESP-IDF Unity tests

- Add component-local test files under:
  - `firmware/components/drv_kts1622/test/`
  - `firmware/components/drv_bq25622/test/`
  - `firmware/components/bsp_gsm4_rev4/test/` (logic-only helpers)
- Integrate test runner wiring following existing host pattern in:
  - `firmware/tests/host/unit/`
  - `firmware/tests/host/mock/`
- Run with existing scripts:
  - `tools/test_unit.sh`
  - `tools/test_mock.sh`

### 4. Bench validation checklist

1. Flash Phase 0 firmware and open UART0 monitor.
2. Verify boot banner and safe GPIO-default logs.
3. Verify I2C init at 100 kHz on GPIO26/GPIO27 and full scan match.
4. Verify KTS1622 init for 0x20 and 0x21.
5. Verify `EXTPWR_CTL` is logged/applied low immediately after expander init.
6. Verify `CHRG_EN` remains in safe state until charger policy step.
7. Trigger one known expander input change and confirm one GPIO_INT event log (no storming).
8. Verify BQ25622 safe-config read-back matches and status/fault summary is present.
9. Verify idle interrupt levels (`GPIO_INT`, `GLOBAL_INT`) are stable and high (active-low lines).
10. Confirm no watchdog behavior is enabled in Phase 0.
11. Confirm diagnostics do not leak secrets; if identity material presence is logged, output is key ID/fingerprint only.

## I) Acceptance criteria (concrete)

- I2C scan shows all expected addresses: `0x10, 0x18, 0x20, 0x21, 0x50, 0x52, 0x6B`
- `EXTPWR_CTL` is deterministically set to PSU enabled boot state (`LOW`) unless policy states otherwise
- BQ25622 read-back matches configured values
- GPIO_INT interrupt triggers as expected, no storms
- No watchdog in Phase 0
- Logs/diagnostics contain no secret material; where proof is needed, only key ID/fingerprint metadata is emitted
- No network bring-up, AT pass-through shell, or debug endpoints enabled by default in Phase 0

## J) Risks and mitigations

- I2C bus lockups
  - Risk: stuck SDA/SCL or transient NACKs block bring-up
  - Mitigation: bounded retries, bus-clear procedure (SCL pulse recovery), and explicit re-init path before fail-safe exit

- Strap pin risks (`GPIO2`, `GPIO12` especially)
  - Risk: unsafe early drive causes incorrect boot strapping or unstable startup
  - Mitigation: strict early-boot strap policy and deferred nonessential pin driving

- PSU glitch on boot due to EXTPWR_CTL timing
  - Risk: delayed or wrong EXTPWR_CTL level briefly disables PSU
  - Mitigation: make EXTPWR_CTL first critical expander output action and verify via immediate read-back log

- BQ25622 polling-only status handling
  - Risk: delayed fault detection without INT line
  - Mitigation: deterministic poll cadence in Phase 0 and explicit status/fault log summary

- Secret leakage through diagnostics
  - Risk: debug dumps or ad-hoc logs accidentally expose credential material
  - Mitigation: centralized log-redaction helper, fingerprint-only proof policy, and dedicated unit/bench checks

- Documentation mismatches across legacy docs
  - Risk: conflicting signal semantics (example: GLOBAL_INT polarity note)
  - Mitigation: board header precedence rule and explicit conflict note in implementation comments/log plan

## K) Implementation checklist

- Review and approve `docs/bringup/phase0_proposal.md` before code implementation.
- Add `bsp.h` API in existing BSP component.
- Implement BSP safe GPIO defaults helper.
- Implement BSP I2C init + scan helper (100 kHz default).
- Add `drv_kts1622` component with minimal register API.
- Add `drv_bq25622` component with minimal register API.
- Add `i2c_bus` abstraction component and IDF backend.
- Wire deterministic boot sequence in `bsp_init()`.
- Add Kconfig flag `CONFIG_GSM4_PHASE0_TELEMETRY` and telemetry guard.
- Add GPIO_INT proof path with anti-storm guard/logging.
- Add BQ25622 safe boot config + read-back verify path.
- Add component-local Unity tests for KTS/BQ/BSP logic.
- Add unit tests for log-redaction/fingerprint-only diagnostic behavior.
- Add profile/config checks that Phase 0 keeps debug/management surfaces default-off in `ci-secure` and `release`.
- Extend host unit/mock runners to execute new tests.
- Add bench validation notes and capture expected logs in acceptance record.

## L) Implementation starting point (signatures only)

```c
esp_err_t bsp_init(void);
esp_err_t bsp_gpio_safe_defaults(void);
esp_err_t bsp_i2c_init(void);
esp_err_t bsp_i2c_scan(i2c_port_t port, uint8_t *found, size_t found_len);

esp_err_t kts1622_init(...);
esp_err_t kts1622_apply_safe_defaults(...);

esp_err_t bq25622_init(...);
esp_err_t bq25622_apply_safe_boot_config(...);
```
