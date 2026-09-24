# 023 — Unified screen header

The Pattern, Settings, and Sounds screens now share the same 34-pixel header
height, background color, title position, and title scale. The modifier-help
subtitles were removed from Settings and Sounds; the modal content can use the
space without repeating interaction instructions.

The browser preview was updated to use the same header geometry.

## Verification

- `pio run -e drum-step-sequencer`
- Inspect all three screens and confirm identical header geometry and no
  `HOLD ...` subtitle.
