# Add percentage swing to the settings screen

## Goal

Turn the percentage-swing research into a hardware-testable musical feature
before deciding which timing concepts belong in shared ecosystem code.

## Interaction

The fourth pair on the A-button Settings screen controls swing:

- `3` decreases swing by one percentage point;
- `+` increases swing by one percentage point;
- the range is 50--75%;
- 50% is straight timing;
- triplet step rates retain the value but display `--` and bypass swing.

The four compact Settings values now show volume, tempo, rate, and swing.

## Timing policy

For a straight interval `T` and swing percentage `p`, consecutive intervals
are `2T * p` and `2T * (1 - p)`. Their combined duration remains `2T`, so
swing does not move the next pair, pattern boundary, or arrangement boundary.

The previous shared deadline clock assumes a single repeated interval. This
experiment therefore uses a local sequencer step clock that evaluates every
alternating deadline. A late poll walks all elapsed long and short intervals,
preserving the existing policy: logical state catches up, while stale audio is
not replayed as a burst.

Changing tempo, rate, or swing preserves progress through the current long or
short interval. This is intentionally local evidence for the later clock and
vocabulary boundary slice; no shared package is changed yet.

## Verification

```text
pio test -e native
just build-sequencer
```

Sixteen native tests passed and the Core Gray firmware build passed. Native
coverage verifies alternating 60/40 deadlines, unchanged pair duration, late
poll counting, and swing bypass on a triplet rate.

Hardware validation remains:

1. program identical hits on consecutive steps at `1/16`;
2. compare 50%, 60%, about 67%, and 75%;
3. confirm the pattern tempo and boundary remain stable while the internal
   long--short feel changes;
4. select `1/16T`, confirm the display shows `--`, and confirm equal spacing;
5. return to `1/16` and confirm the previous percentage is restored;
6. change swing during a long and short interval and listen for phase jumps or
   double triggers.
