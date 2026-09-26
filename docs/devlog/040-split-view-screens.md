# 040 — Split view screen implementations

## Goal

Make the UI redraw responsibilities easier to locate without changing the
calculator sequencer's layout or public view façade.

## Changes

The pattern grid and playhead code remains in `PatternEditorView.cpp`. The
settings, sounds, and arrangement implementations now live in
`PatternEditorViewScreens.cpp`. `PatternEditorView` remains the single façade
used by input and playback, so this is an internal organization change rather
than a new UI contract.

The split keeps the existing partial-redraw methods intact: settings values,
sound selection, arrangement values, playhead changes, and pattern changes are
still updated through their existing narrow paths.

## Verification

```sh
pio test -e native
just build-sequencer
```
