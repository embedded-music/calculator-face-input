# One audible drum track

## Goal

Make only track 1 audible to validate that AMY rendering, Core Gray speaker
output, Calculator input, and incremental display updates can coexist without
disturbing the silent clock established in the preceding slice.

## Audio path

The app consumes the published `amy-synth-m5` 0.3.1 package. It uses
`AmySynthSlot` with the legacy GM drum patch 258, `AmyM5SpeakerBridge` for the
Core Gray PCM path, and `AmyAudioActivityGate` to stop idle speaker output.

Track 1 triggers GM note 38 at full velocity. This Acoustic Snare sound was one
of the usable notes found by the manual drum browser. Tracks 2-4 remain silent,
even when their steps are active. Exact four-track instrument selection remains
a later hardware-listening decision. The app sets the Core Gray speaker volume
to 128 on M5Unified's 0-255 scale after the AMY bridge initializes it, reducing
the demo output to approximately half of the bridge default.

At each clock tick, the state advances first. If the new step is active on T1,
the app wakes the audio path and submits the AMY event before redrawing the two
playhead columns. No per-step or per-trigger Serial logging was added to the
timing path.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray:

1. activate isolated and adjacent steps on T1 and confirm note 38 sounds at the
   matching playhead positions;
2. edit T1 repeatedly during playback and listen for timing gaps or clicks;
3. select and edit T2-T4 and confirm they remain silent;
4. confirm the magenta playhead remains smooth while the speaker is active;
5. leave T1 empty and confirm the speaker path returns to quiet after its tail.

## Hardware result

Validated on the original Core Gray with the Calculator Face. Track 1 sounded
in time with the playhead, editing remained responsive, and display updates did
not produce audible interference. The initial bridge volume was unnecessarily
loud for the test environment; setting the M5Unified speaker level to 128 made
the output comfortable without changing the AMY event velocity.
