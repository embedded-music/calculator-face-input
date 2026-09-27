# Make all three step levels audible

## Goal

Make weak, normal, and strong steps perceptibly different during playback.
The first accent-input build sent both normal and strong steps to AMY at full
velocity, so strong accents changed the display but not the sound.

## Change

The playback mapping now sends these velocities to AMY:

- weak: `0.45`;
- normal: `0.70`;
- strong: `1.00`.

Strong keeps the existing maximum output. Normal moves down enough to leave
useful headroom, and weak remains clearly below it. The mapping lives in a
small hardware-independent policy so native tests can verify that every level
has a distinct, ordered value.

## Verification

```text
pio test -e native
just build-sequencer
```

Hardware validation remains: compare repeated weak, normal, and strong hits on
each drum sound and adjust the spacing if AMY's patches do not make all three
levels perceptually distinct.
