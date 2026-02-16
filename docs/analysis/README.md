# Analysis Folder Guide

This folder stores architecture and coupling analysis artifacts used to plan the ESP-IDF rewrite.

## Files and Purpose

| File | Purpose |
|---|---|
| `A_calls_cluster_all_cpp.md` | Raw file-to-file call-site map. Shows how many call sites exist between pairs of `.cpp` files and call direction (`A→B`, `B→A`, `bidirectional`). |
| `03_entry_points.md` | Entry-point and runtime context inventory. Documents `setup()`/`loop()`, created tasks, ISR/timer hooks, callbacks, and first-level fan-out from those contexts. |
| `05_bus_access_union_fn_refined.md` | Shared-state access matrix for global/register-style data (for example `configReg`). Captures where fields are read/written and in which execution context (loop, task, HTTP callback, etc.). |
| `07_interaction_scores_v2_fanout.csv` | Weighted interaction score table between source files. Contains component scores (state/context/calls) and top evidence used to rank coupling strength. |
| `08_interaction_clusters_v2_fanout.md` | Clustered view of coupling results. Summarizes strongest internal edges and identifies bridge files with highest weighted degree. |
| `13_dependency_edges.csv` | Module-to-module dependency edge list derived from source evidence. Includes interaction type (`call`, `shared-state`) and a traceable evidence string. |
| `13_system_recomposition.md` | Proposed recomposed module architecture. Defines canonical responsibilities, candidate modules, owned state, boundaries, and cross-module dependencies for rewrite planning. |

## How To Use These Together

1. Start with `03_entry_points.md` to understand runtime contexts.
2. Use `05_bus_access_union_fn_refined.md` and `A_calls_cluster_all_cpp.md` to identify high-risk coupling.
3. Review `07_interaction_scores_v2_fanout.csv` and `08_interaction_clusters_v2_fanout.md` to prioritize refactor order.
4. Use `13_system_recomposition.md` and `13_dependency_edges.csv` as the target module contract for implementation planning.
