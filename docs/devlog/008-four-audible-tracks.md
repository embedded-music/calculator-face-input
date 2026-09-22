# Four audible drum tracks

## Goal

Expand the validated one-track audio path to all four tracks and test AMY drum
layering through the small Core Gray speaker. This slice keeps the same clock,
editor, display updates, and half-volume output.

## Candidate sound set

Each track uses a note already rated usable in the manual drum browser:

| Track | GM note | Sound |
| --- | ---: | --- |
| T1 | 38 | Acoustic Snare |
| T2 | 42 | Closed Hi-Hat |
| T3 | 46 | Open Hi-Hat |
| T4 | 55 | Splash Cymbal |

This is a candidate for concurrency and mix testing, not yet the final musical
kit. Earlier listening found the bass-drum and tom-like notes comparatively
poor on the built-in speaker, so they are not forced into this slice merely to
match a conventional four-piece layout.

## Trigger path

At each step, the app scans the fixed four tracks. It wakes the audio output
once if at least one active step is found, then sends every matching note to the
same AMY drum slot. There is no allocation, per-trigger logging, or display
redraw beyond the existing playhead columns.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray:

1. confirm each track produces its distinct assigned sound;
2. place two, three, and four tracks on the same step and confirm each layer is
   still perceptible;
3. combine simultaneous hits with dense adjacent steps and listen for gaps,
   clicks, or unstable timing;
4. edit and change selected tracks during playback and confirm the display and
   Calculator input remain responsive;
5. note whether any sound should be replaced before the kit is treated as a
   musical default.

## Hardware result

Validated on the original Core Gray with the Calculator Face. All four tracks
triggered their assigned sounds, including simultaneous layers, while the
clock, display, and editing interaction continued to work correctly. The sound
set remains a functional test kit rather than a finalized musical default.
