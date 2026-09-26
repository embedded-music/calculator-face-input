# Test playback with fake clock and note sink

## Goal

Exercise the sequencer playback boundary in native tests without requiring
Arduino, the M5 hardware, or AMY audio output.

## Design

`SequencerPlayback` now consumes two small interfaces: `SequencerClock` and
`SequencerNoteSink`. The firmware adapts the shared `DeadlineClock` through
`DeadlineClockAdapter`, while native tests provide a deterministic fake clock
and a note sink that records wake and note-on calls.

This keeps the real-time policy in the playback class while making its timing
and audio decisions observable. The test also verifies the intended stale-event
policy: one elapsed step triggers the current step, while multiple elapsed
steps advance without emitting a burst of delayed notes.

## Verification

```text
pio test -e native
just build-sequencer
```

Both commands passed. The native suite now has eight tests, including the full
playback test with fake dependencies. The Core Gray firmware build also passed.

## Limits and next steps

The fake sink validates that playback requests notes; it does not validate
AMY's synthesis or the physical speaker. Hardware audio remains covered by the
firmware and manual device checks. The adapter is intentionally local to the
demo until a shared clock consumer boundary is needed elsewhere.
