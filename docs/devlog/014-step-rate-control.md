# 014 — Add selectable step rates

## Goal

Make the drum sequencer's previously implicit 1/16 step duration an explicit
musical setting, inspired by the Tempo/Rate controls on the M-VAVE FM1.

## Design

`PatternEditorState` now stores a discrete `StepRate` and converts it to an
integer-microsecond interval together with the BPM. The available values are:

```text
1/1, 1/2, 1/4, 1/4T, 1/8, 1/8T,
1/16, 1/16T, 1/32, 1/32T
```

The default remains `1/16`. While A is held, `6` selects the previous rate and
`-` selects the next rate. The settings controls are grouped in the two right
columns of the calculator layout: `%`/`÷` for volume, `9`/`×` for tempo, and
`6`/`-` for rate. `DeadlineClock` remains unaware of musical rates;
it receives only the resulting interval and preserves phase during changes.

## Verification

Build and flash the drum sequencer. Check that the default still produces a
two-second 16-step pattern at 120 BPM, then cycle rates with A+% and A+9 and
confirm that the playhead speed changes without resetting the pattern.
