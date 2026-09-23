# 013 — Replace the playhead column with an underline

## Goal

Make the playback position a lighter visual marker and reduce display traffic
during continuous playback.

## Design

The playhead is now a three-pixel horizontal magenta underline near the bottom of
each cell. It no longer covers the full cell height or redraws whole cells when
the playhead advances.

On each step, the view restores the previous underline using the cell's active
or inactive fill color, then draws the new underline. Full cell rendering is
still used when drawing the initial screen or editing a cell.

## Verification

Build and flash the drum sequencer, then verify that the underline remains
visible over active and inactive cells, follows all sixteen steps, and does not
alter the pattern editing controls.
