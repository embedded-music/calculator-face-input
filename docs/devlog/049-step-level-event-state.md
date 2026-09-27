# Represent step intensity with `StepLevel`

## Goal

Prepare pattern cells for weak, normal, and strong accents without introducing
velocity values into the pattern model prematurely.

## Change

Each cell now stores a `StepLevel` enum:

- `Off`
- `Normal`
- `Weak`
- `Strong`

The existing calculator toggle still switches between `Off` and `Normal`, so
the current interaction and sound remain unchanged. Playback includes the
active level in `SequencerEvent`; the AMY sink maps it to its velocity API.
The main pattern renderer already derives three cyan shades from the level,
ready for the modifier gesture in a later slice.

## Verification

```text
pio test -e native
just build-sequencer
```

Ten native tests passed and the Core Gray firmware build passed. Weak and
strong levels are not yet reachable from the UI.
