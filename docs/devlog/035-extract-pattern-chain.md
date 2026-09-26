# 035 — Extract the pattern chain

## Goal

Give the sparse chain its own fixed-size module while keeping the editor state
API stable for the current UI and sequencer loop.

## Changes

`PatternChain` now owns enabled positions, pattern assignments, current and
next positions, traversal, and the minimum-length invariant. `PatternEditorState`
delegates the existing chain accessors and commands to it, so the view and
input layers do not need to know how sparse traversal works.

The module remains local to the showcase for now. Its behavior is covered by
the existing native state tests, which continue to exercise sparse traversal,
quantized removal, clone, and clear.

## Verification

```sh
pio test -e native
just build-sequencer
```
