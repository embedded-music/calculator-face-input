# 036 — Extract the pattern bank

## Goal

Separate pattern content from editor and transport state without changing the
showcase's public behavior.

## Changes

`PatternBank` now owns the fixed-size pattern slots, step toggles, track sound
choices, empty checks, clone, and clear operations. `PatternEditorState`
delegates those operations for the current pattern and remains the façade used
by the input, view, and playback code.

The bank is intentionally local to the showcase for now. Its fixed-size data
and pure bounds checks make it a candidate for a later shared composition
module once save/load or another consumer gives that boundary more pressure.

## Verification

```sh
pio test -e native
just build-sequencer
```
