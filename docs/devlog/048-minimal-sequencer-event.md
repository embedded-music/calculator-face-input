# Introduce a minimal sequencer event

## Goal

Make the output contract explicit without adding fields that the current drum
sequencer does not use.

## Change

Added `SequencerEvent`, currently containing only `soundId`. The event sink now
accepts the object by reference. AMY continues to translate that id into its
MIDI-note API, while a PCM sink could use it as a sample identifier.

Velocity was deliberately omitted: all current hits use a fixed level and the
UI has no dynamics feature yet.

## Verification

```text
pio test -e native
just build-sequencer
```
