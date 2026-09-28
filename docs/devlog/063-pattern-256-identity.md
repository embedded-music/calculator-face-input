# 063 — Name the instrument Pattern 256

## Goal

Graduate the Calculator input experiment into a named drum-sequencer product
without changing its musical behavior or obscuring its current limits.

## Identity

The instrument is now **Pattern 256**, an Embedded Music project. The name
comes from a 16-step pattern and a 16-position arrangement target. It is
device-neutral enough to cover both the existing M5Stack Faces Calculator
edition and a planned Cardputer ADV edition.

The firmware header and boot diagnostic now identify Pattern 256. Historical
devlogs keep their original names because they describe the path from the
Calculator protocol probe to the current instrument.

## Current limit

The Calculator edition currently exposes twelve arrangement cells because its
top row selects four patterns and the remaining keypad cells include Clone and
Clear controls. Its maximum arrangement is therefore 12 × 16, or 192 steps.
The product name records the 16 × 16 target; it does not silently redefine the
working firmware as already supporting 256 arranged steps.

Expanding the arrangement is deliberately left for a later interaction slice
with its own on-device validation.

## Product boundary

The Calculator edition remains the reference implementation and first polish
target. A Cardputer ADV port should reuse the musical model and playback
behavior, but it will own a keyboard-appropriate input map and screen design.
Shared sequencing, edition UX, and audio output remain separate boundaries.

## Validation

```text
just test-native
pio test -d packages/step-trigger -e native
just build-sequencer
```

On hardware, boot the Calculator edition and confirm that the main pattern
screen says `PATTERN 256`, while editing, playback, settings, sounds, and the
arrangement screen behave as before. Serial output should begin with
`pattern256: edition=calculator` and report Calculator detection.
