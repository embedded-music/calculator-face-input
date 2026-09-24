# 018 — Sounds selection modal

## Goal

Make the B modifier discoverable and give the player a full-screen view of all
20 available drum patches.

## Design

Holding B now opens a 4×5 keypad-shaped modal. Each cell shows the calculator
key and a compact sound label; the selected patch is highlighted. The footer
shows the selected track and the full patch name. Calculator input while B is
held keeps the existing one-to-one mapping from the 20 keys to the 20 sounds.

Releasing B restores the pattern editor. The clock, audio, and pattern state
continue running while the modal is visible.

Incremental playhead painting is suppressed while B is held so the modal stays
visually stable, just as it does for the Settings modal while A is held.

## Verification

- `pio run -e drum-step-sequencer`
- Flash and hold B; verify all 20 labels fit, the selected patch highlight
  moves after a calculator key press, and releasing B restores the grid.
