# Visual pattern editor

## Goal

Start the drum step sequencer with a silent, hardware-testable UI slice. The
Calculator Face selects one of four tracks and toggles one of sixteen boolean
steps. The Core Gray display makes the complete pattern state visible before a
clock or audio backend adds timing constraints.

## Display boundary

The app uses the `M5Unified` and `M5GFX` stack already proven by the protocol
probe. Its renderer deliberately stays within a small graphics vocabulary:
display dimensions, filled rectangles, rectangle outlines, cursor-positioned
text, and colors. This is the practical common denominator for small embedded
displays and keeps the application model independent from a widget framework.

The screen is an immediate-mode view of four track rows by sixteen step
columns. The selected track uses an explicit yellow label with black `T1`-`T4`
text rather than relying on a font background as an implicit selection shape.
It redraws the whole view only at startup. Track selection redraws the old and
new rows; step edits redraw only one cell. No full-screen sprite or framebuffer
is allocated.

ASCII remains useful in Serial diagnostics, but a text-only screen would make
selection and the 64-step overview harder to scan. LVGL and desktop-oriented
SDL would add lifecycle, buffering, and dependency costs without helping this
fixed grid. If a second display family later becomes a real consumer, the
small renderer vocabulary is a natural seam for an adapter; no reusable UI
package is introduced speculatively.

## Interaction

The mapping follows the observed physical protocol:

```text
AC   M    %    /       select tracks 1-4
7    8    9    *       toggle steps 1-4
4    5    6    -       toggle steps 5-8
1    2    3    +       toggle steps 9-12
.    0   +/-   =       toggle steps 13-16
```

The long-hold bytes from `AC` (`0x08`) and `=` (`0x0D`) are ignored. Core
A/B/C edges remain visible in Serial output, but their musical commands are
intentionally deferred to the later gesture slice so short/hold behavior is
introduced together and tested as one policy.

## Verification

Build and upload with:

```sh
just build-sequencer
just upload-sequencer
just monitor
```

On hardware:

1. Confirm the display shows four rows of sixteen inactive cells.
2. Press each top-row Calculator key and confirm the corresponding track row
   receives the yellow selection outline.
3. On every selected track, press all sixteen grid keys and confirm the
   matching cells toggle cyan and then off on a second press.
4. Hold `AC` and `=` and confirm their long values do not edit the pattern.
5. Press and release Core A/B/C and confirm Serial reports both edges without
   changing the pattern.

This slice intentionally contains no transport, playhead, tempo, persistence,
mute state, pattern navigation, or sound.

## Hardware result

Validated on the original M5Stack Core Gray with the Calculator Face. Track
selection and step toggling followed the physical key map. The first selected
track treatment, which used the text glyph background itself, was not legible
on the device; replacing it with an explicit yellow label and black `T1`-`T4`
text made the active track clear. The corrected presentation was validated on
hardware and closes this slice.
