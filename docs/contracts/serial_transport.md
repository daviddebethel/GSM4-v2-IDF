# Serial Transport
Contract level: v1 (code-verified)

## Purpose and non goals
- Purpose: Thingstream transport adapter that binds modem UART I/O to the `ThingstreamTransport` interface (`serial_transport_create`, `serial_run`, `serial_send`, callback registration).
- Non goals: higher-level modem protocol parsing/state handling (handled outside this module).

## Owner execution context
- Proven owner for run path: `task: modem`.
  - `modem_thread()` loops with `modem_task()` (`src/main.cpp:2986-2994`).
  - `modem_task()` calls `modem_run()` (`src/src/modem_handler.cpp:799-803`).
  - `modem_run()` executes `modem->run(modem, 1)` (`src/src/modem_handler.cpp:4434-4438`), which is the call path into transport `run`.
- Registration path (`serial_register_callback`) context: `unknown`.
  - No repository call sites for `ThingstreamTransport::register_callback` found; likely invoked within vendor Thingstream implementation not present. Search evidence: `rg -n "register_callback\\s*\\(" src src/sdk`, `rg -n "->register_callback\\s*\\(" src src/sdk`, `rg -n "\\.register_callback\\b" src src/sdk` returned only `serial_register_callback` definition lines in `src/src/serial_transport.cpp`.

## Entry points
- Public constructor:
  - `ThingstreamTransport* serial_transport_create(Stream* serial, bool lineBased, Print* debugOut, enum debugLevel_e debugLevel)` (`src/src/serial_transport.cpp:41-51`)
  - Called from `modem_init()` (`src/src/modem_handler.cpp:4242-4249`).
- Transport vtable methods (static; externally reached via `ThingstreamTransport` function pointers):
  - `serial_init` (`src/src/serial_transport.cpp:53`)
  - `serial_send` (`src/src/serial_transport.cpp:87`)
  - `serial_register_callback` (`src/src/serial_transport.cpp:101`)
  - `serial_run` (`src/src/serial_transport.cpp:136`)
  - Vtable wiring in `_transport_instance` (`src/src/serial_transport.cpp:29-39`).
- External usage evidence:
  - `modem->run(modem, 1)` callsite in `modem_run()` (`src/src/modem_handler.cpp:4438`).
  - File-level call coupling includes `src/src/modem_handler.cpp -> src/src/serial_transport.cpp` (1 call site) (`docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md:36`).

## Inputs
- UART stream input via `state->serial->read()` in `serial_run()` (`src/src/serial_transport.cpp:159`).
- Transport stack invocations via Thingstream vtable (`run`, `send`, `register_callback`, `init`) through `_transport_instance` (`src/src/serial_transport.cpp:29-39`).
- Wiring input from modem init chain:
  - `serial_transport_create(...)` -> `Thingstream_createRingBufferTransport(...)` -> `Thingstream_createModem2Transport(...)` (`src/src/modem_handler.cpp:4248-4257`).

## Outputs and side effects
- UART TX via `state->serial->write(data, len)` in `serial_send()` (`src/src/serial_transport.cpp:97`).
- UART RX polling via `state->serial->read()` in `serial_run()` (`src/src/serial_transport.cpp:159`).
- Callback dispatch on received bytes via `state->callback(state->callback_cookie, state->buffer, count)` (`src/src/serial_transport.cpp:224-227`).
- Debug side effects to `debugOut` in init/send/run paths (`src/src/serial_transport.cpp:60-63`, `src/src/serial_transport.cpp:92-96`, `src/src/serial_transport.cpp:174-179`).

## Owned state
- Static singleton state: `static SerialTransportState _transport_state` (`src/src/serial_transport.cpp:19`).
- `SerialTransportState` fields (`src/src/serial_transport.cpp:7-17`):
  - `uint8_t buffer[THINGSTREAM_USSD_BUFFER_LEN]`
  - `ThingstreamTransportCallback_t callback`
  - `void* callback_cookie`
  - `Stream* serial`
  - `Print* debugOut`
  - `enum debugLevel_e debugLevel`
  - `bool lineBased`
  - `bool overflowed`
  - `uint32_t timeStamp`

## Timing and triggering
- Trigger mechanism: modem task poll loop.
  - `modem_thread()` runs continuously with `vTaskDelay(10)` (`src/main.cpp:2989-2995`).
  - `modem_task()` calls `modem_run()` every cycle (`src/src/modem_handler.cpp:799-803`).
  - `modem_run()` calls `modem->run(modem, 1)` (`src/src/modem_handler.cpp:4438`).
- `serial_run(self, millis)` uses timeout-based polling (`limit = now + millis`, then 10ms continuation window per byte) (`src/src/serial_transport.cpp:141-183`).

## Concurrency and ordering rules
- Proven for run path:
  - `serial_run` entry is driven by modem task call chain (`modem_thread -> modem_task -> modem_run -> modem->run`), i.e. `task: modem`.
- Synchronization evidence:
  - No `xSemaphore`, mutex, `portENTER_CRITICAL`, `portEXIT_CRITICAL`, `atomic`, or `__atomic` usage appears in `src/src/serial_transport.cpp` or `src/src/modem_handler.cpp` for transport callback assignment/invocation.
- Callback pointer ordering:
  - `serial_register_callback` writes `state->callback` and `state->callback_cookie` (`src/src/serial_transport.cpp:103-105`).
  - `serial_run` reads and invokes `state->callback` (`src/src/serial_transport.cpp:224-227`).
  - Whether callback registration can overlap runtime polling is UNCERTAIN because no repository invocation of `transport->register_callback` is visible; caller location/execution context cannot be derived from available source files.

## Dependencies (from 13_dependency_edges.csv)
Outgoing edges
| to_module | interaction_type | evidence |
|---|---|---|
| Modem Core | event | `serial_run()` dispatches `state->callback(...)` (`src/src/serial_transport.cpp:224-227`); modem callback bridge is `Thingstream_Application_modemCallback(...) -> modem_callback(...)` (`src/src/modem_handler.cpp:4263-4266`). |

Incoming edges
| from_module | interaction_type | evidence |
|---|---|---|
| Modem Core | call | Modem init constructs transport chain via `serial_transport_create(&hwSerial, ...)` (`src/src/modem_handler.cpp:4248`) and modem run loop executes transport `run` via `modem->run(modem, 1)` (`src/src/modem_handler.cpp:4438`). |

## Uncertainties to verify
- Exact runtime callsite(s) invoking `ThingstreamTransport::register_callback` are UNCERTAIN: repository-wide searches under `src/` and `src/sdk/` show no call expressions (`->register_callback(...)` or `.register_callback`) and only `serial_register_callback` definitions in `src/src/serial_transport.cpp`.
- Registration timing relative to `modem->run(modem, 1)` is UNCERTAIN because caller implementation for `register_callback` is not present in repository source.

## Evidence sources
- `src/src/serial_transport.cpp` (`_transport_state`, `serial_transport_create`, `serial_register_callback`, `serial_run`, callback dispatch).
- `src/src/modem_handler.cpp` (`modem_init` transport creation chain, `modem_run`, `modem_task` call to `modem_run`).
- `src/main.cpp` (`modem_thread` task loop and `vTaskDelay(10)` cadence).
- `src/inc/serial_transport.h` (public `serial_transport_create` declaration).
- `src/sdk/inc/transport_api.h` (`ThingstreamTransport` vtable surface for `register_callback` and `run`).
- `docs/refactor_analysis/agents/A_calls_cluster_all_cpp.md` (file-level call coupling row: `modem_handler.cpp -> serial_transport.cpp`, 1 call site).
- `docs/refactor_analysis/agents/03_entry_points.md` (task and callback inventory context references).
