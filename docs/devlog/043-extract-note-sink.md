# 043 — Extract the playback note sink

## Goal

Keep playback timing and pattern traversal independent from the AMY audio
backend.

## Changes

`SequencerNoteSink` is the small semantic output boundary used by
`SequencerPlayback`: wake the output path and emit a MIDI note-on. The current
firmware adapter, `AmyDrumNoteSink`, forwards those operations to
`AmyAudioActivityGate` and `AmySynthSlot`.

This preserves the existing AMY behavior while allowing a future fake sink for
host playback tests or a different audio backend for another hardware target.
The sink is reference-owned and adds no dynamic allocation.

## Verification

```sh
just build-sequencer
pio test -e native
```

The Core Gray firmware and all seven native tests pass.
