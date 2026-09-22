# Pattern terminology

## Goal

Choose a precise name for the four-track, sixteen-step musical block before it
becomes a code-level type and user-interface label.

## Decision

The app uses **track** for each instrument row and **pattern** for the complete
four-track block. The previous planning term **section** has been replaced.

`Pattern` matches drum-machine vocabulary and describes the musical data stored
by the app. `Section` more often describes a role in a larger arrangement, such
as an intro or chorus, which this baseline does not model.

The other nearby terms keep narrower meanings:

- a **page** is a viewport over part of a larger pattern;
- a **scene** combines independently reusable patterns or clips;
- a **section** may later describe the structural role of a pattern or scene in
  a song;
- a **song** orders musical material over time.

The Calculator baseline stores pattern data directly:

```text
pattern
  -> four tracks
     -> sixteen boolean steps
```

It does not yet contain pattern references, scenes, pages, or a song arrangement.

## Interaction impact

Core button B clears the current pattern when held. Core button C queues the
next pattern on a short press and the previous pattern on hold. A queued change
still becomes audible only at the sixteen-step boundary.

The fixed-capacity growth rules are unchanged: at least one pattern exists, a
new empty pattern is revealed only after advancing from a non-empty final
pattern, and an empty final pattern blocks further growth.

## Verification

The README and the sequencer plan were reviewed so that user-facing and future
code vocabulary consistently use track and pattern. No firmware behavior or
shared package contract changes in this documentation-only slice.
