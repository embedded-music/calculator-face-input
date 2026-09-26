# 039 — Extract sequencer playback

## Goal

Separate real-time playback policy from the application composition in
`main.cpp`, while keeping the current best-effort behavior intact.

## Changes

`SequencerPlayback` now owns deadline polling, logical step advancement,
pattern-boundary reporting, stale-step skipping, and drum triggering. It
returns a fixed-size `SequencerPlaybackUpdate` so the application can update
the display without putting UI work inside the timing/audio policy.

The main loop now composes input, playback, view updates, and audio maintenance
explicitly. The playback object does not allocate dynamically and continues to
discard overdue events rather than emitting a burst after a delayed loop.

## Verification

```sh
pio test -e native
just build-sequencer
```

The native state suite remains green and the Core Gray firmware builds with
the same memory profile range.
