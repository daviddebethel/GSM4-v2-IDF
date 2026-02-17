# Codex Rules (GSM4 v2 IDF)

Date: 2026-02-17
Status: Active (Phase 1 parity)

This file defines mandatory operating rules for Codex changes in this repository.
The objective is to improve architecture, code flow, security posture, and testability
while preserving externally observable behavior required for Phase 1 parity.

## 1) Scope and priority

Rule precedence (highest to lowest):
1. `docs/tests/ACCEPTANCE_INVARIANTS.md`
2. `docs/contracts/v1/`
3. `docs/interfaces/COMMANDS_EVENTS.md`
4. `docs/architecture/IDF_TARGET_ARCH.md`
5. `docs/project/REWRITE_PROGRAMME.md`
6. This file (`docs/CODEX_RULES.md`)

If rules conflict, preserve externally observable behavior and acceptance evidence first.

## 2) Hard invariants vs migration-allowed changes

Codex MUST preserve:
- HTTP route surface and response contract shapes required by acceptance gates.
- `/config.ini` compatibility semantics and recovery markers until migration is explicitly gated.
- Tick/timebase timeout and periodic gate semantics for acceptance-covered flows.
- Shared-state field semantics where still used as contract boundaries in Phase 1.
- Security baseline: no secrets in logs/responses/debug channels.

Codex MAY change (and is encouraged to change):
- Legacy loop/callback ownership to task/service ownership.
- Internal call-chain shape, module decomposition, and file layout.
- Shared-state wiring to queue/event/snapshot interfaces.
- HAL boundaries and abstractions needed for deterministic tests and future HiL.

Any migration-allowed change MUST include evidence updates (see sections 3 and 8).

## 3) Testing requirements (mandatory)

For any new or changed behavior, Codex MUST:
- Add or update ESP-IDF Unity tests for the affected behavior.
- Add or update mock-based unit tests for logic/state-machine/parsing behavior.
- Keep tests deterministic (no hidden timing or environment dependencies).
- Keep or add hardware abstraction seams so logic can run with mocks.

For any new hardware-facing code, Codex MUST:
- Introduce/use HAL/interface seams rather than direct scattered hardware calls.
- Provide a mock implementation or test double path for non-hardware test execution.
- Preserve future HiL injection points (input simulation, telemetry capture, control hooks).

Temporary test gaps are allowed only with an explicit waiver:
- reason,
- owner,
- due date,
- tracking issue/work item.

## 4) Coding standards, lint, and layout

Codex MUST:
- Keep formatting consistent with repo tooling (`.editorconfig`, pre-commit hooks, and project format/lint tooling as introduced).
- Avoid style-only churn in unrelated files.
- Introduce no new warnings in touched modules.
- Follow consistent naming for tasks, queues, events, interfaces, mocks, and tests.
- Keep module boundaries explicit; avoid hidden cross-module coupling.

Codex SHOULD:
- Keep diffs small and reviewable.
- Use explicit error handling and actionable diagnostics.
- Prefer centralized constants/config over magic numbers.

## 5) Architecture and ownership rules

Codex MUST:
- Align changes to Phase goals in `docs/project/REWRITE_PROGRAMME.md`.
- Respect the task/service ownership model and command/event boundaries.
- Avoid adding new direct cross-module write paths that bypass defined owners.
- Preserve runtime-critical ordering/intent semantics even when refactoring internals.

Codex MUST NOT:
- Reintroduce architecture patterns that reduce testability or ownership clarity.
- Add Phase 2+ dependencies as hidden blockers to Phase 1 parity deliverables.

## 6) Security and safety rules

Codex MUST:
- Treat auth/session checks as mandatory for control paths.
- Avoid logging secrets, credentials, tokens, private keys, or sensitive config.
- Keep debug/diagnostic functionality non-blocking and policy-gated.
- Gate serial debug streams by build profile flags (development/CI-test vs release), with release defaults set to minimal/default-off unless explicitly approved.
- Fail safely: no silent failure paths in control/persistence/security-sensitive code.

## 7) Change execution rules

Before implementation, Codex MUST identify:
- which requirement/phase item the change serves,
- which contracts/invariants are affected,
- which tests will be added/updated.

After implementation, Codex MUST update:
- acceptance evidence/tests (where behavior changed),
- `docs/tests/TRACEABILITY.md` mappings,
- relevant contract docs if behavior contracts were intentionally changed,
- `docs/project/DECISIONS.md` when architectural/policy decisions are introduced.

## 8) Definition of done for Codex-delivered changes

A change is done only when all are true:
- build and required tests pass for the changed scope,
- unit and mock tests exist for new/changed behavior (or approved waiver exists),
- acceptance/traceability documentation is updated,
- no unresolved critical contract ambiguity remains undocumented.

## 9) Stop-and-ask triggers

Codex must pause and request clarification when:
- contract/invariant evidence is ambiguous and could alter observable behavior,
- a requested change conflicts with accepted phase boundaries,
- a change requires destructive actions not explicitly approved,
- required test evidence cannot be produced and no waiver exists.
