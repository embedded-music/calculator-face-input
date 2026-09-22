# Consolidate the settings layer

## Goal

Move tempo control into the proven Core A modifier layer so Core B and Core C
remain available for future musical interactions.

## Interaction

While Core A is held, the Calculator operators control two settings:

| Chord | Action |
| --- | --- |
| A + `-` / `+` | decrease/increase speaker volume by 16 |
| A + `/` / `*` | decrease/increase tempo by 5 BPM |

The settings footer shows both current values and the two operator pairs.
Unsupported Calculator keys are ignored while A is held. Core B and Core C
also continue to suppress normal Calculator editing while held, but neither
button has an assigned command yet.

Tempo changes still affect future clock intervals without resetting the
playhead. Volume changes still update M5Unified directly without changing drum
event velocity.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray:

1. hold A and use `-` and `+`, confirming volume changes without toggling steps;
2. keep holding A and use `/` and `*`, confirming tempo changes without
   selecting a track or toggling steps;
3. release A and confirm all four operators resume their normal step-editing
   roles;
4. hold C and press Calculator operators, confirming C no longer changes tempo
   and no steps are edited;
5. confirm the settings footer remains legible at boundary values.

## Hardware result

Pending validation on the original Core Gray with the Calculator Face.
