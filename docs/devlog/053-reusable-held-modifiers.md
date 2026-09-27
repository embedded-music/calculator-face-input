# Reuse a held modifier across steps

## Goal

Allow one continuous A or B hold to assign the same intensity to several
Calculator steps.

The first modifier implementation consumed the Core button after one
Calculator event. That prevented its screen action, but also made the button
unavailable to the next key until it was released and pressed again.

## Change

Each A/B gesture now records two independent facts:

- the button press is still pending and physically held;
- at least one Calculator key consumed the gesture as a modifier.

Consuming the gesture no longer ends the hold. Every Calculator key received
before release therefore carries the same modifier. On release, any consumed
gesture suppresses the button's screen toggle; an unused gesture still acts as
a normal short tap.

The small gesture state is independent of M5Unified so native tests cover
multiple uses during one hold, tap suppression, and ordinary short taps.

## Verification

```text
pio test -e native
just build-sequencer
```

Hardware validation remains: hold A and assign several weak steps, then hold B
and replace several of them with strong steps. Releasing either button after
the edits must not change screens, while a standalone tap still must.

## Hardware result

Validated on the Core Gray with the Calculator Face. Weak, normal, and strong
levels were audibly distinct; modifier presses replaced existing levels
without removing the step; and one continuous A or B hold successfully edited
multiple Calculator steps. The revised interaction was comfortable enough to
close the accent-input experiment.
