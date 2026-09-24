# 026 — Make missed-step handling explicit

## Goal

Clarify what the drum sequencer does when its main loop polls the clock late
and one or more step deadlines have already passed.

## Decision

The sequencer is a real-time participant, not an event recorder. It advances
the logical playhead to the current musical position and discards audio events
whose deadlines are already in the past. It does not replay missed triggers as
a burst. This lets it rejoin an external performance clock instead of
producing sounds after the other instruments have moved on.

`DeadlineClock::poll()` still reports the number of elapsed physical
intervals, and `PatternEditorState::advanceByElapsedSteps()` applies that
count to the local position. If exactly one interval elapsed, the current step
is emitted normally. If several intervals elapsed, all overdue triggers are
discarded and audio waits for the next absolute deadline; this avoids a short,
off-tempo interval immediately after recovery.

This is intentionally separate from future pattern-chain bookkeeping: a chain
will need to observe pattern-boundary crossings while advancing the logical
position, even when the corresponding audio triggers are skipped.

## Verification

- Build the drum sequencer with `pio run -e drum-step-sequencer`.
- A temporary validation build injected a single 700 ms loop stall after
  approximately eight seconds of playback. On hardware, the missed notes were
  not replayed, and the next click resumed on the absolute deadline without a
  compressed first interval. The test-only stall was removed before closing
  this slice.
