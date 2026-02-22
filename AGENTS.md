# AGENTS.md

## Always load context first
Before doing any work, read these files in this order and use them as source of truth:
1) docs/project/context.md
2) docs/project/decisions.md

If either file is missing or clearly outdated, create or update it before making major changes.

## Decision discipline
- Do not re-litigate prior decisions that are already in docs/project/decisions.md.
- If you believe a previous decision should change, propose an explicit new decision entry with:
  Decision, Why, Alternatives considered, Consequences.

## Context pack discipline
When starting a new task or new thread, ensure docs/project/context.md reflects:
- Project one liner (what you are building and for whom)
- Current objective (what you are trying to achieve right now)
- Repo map (key folders and what lives where)
- Key decisions (with dates)
- Constraints (OS, tooling, security constraints, performance targets)
- Interfaces/contracts (APIs, file formats, schemas, message types)
- Open questions / risks (what is unknown or blocked)
- Next actions (5 to 10 bullet items)
- Glossary (local terms and abbreviations)

## Working style
- Prefer small, reviewable diffs.
- State assumptions and point to where they are recorded (Context Pack or Decision Log).
- For firmware work: explicitly call out risk around timing, interrupts, I2C/SPI contracts, and power states.
- If instructions in docs/decisions.md conflict with other guidance, follow docs/decisions.md and record any updates as a new decision entry.
