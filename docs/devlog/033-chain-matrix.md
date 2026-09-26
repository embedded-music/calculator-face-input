# 033 — Chain matrix on the arrangement screen

## Goal

Turn the C-button arrangement view into a discoverable calculator-like matrix
and make room for an initial pattern chain. The screen now uses the four
calculator columns and five rows: a yellow first row chooses the next pattern,
while the first three cells of the following four rows display chain slots.

The chain is intentionally limited to twelve positions so every slot is
visible at once. Its initial length is two. `*` decreases the length down to
two and `-` increases it up to twelve; `+` and `=` remain reserved.

## Behavior

Selecting `AC`, `M`, `%`, or `/` writes the selected pattern into the chain
position immediately after the current position. At each sixteen-step
boundary the sequencer advances the chain position, derives the next pattern,
and keeps the current pattern header synchronized. The arrangement view marks
the current slot and the upcoming slot without redrawing the whole screen.

## Verification

- Build with `just build-sequencer`.
- Flash the Core Gray and press C to inspect the 5-by-4 matrix.
- Confirm the top row is yellow, chain slots are cyan while active, and the
  current/upcoming slots have horizontal bars at the bottom of their cells
  (magenta for current and yellow for next), matching the main playhead.
- Use `*` and `-` to test the 2–12 bounds, then select patterns in the top row
  and verify that the chain advances at the end of each sixteen-step cycle.
