# Consume the shared alternating deadline clock

## Goal

Use the hardware-validated swing implementation to refine the boundary between
musical sequencer policy and reusable physical scheduling.

## Change

The local `SequencerStepClock` still owns tempo-derived straight intervals,
percentage-to-pair conversion, and triplet swing bypass. It now delegates the
resulting long/short deadline schedule to `AlternatingDeadlineClock` from the
sibling `musical-clock` package.

The shared mechanism owns only absolute deadline accumulation, alternating
intervals, late-poll counts, and phase-preserving pair changes. Pattern
position, stale-audio policy, swing vocabulary, and display behavior remain in
this application.

The obsolete constant `DeadlineClockAdapter` was removed. The native test
environment now declares `musical-clock` explicitly, matching the firmware
dependency graph.

## Verification

```text
pio test -e native
just build-sequencer
```

Sixteen native tests passed and the Core Gray firmware build passed. This is an
organization slice: the intended hardware behavior remains identical to the
validated percentage-swing build.
