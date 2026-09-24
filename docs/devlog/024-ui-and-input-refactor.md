# 024 — UI and input organization refactor

This slice reorganizes the sequencer without changing the intentionally
deferred multi-step playback policy.

## Changes

- `SequencerInput` now owns calculator I2C reads, A/B/C handling, explicit
  `UiMode` transitions, command dispatch, and clock rescheduling.
- `main.cpp` is reduced to hardware setup, playback polling, and orchestration.
- `PatternEditorView` has a shared Settings legend renderer and no longer
  carries obsolete footer-layer branches.
- Calculator keypad labels are shared through `CalculatorKeyLayout.h`.
- Calculator bus address, pins, and interrupt configuration are shared through
  `CalculatorLink.h`.
- Sound metadata contains both full names and compact display names.
- Tempo and volume mutations report whether a clamped value actually changed.
- Sound selection uses an explicit calculator-value-to-sound-index helper instead
  of reusing the step-command index arithmetically.

The question of whether multiple elapsed clock steps should trigger every
intermediate drum event remains intentionally deferred for a later experiment.

## Verification

- `pio run -e drum-step-sequencer`
- Hardware validation still required for A/B modal transitions, calculator
  selection, tempo/rate changes, and sound selection.
