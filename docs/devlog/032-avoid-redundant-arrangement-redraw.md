# 032 — Avoid redundant arrangement redraws

The Arrangement screen was being refreshed at every sixteen-step boundary even
when `Current` and `Next` were the same slot. The playhead still wraps at that
boundary, but the visible arrangement values do not change.

`PatternEditorState::advanceByElapsedSteps()` now reports whether the boundary
changed the current pattern, rather than merely whether the playhead crossed a
boundary. Pattern and Arrangement views redraw only when their visible pattern
identity changes; ordinary cycle wraps remain on the incremental playhead path.

Verification: `just build-sequencer` succeeds. On hardware, leave the C screen
open with `Current == Next` and confirm that no periodic full value redraw is
visible.
