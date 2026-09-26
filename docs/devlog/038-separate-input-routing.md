# 038 — Separate physical input from command routing

## Goal

Make the sequencer's controls portable to another frontend, such as a
Cardputer keyboard or a desktop event loop, without changing the calculator
behavior.

## Changes

`SequencerInput` now only polls the Core buttons and calculator I²C link. It
turns those observations into a small `SequencerInputEvent` vocabulary.
`SequencerCommandRouter` owns the semantic actions: mode toggles, pattern and
sound selection, step editing, arrangement commands, and settings changes.

The current router still owns display and speaker side effects because those
are part of the showcase command behavior. A future frontend can reuse the
same event dispatch path while replacing only the physical event adapter.
No dynamic allocation was added.

## Verification

```sh
pio test -e native
just build-sequencer
```

The native state tests remain green and the Core Gray firmware builds with the
same memory footprint profile.
