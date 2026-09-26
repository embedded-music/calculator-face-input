# 037 — Extract sequencer settings

## Goal

Keep tempo, rate, and volume policy separate from pattern editing and chain
state while preserving the calculator sequencer behavior.

## Changes

`SequencerSettings` now owns the fixed bounds, increments, rate-ratio table,
step interval calculation, display names, and clamped adjustment operations.
`PatternEditorState` remains the façade consumed by the input, view, and
playback code, delegating settings access to this small value-oriented module.

The module uses fixed storage and no dynamic allocation. It is still local to
the showcase: a future shared clock/control package should be extracted only
when its contract is clearer than this UI-facing settings vocabulary.

## Verification

```sh
pio test -e native
just build-sequencer
```

The native suite now checks tempo and volume clamping plus both ends of the
rate list, in addition to the pattern-chain behavior.
