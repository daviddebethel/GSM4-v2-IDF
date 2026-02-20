# GSM4 Phase 0 Bench Checklist

Date baseline: 2026-02-20
Scope: deterministic bring-up validation for Phase 0 only.

## Preconditions

- Board: GSM4 DA017 Issue 4 (rev4 mapping).
- Firmware built from this branch and flashed to ESP32.
- UART0 monitor connected.
- No network bring-up, no AT passthrough shell, no debug endpoints enabled.

## Procedure

1. Power-cycle the board and connect UART0 monitor.
2. Confirm boot banner and safe GPIO defaults are logged.
3. Confirm I2C init is 100 kHz on GPIO26/GPIO27.
4. Confirm I2C scan reports all expected devices with no missing/unexpected entries.
5. Confirm KTS1622 probes succeed at `0x20` and `0x21`.
6. Confirm expander safe defaults are applied immediately, including `EXTPWR_CTL=0` and `CHRG_EN=0`.
7. Confirm interrupt idle-level log indicates active-low behavior (`gpio_int=1`, `global_int=1`).
8. Trigger one known expander input transition and confirm a single event log with `storm=0`.
9. Confirm BQ25622 probe/config/readback is successful and status/fault summary is present.
10. If telemetry is enabled (`CONFIG_GSM4_PHASE0_TELEMETRY=y`), confirm 1 Hz telemetry logs appear.
11. Confirm logs do not leak secrets; only `key_id` / `sha256` / fingerprint-style proof fields are allowed.

## Expected Log Signatures

Use these exact signatures for pass/fail checks:

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

## Pass Criteria

- No expected signature missing.
- No unexpected I2C missing devices.
- No interrupt storm behavior.
- No secret-bearing log output.
