# 016 — Track labels use calculator keys

## Goal

Make track selection discoverable on the main pattern page. The previous
`T1`–`T4` labels described implementation numbering but did not show which
calculator keys select each track.

## Change

Track labels now mirror the calculator's track-selection keys:

| Track | Key |
| --- | --- |
| 1 | `AC` |
| 2 | `M` |
| 3 | `%` |
| 4 | `/` |

The selected-track highlight and all step editing behavior are unchanged.

## Verification

- `pio run -e drum-step-sequencer`
- Confirm on hardware that the four labels fit in the left column and that
  pressing each calculator key still selects the corresponding track.
