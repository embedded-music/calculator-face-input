# Pattern 256

Pattern 256 is a compact drum sequencer for M5Stack devices. The current
edition turns the original M5Stack Faces Calculator and M5Stack Core Gray into
a four-track, sixteen-step instrument; a Cardputer ADV edition is planned.

The name describes the product's sixteen-step patterns and sixteen-position
arrangement: a chain can span 256 programmed steps before repeating.

This repository began as a Calculator input probe and records the instrument's
growth into a playable hardware experiment. It contains the firmware, shared
sequencing packages, and narrative devlogs behind Pattern 256.

Pattern 256 is an [Embedded Music][embedded-music] project. The firmware is
developed in the open; a future product site may package polished releases,
manuals, media, and supported downloads separately.

## What it does

The Calculator's four-by-four key area is a sixteen-step grid. Its top row
selects one of four drum tracks, while the remaining keys toggle steps in the
selected track. The Core Gray speaker plays one-shot drum sounds through
[AMY][amy]. A visible playhead moves continuously across the pattern.

The default kit is deliberately small: four contrasting drum roles with a
fixed catalog of validated General MIDI percussion sounds. Each track can
choose another sound from the catalog without changing the pattern data.

The sequencer currently starts at boot and loops one pattern. Tempo and step
rate can be changed while playing, and the clock preserves phase when those
values change.

## Hardware

- M5Stack Core Gray
- M5Stack Faces Calculator
- USB cable for flashing and serial monitoring

The Calculator is connected through the Core Gray's I2C face connector. The
firmware uses the Core Gray display, buttons, and internal speaker.

## Editions and architecture

The Calculator edition is the reference implementation and immediate polish
target. The planned Cardputer ADV edition will share musical behavior and
fixed-size pattern data while owning its keyboard mapping, screen layout, and
hardware integration. Device interfaces should fit their controls rather than
imitate one another.

The repository currently keeps the shipping firmware and its supporting
packages together. Future reorganization should preserve three boundaries:

- shared sequencing code owns musical data and deterministic playback;
- an edition owns input mapping and presentation;
- an audio backend owns hardware sound output.

## Controls

### Pattern screen

```text
AC   M    %    /       select tracks 1–4

7    8    9    *       steps 1–4
4    5    6    -       steps 5–8
1    2    3    +       steps 9–12
.    0   +/-   =       steps 13–16
```

Press a track key to select it. Press a step key to toggle that cell. Hold Core
button **A** while pressing one or more step keys to assign weak hits, or hold
Core button **B** to assign strong hits. Modified presses replace the existing
level instead of toggling the step off.

### Core buttons

- Press **A** to toggle the settings screen. Press it again to return to the
  pattern.
- Press **B** to toggle the sound browser for the selected track. Press it
  again to return to the pattern.
- Press **C** to toggle the arrangement screen. Its yellow top row selects the
  next pattern with **AC**, **M**, **%**, or **/**. The sixteen cells below are
  chain positions: press their calculator keys to toggle positions on or off.
  The active positions, in grid order, form the chain. The current and
  upcoming chain positions are marked in the grid.
  While the arrangement is open, Core button **A** toggles Clone mode; when it
  is ON, choosing a pattern first copies the current pattern into that slot
  before queuing it. Core button **B** clears all steps from the currently
  selected next pattern while preserving its sound choices. Core button **C**
  returns to the pattern screen.

The Settings screen uses two compact Calculator rows: `A`/`M` adjust volume,
`%`/`/` adjust tempo, `7`/`8` adjust step rate, and `9`/`*` adjust swing from
50% (straight) through 75% (hard shuffle). Swing delays alternating steps
without changing the duration of each two-step pair. Triplet rates bypass
swing and show `--`; the selected percentage returns when a straight rate is
selected again.

Short A and B taps remain screen toggles; their screen action is suppressed
when the button is used as a step modifier. Pattern changes are applied at the
end of the current sixteen-step cycle.

The display shows the active mode and the relevant Calculator key legend so
the instrument remains discoverable without a separate controller.

## Build and flash

[`PlatformIO Core`][platformio-core] 6.2 or newer and the [`pioarduino`][pioarduino] platform are required.

```sh
just build-sequencer
just test-native
just upload-sequencer
just monitor
```

The serial monitor runs at 115200 baud. Useful diagnostics include step
advancement, tempo/rate changes, sound selection, and transport recovery.

The repository also contains the original protocol probe:

```sh
just build-probe
just upload-probe
```

## M5Burner package

Build a versioned, merged image suitable for M5Burner's custom firmware and
publishing flow with:

```sh
just package-m5burner
```

The ignored output is written under `dist/m5burner/`. See the
[M5Burner publishing guide](docs/publishing/m5burner.md) for binary layout,
hardware validation, and submission fields.

## Timing and dependencies

The app converts BPM and the selected musical rate into a step duration, then
uses the platform-independent [`musical-clock`][musical-clock]
package for absolute monotonic deadlines. The clock reports elapsed physical
intervals but does not know about beats, patterns, or audio. The sequencer
owns its playhead and applies a real-time best-effort policy: stale sounds are
discarded after a late poll, and playback resumes at the next deadline rather
than producing a delayed burst. See the [late-poll policy][late-poll] devlog
for the validation experiment.

Audio is synthesized by AMY, an open-source embedded synthesizer by Shore Pine,
and delivered to the Core Gray speaker
through our [`amy-synth-m5`][amy-synth-m5]
integration package. AMY owns the synthesis engine and PCM rendering; the
local package supplies the M5Stack/Arduino bridge, slot configuration, and
speaker scheduling needed by this experiment. The adapter is still an
external dependency while its long-term package boundary is being explored.

## Development notes

The narrative history lives in [`docs/devlog/`](docs/devlog/). Start with:

- [`002-drum-step-sequencer-plan.md`](docs/devlog/002-drum-step-sequencer-plan.md)
  — the original interaction and architecture plan;
- [`012-deadline-clock-migration.md`](docs/devlog/012-deadline-clock-migration.md)
  — migration from the local step clock;
- [`014-step-rate-control.md`](docs/devlog/014-step-rate-control.md)
  — musical rate selection;
- [`026-missed-step-policy.md`](docs/devlog/026-missed-step-policy.md)
  — late-poll and stale-audio behavior.

Each feature is developed as a small hardware-testable slice. Historical
chapters retain their original Calculator-oriented terminology; new chapters
use the Pattern 256 product name and identify edition-specific decisions.

[amy]: https://github.com/shorepine/amy
[amy-synth-m5]: https://github.com/fczuardi/amy-synth-m5
[embedded-music]: https://github.com/embedded-music
[late-poll]: docs/devlog/026-missed-step-policy.md
[musical-clock]: https://github.com/embedded-music/musical-clock
[pioarduino]: https://github.com/pioarduino/platform-espressif32
[platformio-core]: https://platformio.org/install/cli
