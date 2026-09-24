# 015 — Settings modal while holding A

## Goal

Make the calculator controls discoverable without hiding the normal pattern
editor permanently. While A is held, show a dedicated settings page that
explains the three control pairs currently available.

## Design

The A modifier now switches the display to a full-screen settings modal:

- `%` / `/`: volume down/up
- `9` / `*`: tempo down/up
- `6` / `-`: step-rate down/up

The keypad is drawn with the calculator's ASCII-compatible symbols and the three active pairs are
highlighted with matching color legend boxes. The legend also shows the live
volume, BPM, and rate values. Releasing A restores the pattern editor; changing
a value while A remains held redraws the modal so the feedback stays visible.

The browser mockup used during the design discussion lives at
`tools/display-preview/index.html` and mirrors the 320×240 logical display.

## Verification

- `pio run -e drum-step-sequencer`
- Browser preview opened with Chromium and checked at 320×240.
- Firmware build remained within the previous memory profile (about 14.8% RAM
  and 28.6% flash).

## Hardware test

Flash the firmware and hold A. Confirm that the settings page appears, each
calculator pair changes only its intended value, the live legend updates, and
releasing A returns to the pattern grid. Existing track editing and B/C
behavior should remain unchanged when A is not held.

The playhead continues advancing internally while the modal is visible, but
its incremental drawing is suppressed until A is released. This keeps the
settings page visually stable without pausing the sequencer or its audio.
