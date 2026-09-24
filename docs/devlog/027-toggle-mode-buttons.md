# 027 — Toggle mode buttons

## Goal

Make the Core Gray mode buttons usable with one hand while the other hand
plays the Calculator grid.

## Interaction

- A toggles between the pattern and settings screens.
- B toggles between the pattern and sound-selection screens.
- C always returns to the pattern screen.
- Release events do not change the mode, and no chord is required.

The clock and pattern state continue running while a modal screen is visible.
This is an interaction experiment; future pattern-chain commands may give C a
more specific role once that model exists.

## Verification

- Build with `just build-sequencer`.
- Flash the Core Gray and confirm that A and B enter and leave their modes
  with individual presses, while C returns to the pattern from either mode.
- Confirm Calculator edits continue working after each toggle.
