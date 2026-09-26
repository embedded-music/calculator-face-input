# Expand native playback coverage

## Goal

Exercise the complete playback path with deterministic dependencies before
changing musical behavior or adding another output backend.

## Tests added

The fake clock and event sink now cover three useful runtime scenarios:

- one on-time step emits one event and wakes the audio backend once;
- multiple active tracks on one step emit multiple events but share one wake;
- a chain boundary reports the pattern transition without replaying stale audio,
  then the first on-time step of the new pattern emits normally.

These tests make the best-effort real-time policy explicit: the logical
playhead catches up, but missed deadlines do not become a burst of delayed
sounds.

## Verification

```text
pio test -e native
just build-sequencer
```

Ten native tests passed and the Core Gray firmware build passed. No hardware
behavior was changed in this slice.
