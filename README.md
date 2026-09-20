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
maps as follows:

```text
AC  A/08   M  M     %  %     /  /
7   7      8  8     9  9     *  *
4   4      5  5     6  6     -  -
1   1      2  2     3  3     +  +
.   .      0  0    +/- `      =  =/0D
```

`AC` emits `A` when a short press is released. If held, it emits `0x08` as soon
as an internal time threshold is reached; releasing it afterward emits nothing.
`=` follows the same pattern: `=` on a short release, or carriage return
(`0x0D`) when the hold threshold is reached followed by a silent release. The
other keys emit the displayed character on release. Core A/B/C provide normal
press and release edges independently of the Face.
