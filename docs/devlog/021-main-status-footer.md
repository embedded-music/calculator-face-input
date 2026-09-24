# 021 — Main status footer shows rate and percentage volume

The main-page footer now uses one large line with BPM, step rate, and abbreviated
volume as a percentage. This avoids the old `128 / 255` representation and
makes the three playback settings easier to scan without spending two rows.

## Verification

- `pio run -e drum-step-sequencer`
- Confirm that BPM, rate, and `vol NN%` remain inside the 320×240 display at
  the minimum and maximum values.
