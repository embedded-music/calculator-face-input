# 033 — Chain matrix on the arrangement screen

## Goal

Turn the C-button arrangement view into a discoverable calculator-like matrix
and make room for an initial pattern chain. The screen now uses the four
calculator columns and five rows: a yellow first row chooses the next pattern,
while the first three cells of the following four rows display chain slots.

The chain is intentionally limited to twelve positions so every slot is
visible at once. Its initial length is one. The twelve calculator cells below
the selector row toggle positions on and off; the enabled positions, in grid
order, form the chain. The chain never becomes empty, so at least one position
remains enabled.

## Behavior

Selecting `AC`, `M`, `%`, or `/` writes the selected pattern into the enabled
chain position immediately after the current position. At each sixteen-step
boundary the sequencer advances to the next enabled position, derives the next
pattern, and keeps the current pattern header synchronized. The arrangement
view marks the current slot and the upcoming slot without redrawing the whole
screen.

Turning off the currently playing position is quantized: it remains visibly
current until the end of its sixteen-step cycle, while the next transition
skips it. This avoids changing the active pattern in the middle of a cycle.

## Verification

- Build with `just build-sequencer`.
- Flash the Core Gray and press C to inspect the 5-by-4 matrix.
- Confirm the top row is yellow, chain slots are cyan while active, and the
  current/upcoming slots have horizontal bars at the bottom of their cells
  (magenta for current and yellow for next), matching the main playhead.
- Toggle several sparse cells in the 12-position matrix, then select patterns
  in the top row and verify that the enabled positions advance at the end of
  each sixteen-step cycle.
