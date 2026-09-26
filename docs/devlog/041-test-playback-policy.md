# 041 — Test the playback timing policy

## Goal

Make the sequencer's real-time behavior executable and observable in host-side
tests before adding more playback features.

## Changes

`SequencerPlaybackPolicy` is a pure fixed-size decision layer. Given the number
of elapsed deadlines, it advances the editor, reports pattern boundaries and
pattern changes, and marks a current step for audio only when exactly one
deadline arrived on time. Multiple elapsed deadlines are treated as stale and
discarded.

`SequencerPlayback` now uses this policy before invoking the hardware audio
backend. This keeps the timing rule independent from AMY and makes it testable
without a speaker.

## Verification

```sh
pio test -e native
just build-sequencer
```

The native suite now has six passing tests, including on-time, delayed and
pattern-transition playback cases.
