# IDF Target Architecture
## Goals
- Deliver a full ESP-IDF rebuild that preserves Phase 1 behavior parity defined by module v1 contracts and `docs/tests/ACCEPTANCE_INVARIANTS.md`.
- Replace Arduino `setup()/loop()` orchestration with explicit IDF tasks, queues, event groups, and startup lifecycle while preserving externally observable behavior.
- Preserve existing route surface, config persistence compatibility, timing gates, and cross-module interactions during core bring-up.
- Establish architectural hooks in Phase 1 for Phase 2+ security, OTA dual image, cloud comms, and field debug channels.

## Non goals
- No in-place refactor of current code paths.
- No behavior redesign beyond contract-preserving rebuild requirements.
- No Phase 1 dependency on features explicitly assigned to Phase 2+ (secure boot enablement, flash encryption enablement, OTA rollout logic, SNMPv3 rollout, Bluetooth/web terminal rollout).
- No schema redesign of persisted config in Phase 1 (compatibility-first persistence retained).

## Phased delivery plan
### Phase 1: Core functional parity
- Objectives:
  - Rebuild module behavior with IDF task model and IPC while meeting acceptance gates in `docs/tests/ACCEPTANCE_INVARIANTS.md`.
  - Preserve route behavior (`/values.xml`, `/setup.xml`, `/config.xml`, `/permissions.xml`, `/config.json`, `/login`, `/logout`, `/upload`, `/config`, `/config_upload`) from WiFi/Web contract.
  - Preserve time gates and key flows: modem polling, audio test, DTMF timeout, power/IO periodic checks.
- Entry criteria:
  - v1 contracts baseline fixed: `docs/modules/*.md` (Timebase, Serial Transport, Modem Core, WiFi/Web, Config Store, Audio Runtime, Power/IO, Sensor/UI, In-call DTMF).
  - dependency and module mapping available (`docs/analysis/13_dependency_edges.csv`, `docs/analysis/13_system_recomposition.md`).
- Exit criteria:
  - All Phase 1 acceptance checks in `docs/tests/ACCEPTANCE_INVARIANTS.md` pass.
  - Boot/lifecycle order and route surface parity verified.
  - Config compatibility (`/config.ini` ordering + markers) verified.
- Architectural hooks needed from Phase 1:
  - security interface stubs for key/cert retrieval and secret redaction.
  - partition layout with reserved regions for future secure assets and OTA slots.
  - centralized log ring buffer and state snapshot service.

### Phase 2: Security hardening
- Objectives:
  - Add secure boot and flash encryption.
  - Add X.509 identity model and key/cert storage abstraction.
  - Enable encrypted transport baseline and TLS capability scaffolding (TLS 1.3 where target/stack supports).
- Entry criteria:
  - Phase 1 parity complete and stable.
  - log and storage abstractions from Phase 1 present.
- Exit criteria:
  - Secure boot and flash encryption enabled in build/deployment profile.
  - Device identity and trust anchor loading through abstraction layer.
  - No secrets logged (validated by log policy checks).
- Architectural hooks needed from Phase 1:
  - key material abstraction API.
  - reserved secure-storage partition hooks.
  - endpoint/session integration points in `web_task` for transport security.

### Phase 3: OTA dual image
- Objectives:
  - Introduce dual-image OTA manager and rollback-capable update lifecycle.
- Entry criteria:
  - Phase 1 parity and Phase 2 security baseline stable.
  - partitioning hooks from Phase 1 available.
- Exit criteria:
  - OTA state machine implemented: download, verify, switch, health check, rollback.
  - power-loss resilience and rollback behavior validated.
- Architectural hooks needed from Phase 1:
  - reserved OTA partitions and boot metadata path.
  - non-blocking command/event path from web/control plane to OTA manager.

### Phase 4: Cloud comms and management
- Objectives:
  - Add cloud comms management interfaces with TLS capability.
  - Add SNMPv3 capability for future management/telemetry integration.
- Entry criteria:
  - Phase 3 OTA baseline complete.
  - security services and credential abstraction available.
- Exit criteria:
  - cloud comms path operational with authenticated transport.
  - SNMPv3 capability integrated behind feature flag and policy controls.
- Architectural hooks needed from Phase 1:
  - modular comms adapter interfaces and command routing.
  - normalized telemetry snapshot publisher.

### Phase 5: Field debug UX
- Objectives:
  - Add Bluetooth serial debug channel (development/service).
  - Add authenticated web serial terminal page for log capture/export and optional upstream streaming.
- Entry criteria:
  - Phase 1 observability plumbing and auth/session model stable.
- Exit criteria:
  - feature-flagged, role-gated debug UX available without impacting core task timing.
  - rate limiting and secret redaction enforced in debug outputs.
- Architectural hooks needed from Phase 1:
  - centralized log ring buffer.
  - role-aware web auth/session hooks.
  - non-blocking log fan-out pipeline.

## Task model
### boot_coordinator (app_main)
- Task name: `boot_coordinator` (`app_main` responsibilities).
- Priority: high during startup, then supervisory/low-run-time.
- Stack sizing approach: start with medium stack estimate; validate via high-watermark telemetry and panic backtraces under full startup + route + modem/audio load.
- Inputs (queues/events): boot events, health events from subsystem tasks.
- Outputs (queues/events): init commands to `config_task`, `io_task`, `audio_task`, `modem_task`, `ui_task`, `web_task`; lifecycle event bits.
- Owned state: startup/lifecycle phase state; task-handle registry; boot readiness bits.
- Contract references: `docs/refactor_analysis/agents/03_entry_points.md`, `docs/modules/modem_core.md`, `docs/modules/audio_runtime.md`, `docs/modules/wifi_web_control_plane.md`, `docs/modules/power_and_physical_io.md`, `docs/modules/sensor_and_visual_ui.md`.

### modem_task
- Task name: `modem_task`.
- Priority: high (timing-sensitive modem polling + decode).
- Stack sizing approach: start high due to parser + transport + JSON/sms/gps paths; validate with peak decode traffic and stack watermark checks.
- Inputs (queues/events): modem command queue; event bits for reboot/config/audio-test triggers; transport RX callback events.
- Outputs (queues/events): telemetry/state updates to snapshot service; events to `audio_task`, `web_task`, `io_task`.
- Owned state: modem runtime state machine and modem-local buffers/parsers.
- Contract references: `docs/modules/modem_core.md`, `docs/modules/serial_transport.md`, `docs/analysis/13_dependency_edges.csv`.

### audio_task
- Task name: `audio_task`.
- Priority: high (continuous audio processing + test timing windows).
- Stack sizing approach: start high for audio pipeline + file/decode control paths; validate under playback + DTMF + audio-test concurrency.
- Inputs (queues/events): audio command queue (playback, stop, volume, dtmf); event bits for audio test handshake and tone timing.
- Outputs (queues/events): status events/results to `modem_task` and snapshot service.
- Owned state: audio runtime objects/state machine; DTMF/audio-test runtime state.
- Contract references: `docs/modules/audio_runtime.md`, `docs/modules/modem_core.md`, `docs/modules/timebase.md`.

### io_task (power + adc + gpio + buttons + in-call DTMF)
- Task name: `io_task`.
- Priority: medium-high (hardware scan/control deadlines).
- Stack sizing approach: medium stack with ISR-safe interfaces; validate under rapid input activity and output updates.
- Inputs (queues/events): IO command queue (gpio/power/setpoints), DTMF command events, periodic tick events.
- Outputs (queues/events): command/event outputs to `modem_task`, `config_task`, `web_task`; snapshot updates.
- Owned state: power/adc/button/gpio runtime state; in-call DTMF decoder runtime state (logical module retained, task ownership moved from loop to io task).
- Contract references: `docs/modules/power_and_physical_io.md`, `docs/modules/in_call_dtmf_control.md`, `docs/modules/timebase.md`.

### ui_task
- Task name: `ui_task`.
- Priority: medium.
- Stack sizing approach: medium stack; validate with TFT init/logo/message paths + LED and IMU periodic paths.
- Inputs (queues/events): UI update events, periodic tick events, state snapshot reads.
- Outputs (queues/events): UI hardware side effects; snapshot field writes for UI-owned outputs.
- Owned state: IMU/LED/TFT runtime objects and timers.
- Contract references: `docs/modules/sensor_and_visual_ui.md`, `docs/modules/timebase.md`, `docs/analysis/13_dependency_edges.csv`.
- Decision: choose `ui_task` (not loop-polled runner) because IDF rebuild uses task model and UI contract already defines independent periodic gates (IMU 1s, LED 10ms) and event-driven TFT actions.

### web_task
- Task name: `web_task` (`esp_http_server`).
- Priority: medium.
- Stack sizing approach: medium-high for route handlers, auth/session, upload handlers; validate with concurrent request and upload bursts.
- Inputs (queues/events): HTTP request callbacks, WiFi/network events, snapshot reads.
- Outputs (queues/events): command queue writes to `config_task`, `io_task`, `modem_task`; event bits for update/control; HTTP responses.
- Owned state: web auth/session/client-tracking/upload state.
- Contract references: `docs/modules/wifi_web_control_plane.md`, `docs/analysis/13_dependency_edges.csv`.

### config_task
- Task name: `config_task`.
- Priority: medium-low (serialized persistence owner).
- Stack sizing approach: medium stack; validate under upload persistence, periodic writes, and migration/recovery scenarios.
- Inputs (queues/events): config command queue (`load`, `write`, `default+write`, `migrate`), save-request event bits.
- Outputs (queues/events): config-available and config-updated events; snapshot updates for config metadata.
- Owned state: persistence engine for `/config.ini` compatibility format and in-memory canonical config object.
- Contract references: `docs/modules/config_store.md`, `docs/modules/wifi_web_control_plane.md`, `docs/modules/power_and_physical_io.md`, `docs/modules/modem_core.md`.
- Decision: choose dedicated `config_task` (not io-owned config functions) because v1 contracts show writes from loop/http/modem/button contexts; Phase 1 serialization preserves behavior safely.

### timebase service (logical component, not task)
- Component name: `timebase_service`.
- Priority: ISR + lightweight API.
- Stack sizing approach: ISR-only writer plus lock-safe reader API; validate with contention and wake-jump tests.
- Inputs/outputs: hardware timer ISR tick events and optional wake-jump command from coordinator.
- Owned state: monotonic tick counter and API (`TickGet` equivalent).
- Contract references: `docs/modules/timebase.md`, `docs/tests/ACCEPTANCE_INVARIANTS.md`.

### security_services (future logical component)
- Component name: `security_services` (Phase 2+).
- Task decision: logical service in Phase 1; taskization deferred unless runtime load requires dedicated task.
- Owned scope: key/cert access abstraction, crypto policy metadata, secret redaction rules.
- Contract references: `docs/tests/ACCEPTANCE_INVARIANTS.md` (global gates), this architecture phase plan.

### ota_manager (future logical component)
- Component name: `ota_manager` (Phase 3+).
- Task decision: logical component in Phase 1; dedicated OTA task introduced in Phase 3 dual-image rollout.
- Owned scope: OTA state machine and image lifecycle metadata.
- Contract references: `docs/modules/wifi_web_control_plane.md` (current update trigger behavior), this architecture phase plan.

## Ownership model
### State categories
- Telemetry (read-mostly): status snapshots exposed to web/UI/logging.
- Commands (write-once, consume-and-clear): transient requests/events (example: reboot requests, audio-test triggers, gpio update requests).
- Configuration (persistent): `configReg`-equivalent persisted settings.

### Single-writer targets
| Category | Phase 2 single writer target | Phase 1 compatibility writer strategy |
|---|---|---|
| Telemetry | Producer task per domain (`modem_task`, `audio_task`, `io_task`, `ui_task`) writing only owned telemetry slice | Retain multi-writer legacy field behavior through `bus_api` with internal serialization and write-audit logging |
| Commands | Origin task writes command message; target task is single consumer/clearer | Allow legacy-style direct flag requests only through `bus_api` wrapper with strict origin tagging and consume/clear logs |
| Configuration | `config_task` | All module-originated config mutations converted to `config_task` commands in Phase 1 |

### Phase 1 compatibility strategy for current multi-writer fields
- Provide a central `bus_api` with:
  - serialized write access (mutex/critical section internal to API layer).
  - required origin metadata on each write.
  - structured logs for write conflicts and overwrite order.
- Preserve legacy semantics in Phase 1 while collecting evidence for Phase 2 migration to stricter message-passing single-writer rules.

## Inter-task communication
### IPC primitives
- Command queues (typed messages): primary control-plane IPC.
- Event group bits (fast flags): low-latency state signals and wakeups.
- State snapshot struct: lock-bounded read model for web/status/UI/log exporters.

### IPC mapping table
| From task | To task | Mechanism (queue/event) | Message/Event name | Trigger | Contract evidence |
|---|---|---|---|---|---|
| `boot_coordinator` | `config_task` | queue | `CFG_LOAD_BOOT` | boot config load stage | `docs/modules/config_store.md`, `docs/refactor_analysis/agents/03_entry_points.md` |
| `boot_coordinator` | `modem_task` | queue | `MODEM_INIT_START` | task startup order | `docs/modules/modem_core.md`, `docs/refactor_analysis/agents/03_entry_points.md` |
| `boot_coordinator` | `audio_task` | queue | `AUDIO_INIT_START` | task startup order | `docs/modules/audio_runtime.md`, `docs/refactor_analysis/agents/03_entry_points.md` |
| `boot_coordinator` | `io_task` | queue | `IO_INIT_START` | task startup order | `docs/modules/power_and_physical_io.md`, `docs/refactor_analysis/agents/03_entry_points.md` |
| `boot_coordinator` | `ui_task` | queue | `UI_INIT_START` | task startup order | `docs/modules/sensor_and_visual_ui.md`, `docs/refactor_analysis/agents/03_entry_points.md` |
| `boot_coordinator` | `web_task` | event | `EVT_WEB_ENABLE` | config/task readiness complete | `docs/modules/wifi_web_control_plane.md`, `docs/refactor_analysis/agents/03_entry_points.md` |
| `web_task` | `config_task` | queue | `CFG_WRITE_REQ` | `/config_upload` final chunk, `/config` update path | `docs/modules/wifi_web_control_plane.md`, `docs/modules/config_store.md` |
| `web_task` | `config_task` | queue | `CFG_DEFAULT_REQ` | config reset command route action | `docs/modules/wifi_web_control_plane.md`, `docs/modules/config_store.md` |
| `io_task` | `config_task` | queue | `CFG_DEFAULT_AND_WRITE_REQ` | button hold reset path | `docs/modules/power_and_physical_io.md`, `docs/modules/config_store.md` |
| `web_task` | `io_task` | queue | `IO_SET_CHARGERATE` | setup_advan command action | `docs/modules/wifi_web_control_plane.md`, `docs/modules/power_and_physical_io.md` |
| `web_task` | `io_task` | queue | `IO_GPIO_UPDATE_REQ` | gpio_on/gpio_off/esim_apply_reset actions | `docs/modules/wifi_web_control_plane.md`, `docs/modules/power_and_physical_io.md` |
| `web_task` | `modem_task` | queue | `MODEM_SIM_SWITCH_GUARD_REQ` | esim apply/reset command route | `docs/modules/wifi_web_control_plane.md`, `docs/modules/modem_core.md` |
| `audio_task` | `modem_task` | event | `EVT_AUDIO_TEST_TONE_START_REQ` | audio test handshake request | `docs/modules/audio_runtime.md`, `docs/modules/modem_core.md` |
| `modem_task` | `audio_task` | event | `EVT_MODEM_AUDIO_TONE_ACTIVE` | audio test tone active + tick updates | `docs/modules/modem_core.md`, `docs/modules/audio_runtime.md` |
| `modem_task` | `io_task` | queue | `DTMF_DIGIT_RX` | modem decode produces DTMF digit/response | `docs/modules/modem_core.md`, `docs/modules/in_call_dtmf_control.md` |
| `modem_task` | `web_task` | event | `EVT_MODEM_STATUS_UPDATED` | modem telemetry change | `docs/modules/modem_core.md`, `docs/modules/wifi_web_control_plane.md` |
| `timebase_service` | all periodic tasks | event | `EVT_TICK_WINDOW` | periodic gate scheduling | `docs/modules/timebase.md`, module contracts using TickGet |

## Web architecture
- Runtime stack: `esp_http_server` in `web_task`.
- Static file serving plan (Phase 1): serve web assets from SPIFFS (compatibility with existing filesystem usage and config storage paths).
- Endpoint parity plan:
  - Preserve existing external routes and response contracts from v1 WiFi/Web contract.
  - Map mutating routes to command queues/events (`CFG_WRITE_REQ`, `IO_SET_CHARGERATE`, `MODEM_SIM_SWITCH_GUARD_REQ`, etc).
  - Keep `/values.xml` and config/value/status payload semantics compatible with acceptance gates.
- Auth/session handling placement:
  - `web_task` owns all session state, cookies, lockout windows, and role checks.
  - Other tasks receive only validated commands, never raw auth/session tokens.
- Phase 5 web serial terminal capability:
  - Implement as restricted endpoint under authenticated session + role gating.
  - Source logs from controlled ring buffer only; no direct task `stdout` piping.
  - Support display, export to client, and optional secure upstream streaming via feature flags.

## Security and device identity
### Phase 1 architectural hooks only
- Reserve partition/layout hooks for future cert/key storage.
- Define key material abstraction interface (no hard-coded file paths in business modules).
- Enforce log policy: never log secrets (passwords, certs, private keys, tokens).
- Keep auth/session data owned by `web_task`; cross-task messages carry only normalized command data.

### Phase 2+ security intentions
- X.509 device identity model:
  - device cert + private key + trust anchors accessed via `security_services` abstraction.
- Encryption in transit:
  - TLS-enabled service endpoints and outbound secure channels; TLS 1.3 capability where IDF/target supports.
- Encryption at rest:
  - protected storage strategy for key/cert material and sensitive persisted secrets.
- Platform hardening:
  - secure boot enablement and flash encryption enablement in deployment profile.

## Persistence architecture
- Strategy: compatibility-first in Phase 1.
- Phase 1 persistence contract:
  - keep `/config.ini` compatibility format and ordering behavior.
  - preserve marker handling (current marker and legacy marker migration behavior).
  - centralize persistence in `config_task` (serialized writes).
- Phase 2 migration path:
  - add versioned schema migration to NVS-backed storage while maintaining read compatibility during migration window.
  - keep explicit migration state and rollback-safe conversion path.
- Contract references:
  - `docs/modules/config_store.md`
  - `docs/tests/ACCEPTANCE_INVARIANTS.md`

## OTA and firmware lifecycle
- Phase 1:
  - no production OTA rollout logic.
  - reserve partition hooks and command/event interfaces for OTA manager integration.
- Phase 3 target architecture:
  - dual image partition strategy with boot metadata.
  - OTA manager state machine: `download -> verify -> switch -> health-check -> confirm/rollback`.
  - command ingress from `web_task` and future cloud channel via queue to `ota_manager`.
- OTA acceptance gates (Phase 3):
  - power-loss resilience during download/apply.
  - deterministic rollback on failed health check.
  - active-slot and previous-slot state visibility in diagnostics.

## Observability and debug channels
### Phase 1
- Logging tags baseline: `BOOT`, `TMB`, `SRT`, `MOD`, `WEB`, `CFG`, `AUD`, `PWR`, `UI`, `DTMF`, `IPC`, `SEC`.
- Log levels: error/warn/info baseline, debug by feature flags.
- Central ring buffer for recent logs, non-blocking writes only.
- State snapshot service:
  - bounded struct updated by producer tasks.
  - consumed by web/status endpoints and diagnostics.

### Phase 5 intentions
- Bluetooth serial debug channel for development/service.
- Web serial terminal page:
  - display ring-buffer logs.
  - export logs to client machine.
  - optional secure streaming to base when enabled.
- Constraints:
  - must not block core tasks.
  - must be rate limited.
  - must be authenticated and role/feature-flag gated.
  - must redact or exclude secrets.

## Timing architecture
- Single time API surface:
  - `timebase_service` provides `TickGet`-equivalent monotonic millisecond API and wake-jump compensation entry.
- Timing ownership:
  - ISR writer for base tick.
  - coordinator-triggered wake compensation for sleep delta.
- Periodic gate mapping from contracts:
  - `modem_task`: continuous poll cadence with modem timing gates.
  - `audio_task`: continuous cadence; DTMF ~200 ms gate; audio-test windows.
  - `io_task`: power ~5 s gate; gpio/button ~10 ms gates; ADC on request.
  - `ui_task`: IMU ~1 s gate; LED ~10 ms gate; TFT event-driven calls.
  - `web_task`: event-driven request handling + periodic client timeout maintenance gate.
  - `config_task`: deferred write gate compatible with `CONFIG_CHECK_PERIOD` semantics.

## Boot and lifecycle
1. Initialize runtime and logging core in `app_main` (`boot_coordinator`).
   - Evidence: migration from `setup()/loop()` orchestration inventory (`docs/refactor_analysis/agents/03_entry_points.md`).
2. Initialize NVS.
   - Required for IDF runtime settings and future secure/OTA metadata.
3. Mount filesystem (SPIFFS in Phase 1 compatibility mode).
   - Evidence: config and asset file use in `docs/modules/config_store.md` and `docs/modules/sensor_and_visual_ui.md`.
4. Initialize timebase service and timer ISR registration.
   - Evidence: `docs/modules/timebase.md`.
5. Start `config_task` and execute config load (`config_init/read/default+write` compatibility behavior).
   - Evidence: `docs/modules/config_store.md`, `docs/analysis/13_dependency_edges.csv`.
6. Start domain tasks in order: `io_task` -> `audio_task` -> `modem_task` -> `ui_task`.
   - Evidence: startup ownership and dependencies from module contracts and `docs/analysis/13_system_recomposition.md`.
7. Start `web_task` (`esp_http_server`) only after config-ready and core task-ready events.
   - Evidence: web uses config/session/status fields (`docs/modules/wifi_web_control_plane.md`).
8. Enter steady-state lifecycle supervision in `boot_coordinator`.
   - Monitor health events, snapshot freshness, and watchdog-safe liveness.

## Compatibility notes
- Preserve current route behavior and payload shape for legacy endpoints while moving implementation to `esp_http_server`.
- Preserve `/config.ini` compatibility format, marker behavior, and startup recovery semantics in Phase 1.
- Preserve command/flag semantics used in DTMF load activation/deactivation and timeout behavior (`DTMF_TIMEOUT_PERIOD` flow).
- Preserve audio test handshake and result flags between modem and audio modules.
- Preserve timing gate semantics (TickGet-based) for power, gpio/button, IMU/LED, modem/sms/gps, and DTMF timeout.
- Preserve status/config/general flag semantics consumed by UI and web status paths.
- Web stack implementation changes are allowed (ESPAsyncWebServer -> `esp_http_server`), but externally observable route behavior must remain compatible in Phase 1.

## Implementation checklist
### Phase 1: Core functional parity
- [ ] Create `boot_coordinator` (`app_main`) skeleton and lifecycle state machine (refs: `docs/refactor_analysis/agents/03_entry_points.md`, `docs/tests/ACCEPTANCE_INVARIANTS.md`).
- [ ] Implement `timebase_service` API with ISR tick writer and wake-jump hook (ref: `docs/modules/timebase.md`).
- [ ] Add IPC foundations: command queues, event groups, snapshot struct (ref: this architecture IPC section).
- [ ] Implement `config_task` with serialized `load/write/default+write` commands (ref: `docs/modules/config_store.md`).
- [ ] Preserve `/config.ini` compatibility parser/writer ordering and marker handling (ref: `docs/modules/config_store.md`).
- [ ] Implement `io_task` periodic gates: power (5s), gpio/button (10ms), ADC request handling (ref: `docs/modules/power_and_physical_io.md`).
- [ ] Implement `io_task` DTMF decoder flow preserving trigger/timeout/output behavior (ref: `docs/modules/in_call_dtmf_control.md`).
- [ ] Implement `audio_task` loop and command handling for playback/stop/volume/DTMF/test flow (ref: `docs/modules/audio_runtime.md`).
- [ ] Implement modem transport adapter and `modem_task` poll/decode chain (refs: `docs/modules/serial_transport.md`, `docs/modules/modem_core.md`).
- [ ] Implement `ui_task` periodic IMU/LED gates and event-driven TFT operations (ref: `docs/modules/sensor_and_visual_ui.md`).
- [ ] Implement `web_task` with `esp_http_server` and route parity for all documented endpoints (ref: `docs/modules/wifi_web_control_plane.md`).
- [ ] Route web mutating actions through queues/events to owning tasks (refs: `docs/analysis/13_dependency_edges.csv`, this IPC table).
- [ ] Implement session/auth ownership in `web_task` only (ref: `docs/modules/wifi_web_control_plane.md`).
- [ ] Add ring-buffer logging and state snapshot service for acceptance observability (ref: `docs/tests/ACCEPTANCE_INVARIANTS.md`).
- [ ] Add phase-1 write-audit logging in `bus_api` for compatibility multi-writer fields (refs: module contracts with multi-context writes).
- [ ] Verify boot order: NVS init, FS mount, config load, task startup, web start (ref: this lifecycle section).
- [ ] Execute acceptance checks for Timebase/Serial/Modem/Web/Config modules (ref: `docs/tests/ACCEPTANCE_INVARIANTS.md`).
- [ ] Execute acceptance checks for Audio/Power/UI/DTMF modules (ref: `docs/tests/ACCEPTANCE_INVARIANTS.md`).
- [ ] Add partition hooks for future secure storage and OTA slots (ref: this phase plan).

### Phase 2: Security hardening
- [ ] Implement `security_services` abstraction for cert/key retrieval (ref: this security section).
- [ ] Enforce no-secret logging policy in logger pipeline (ref: this security and observability sections).
- [ ] Enable secure boot in deployment profile (ref: this phase plan).
- [ ] Enable flash encryption in deployment profile (ref: this phase plan).
- [ ] Integrate TLS capability scaffolding with TLS 1.3 where supported (ref: this phase plan).
- [ ] Add encrypted-at-rest storage path for security material (ref: this security section).

### Phase 3: OTA dual image
- [ ] Add `ota_manager` component and command ingress from `web_task` (ref: this OTA section).
- [ ] Implement dual-image state machine: download/verify/switch/health/rollback (ref: this OTA section).
- [ ] Implement power-loss resilience checks for OTA transitions (ref: this OTA acceptance gates).
- [ ] Implement rollback decision and confirmation policy with diagnostics (ref: this OTA section).

### Phase 4: Cloud comms and management
- [ ] Add cloud comms adapter interface on top of existing task/IPC model (ref: this phase plan).
- [ ] Add SNMPv3 capability behind feature flag and role/policy controls (ref: this phase plan).
- [ ] Integrate secure channel usage with security_services identity material (ref: this security section).

### Phase 5: Field debug UX
- [ ] Add Bluetooth serial debug channel behind feature flag and service policy (ref: this observability section).
- [ ] Implement authenticated web serial terminal page backed by log ring buffer (ref: this web + observability sections).
- [ ] Add log export path to client with rate limiting and secret redaction (ref: this observability section).
- [ ] Add optional secure upstream log streaming path to base (feature-flagged) (ref: this observability section).
- [ ] Verify debug channels are non-blocking for core tasks under stress (ref: this observability constraints).
