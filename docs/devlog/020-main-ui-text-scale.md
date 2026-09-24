# 020 — Enlarge main-page state text

The freed footer space is now used for more readable status information on the
main pattern page. BPM and volume use the larger text size, and the track
labels (`AC`, `M`, `%`, `/`) are also enlarged so the calculator mapping is
visible at a glance.

The top bar and grid geometry remain unchanged.

## Verification

- `pio run -e drum-step-sequencer`
- Flash and confirm that the larger BPM/volume line fits the footer and that
  all four track labels remain centered and readable in the left column.
