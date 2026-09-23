# 012 — Migrate the drum sequencer to `DeadlineClock`

## Goal

Use the shared physical deadline scheduler in the drum sequencer while keeping
the application's 16-step pattern and playhead semantics local to the
sequencer.

## Design

The local `StepClock` is replaced by `DeadlineClock`. The sequencer supplies
monotonic microsecond timestamps and converts its BPM/16th-note interval to
microseconds. Delayed polling returns an event count, which advances the
pattern playhead by that many steps.

Tempo changes use `PreservePhase`, matching the metronome's selected physical
scheduling policy. The shared package does not know about tracks, patterns,
or drum sounds.

## Verification

The drum sequencer firmware should be built and flashed. Hardware validation
should check the 16-step playhead, simultaneous track triggers, and tempo
changes through the calculator controls.

