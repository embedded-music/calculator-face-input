# 019 — Remove low-value footer help text

The final helper line was removed from the pattern, Settings, and Sounds
views. State values remain visible, but the bottom row is now intentionally
quiet and available for future UI expansion.

The change is visual only; controls and selection behavior are unchanged.

## Verification

- `pio run -e drum-step-sequencer`
- Check the main, Settings, and Sounds screens for retained state values and a
  clean lower edge without helper instructions.
