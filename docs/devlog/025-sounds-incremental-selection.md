# 025 — Incremental Sounds selection redraw

Selecting a patch while B is held no longer redraws the complete Sounds modal.
The view now repaints only the previously selected cell, the newly selected
cell, and the current-patch footer. Entering Sounds still performs a full
screen draw.

## Verification

- `pio run -e drum-step-sequencer`
- On hardware, hold B and cycle through several patches. The keypad should stay
  visually stable while only selection and footer content change.
