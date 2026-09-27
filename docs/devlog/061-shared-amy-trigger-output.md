# 061 — Delegate AMY trigger output

## Goal

Keep the Calculator responsible for pattern-specific sound selection while
removing backend plumbing that is useful to other trigger sequencers.

## Design

`SequencerPlayback` still emits semantic lane and accent information. Its local
router resolves the current pattern's lane to a MIDI note, then
`AmyDrumEventSink` delegates audio wake-up, accent-to-velocity conversion, and
the final note-on call to `AmyTriggerOutput`.

This removes `StepVelocity.h` and the `wake()` method from the Calculator's
event-sink contract. Audio lifetime is now an implementation detail of the AMY
output boundary instead of sequencing policy.

## Validation

```text
pio test -e native
pio run -e drum-step-sequencer
```

The native suite has 15 tests after moving the velocity-curve test to the AMY
package. The firmware build validates the concrete adapter integration.
Hardware playback passed with unchanged triggering and distinct weak, normal,
and strong accents.

The final build resolves the published `fcz2/amy-synth-m5@0.3.2` package, so
the Calculator no longer requires the sibling AMY repository checkout.
