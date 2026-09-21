# Calculator Face Input

Experimental home for input-driven applications using the original M5Stack
Faces Calculator panel on an M5Stack Core Gray. This repository is not yet a
reusable or published package.

The first app is a protocol probe. Later apps may explore preset and bank
selection, a step sequencer, and other command-oriented musical interactions.
Shared code will remain local until at least two real apps reveal a stable
contract worth extracting.

## Apps

### `probe`

Reports the raw byte returned by the Calculator Face after its interrupt, plus
press and release edges from the Core Gray's A, B, and C buttons. It deliberately
does not assign meanings to Calculator keys before the hardware map has been
observed.

### `drum-step-sequencer` (planned)

A self-contained four-track, sixteen-step drum sequencer for the Calculator
Face and Core Gray speaker. The Calculator selects tracks and toggles steps;
the Core buttons provide mute, clear, and section navigation gestures. It will
run continuously without requiring an external MIDI controller. See
[`docs/devlog/002-drum-step-sequencer-plan.md`](docs/devlog/002-drum-step-sequencer-plan.md)
for the interaction and incremental validation plan.

## Commands

PlatformIO Core 6.2 or newer is required by the current pioarduino platform.

```sh
just build-probe
just upload-probe
just monitor
```

Press and release every Calculator key separately, then test repeated presses
and long holds. Record whether output occurs on press or release and whether a
long hold produces a distinct event. Also verify the Core Gray A/B/C buttons.

## Observed original-panel protocol

Hardware testing found release-oriented ASCII-like events. The physical layout
is:

```text
AC   M    %    /
7    8    9    *
4    5    6    -
1    2    3    +
.    0   +/-   =
```

Most keys emit their printed character on release. The exceptions are:

| Physical key | Short press | Long hold |
| --- | --- | --- |
| `AC` | `A` (`0x41`) on release | `0x08` at the hold threshold; release is silent |
| `+/-` | `` ` `` (`0x60`) on release | Same value on release |
| `=` | `=` (`0x3D`) on release | `0x0D` at the hold threshold; release is silent |

Core A/B/C provide normal press and release edges independently of the Face.
