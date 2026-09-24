# 017 — Sounds screen mockup without redundant numbers

## Design note

The Sounds modal uses the calculator keypad as the selector, so numbering the
20 cells adds information that is not useful to the player. The browser
preview now gives the full cell width to each abbreviated patch name and keeps
the selected patch indicated by color and the footer.

This is still a visual experiment; no firmware Sounds screen behavior was
changed in this slice.

## Verification

Open `tools/display-preview/index.html` and select **Sounds (B)**. Confirm that
the four-column by five-row layout remains legible without the numeric labels.
