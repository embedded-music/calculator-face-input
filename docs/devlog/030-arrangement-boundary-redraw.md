# 030 — Refresh arrangement state at pattern boundaries

The queued pattern switch already occurred correctly at the end of the
sixteen-step cycle, but the Arrangement screen remained visually stale while
it was open. The sequencer now redraws only the three value rows when the
boundary changes `Current`; the static arrangement layout is left untouched
and the Pattern screen keeps its incremental playhead redraw path. This avoids
a full-screen flush in the timing path.

Verification: `just build-sequencer` succeeds. On hardware, leave the C screen
open while queuing a different pattern and confirm that `Current` updates
without leaving the screen.
