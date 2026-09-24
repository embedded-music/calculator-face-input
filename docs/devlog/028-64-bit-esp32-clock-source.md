# 028 — Use a 64-bit ESP32 clock source

## Goal

Keep the drum sequencer alive beyond the 32-bit `micros()` wrap point.

## Discovery

An overnight run stopped advancing after roughly 71 minutes and reported:

```text
clock: source_time_reversed previous_us=4294966420 now_us=124 possible_micros_wrap=yes
```

The sequencer was casting `micros()` to `uint64_t`, but the cast happened
after the 32-bit value had already wrapped. `DeadlineClock` therefore saw time
move backwards and correctly stopped advancing deadlines.

## Decision

The ESP32 consumer now uses `esp_timer_get_time()`, whose monotonic result is
64-bit, both when starting the clock and on every loop poll. The temporary
wrap detector was removed because the source no longer has this 32-bit wrap
behavior.

## Verification

- Build with `just build-sequencer`.
- Leave the firmware running past the former 71-minute boundary and confirm
  that the playhead and audio continue advancing.
