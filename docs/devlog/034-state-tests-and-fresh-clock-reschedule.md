# 034 — State tests and fresh clock rescheduling

## Goal

Start the sequencer architecture review with executable coverage for the
sparse chain and remove a subtle timing hazard when tempo or rate changes are
read through I2C.

## Changes

The native PlatformIO environment now runs host-side tests against the pattern
state. The tests cover sparse chain traversal, quantized removal of the current
position, clone, clear, and preservation of track sound choices.

Tempo and rate changes now take a fresh `esp_timer` timestamp immediately
before rescheduling the deadline clock. The loop timestamp captured before the
I2C read is no longer reused after input and redraw work may have elapsed.

## Verification

```sh
pio test -e native
just build-sequencer
```
