# Calculator Face protocol probe

## Goal

Characterize the original M5Stack Faces Calculator panel on a Core Gray before
building musical interactions on assumptions inherited from the Keyboard and
Gamepad Faces.

The immediate validation target is a complete map of all 20 keys, including
event timing, repeated presses, and any special behavior caused by long holds.

## Repository role

This repository may contain several experimental apps. Candidate follow-up
apps include a preset or bank controller and a step sequencer. They should
share code locally only when real duplication appears; a public package should
not be designed from this first probe alone.

## Probe design

The initial hardware assumptions match the other original Faces panels:

```text
I2C address  0x08
SDA          GPIO 21
SCL          GPIO 22
IRQ          GPIO 5, active low
```

On each interrupt, the probe reads exactly one byte and prints its hexadecimal,
decimal, and printable-character representations. Identical consecutive values
are never suppressed. The display shows the most recent event, while Serial is
the authoritative observation log.

The Core Gray's A, B, and C buttons are also reported with their real press and
release edges. This keeps all available physical input observable without
conflating Core buttons with Calculator keys.

No musical command, key name, or reusable input event is introduced in this
slice.

## Verification

```sh
just build-probe
just upload-probe
just monitor
```

On hardware:

1. confirm that the Face acknowledges at `0x08`;
2. press and release each of its 20 keys individually;
3. press the same key twice and confirm both events appear;
4. hold representative keys for several seconds and note when output occurs;
5. check for distinct long-press or repeat values;
6. press and release Core A, B, and C.

The hardware observations and final key map will be appended here before this
slice is closed.

## Hardware result

The Face acknowledged at `0x08`. Its ordinary events appeared on key release,
including repeated presses of the same key. The complete physical map is:

```text
key       short press        when held
AC        0x41 A on release  0x08 at threshold; release silent
M         0x4D M on release  same value on release
%         0x25 % on release  same value on release
/         0x2F / on release  same value on release
7         0x37 7 on release  same value on release
8         0x38 8 on release  same value on release
9         0x39 9 on release  same value on release
*         0x2A * on release  same value on release
4         0x34 4 on release  same value on release
5         0x35 5 on release  same value on release
6         0x36 6 on release  same value on release
-         0x2D - on release  same value on release
1         0x31 1 on release  same value on release
2         0x32 2 on release  same value on release
3         0x33 3 on release  same value on release
+         0x2B + on release  same value on release
.         0x2E . on release  same value on release
0         0x30 0 on release  same value on release
+/-       0x60 ` on release  same value on release
=         0x3D = on release  0x0D at threshold; release silent
```

A controlled short-press pass, moving left to right across each physical row,
confirmed the exact sequence:

```text
keys:   AC M  %  /  7  8  9  *  4  5  6  -  1  2  3  +  .  0  +/- =
bytes:  41 4D 25 2F 37 38 39 2A 34 35 36 2D 31 32 33 2B 2E 30 60  3D
```

All 20 events arrived with the IRQ line back high by the time they were logged.

`AC` and `=` therefore encode short versus long interaction as both a different
value and different event timing. Once held past the internal threshold, the
long value is emitted immediately while the key remains down; its eventual
release emits nothing. A short interaction emits its normal value only on
release. Neither key exposes its initial press edge or the measured duration.
This distinction can become a deliberate command gesture in later apps.

Core A, B, and C each produced both `pressed` and `released` events. All 23
physical controls were observed, and identical Calculator events were retained.
The protocol is suitable for deliberate commands, preset selection, and step
editing, but the Calculator keys should not be presented as live held-note
controls.
