# Calculator Face Drum Sequencer

An experimental four-track, sixteen-step drum instrument for the original
M5Stack Faces Calculator and M5Stack Core Gray.

This repository is where the instrument grew from a Calculator input probe
into a playable hardware demo. It contains the firmware, the interaction
experiments, and the devlogs that record the discoveries behind it. It is not
yet a reusable sequencer library or a finished product.

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

## Controls

### Pattern screen

```text
AC   M    %    /       select tracks 1–4

7    8    9    *       steps 1–4
4    5    6    -       steps 5–8
1    2    3    +       steps 9–12
.    0   +/-   =       steps 13–16
```

Press a track key to select it. Press a step key to toggle that cell.

### Core buttons

- Button **A** to open settings. Calculator keys adjust volume,
  tempo, and step rate.
- Button **B** to browse sounds for the selected track.
- **C** is reserved for future pattern and transport interactions.

The display shows the active mode and the relevant Calculator key legend so
the instrument remains discoverable without a separate controller.

## Build and flash

[`PlatformIO Core`][platformio-core] 6.2 or newer and the [`pioarduino`][pioarduino] platform are required.

```sh
just build-sequencer
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

Each feature is developed as a small hardware-testable slice. Shared code is
extracted only after real consumers reveal a stable contract. A future product
repository may eventually grow from this experiment, but this repository
remains the laboratory and historical record for now.

[amy]: https://github.com/shorepine/amy
[amy-synth-m5]: https://github.com/fczuardi/amy-synth-m5
[late-poll]: docs/devlog/026-missed-step-policy.md
[musical-clock]: https://github.com/embedded-music/musical-clock
[pioarduino]: https://github.com/pioarduino/platform-espressif32
[platformio-core]: https://platformio.org/install/cli
