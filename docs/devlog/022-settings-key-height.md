# 022 — Increase Settings key height

The Settings keypad buttons are now 32 pixels high instead of 27. The value
legends move down accordingly, using the space freed when the footer helper
line was removed. The Sounds keypad keeps its original geometry.

The browser preview was updated to match the firmware layout.

## Verification

- `pio run -e drum-step-sequencer`
- Check the Settings modal at 320×240 and confirm that the five keypad rows and
  three value cards remain separated and fully visible.
