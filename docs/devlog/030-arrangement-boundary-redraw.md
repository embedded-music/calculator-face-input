# 030 — Refresh arrangement state at pattern boundaries

The queued pattern switch already occurred correctly at the end of the
sixteen-step cycle, but the Arrangement screen remained visually stale while
it was open. The sequencer now redraws that screen when the boundary changes
`Current`; the Pattern screen keeps its incremental playhead redraw path.

Verification: `just build-sequencer` succeeds. On hardware, leave the C screen
open while queuing a different pattern and confirm that `Current` updates
without leaving the screen.
