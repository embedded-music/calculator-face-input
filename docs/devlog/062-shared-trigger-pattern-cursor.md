# 062 — Extract the trigger pattern cursor

## Goal

Remove repeated playhead, wrapping, and missed-deadline policy from the
Calculator and electronic drummer without coupling trigger patterns to a
particular clock.

## Boundary

`TriggerPatternCursor` lives in `step-trigger`. It owns the current step in
a variable-length cycle, advances from an elapsed-step count, reports completed
cycles, and emits the current step through `TriggerEventSink`.

Time remains outside the package. Calculator may feed it an alternating swing
clock while the electronic drummer uses a straight deadline clock. Pattern
chains also remain Calculator policy: the cursor reports a cycle boundary and
`PatternEditorState` decides how that advances the chain.

Missed deadlines advance the playhead but set `shouldEmit()` false, keeping the
existing no-audio-burst behavior.

## Dependency finding

The firmware build revealed that `amy-synth-m5@0.3.2` pinned
`step-trigger@0.1.0`. The AMY package dependency is now a compatible lower
bound so an application can select the newer trigger package explicitly.

## Validation

```text
pio test -d packages/step-trigger -e native
pio test -e native
pio run -e drum-step-sequencer
```

The package has nine native tests, including variable-length wrapping,
validated sparse-cell loading, and
multi-cycle catch-up. All 15 Calculator tests and its firmware build pass.

During vocabulary review, pattern time coordinates were standardized as
"steps." `TriggerPatternCell { lane, step, level }` and
`TriggerPatternLoader` now provide the shared sparse authoring/import boundary
used by compiled presets and intended for future persistence decoders.

The follow-up composition added `TriggerPatternPlayer`. It owns the deadline
clock and cursor and exposes `begin`, phase-preserving timing changes,
`restart`, and `update`. A source supplies the current pattern and receives
completed-cycle notifications; a sink receives emitted trigger events. This
keeps tempo calculation and pattern-chain policy outside the player while
removing routine clock/cursor plumbing from simple compositions.

Both `begin` and `restart` require an explicit `TriggerStart` policy. Consumers
choose whether step zero is emitted immediately or playback starts silently;
the electronic drummer and metronome both choose immediate emission.

Concrete scheduling is expressed as `StepIntervals`. Straight consumers use
`StepIntervals::constant(stepUs)`; swing-capable consumers can supply
`StepIntervals::alternating(firstUs, secondUs)`. This makes the call site state
what varies without asking the player to interpret BPM or swing percentages.
