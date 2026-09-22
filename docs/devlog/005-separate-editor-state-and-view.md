# Separate editor state and view

## Goal

Separate application state, Calculator command decoding, and display rendering
before clock and audio responsibilities are introduced. This is a structural
slice: the validated visual editor behavior must remain unchanged.

## Design

- `PatternEditorState` owns the selected track and fixed 4-by-16 step matrix.
- `CalculatorCommand` translates Calculator protocol bytes into app commands.
- `PatternEditorView` owns M5GFX screen geometry, colors, and rendering.
- `main.cpp` reads hardware and coordinates state changes with partial redraws.

State and command translation do not depend on Arduino, M5Unified, or the
display. The view receives a display reference instead of owning hardware.
These are local app boundaries, not reusable packages.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray, select all four tracks, toggle representative steps including
1 and 16, toggle them off again, and confirm long `AC` and `=` values remain
ignored. The screen and Serial output should be indistinguishable from the
preceding slice.

## Hardware result

Validated on the original Core Gray with the Calculator Face. Track selection,
step toggling, display updates, and ignored long values remained equivalent to
the preceding visual-editor slice.
