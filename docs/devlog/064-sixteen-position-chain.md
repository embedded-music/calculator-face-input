# 064 — Complete the sixteen-position chain

## Goal

Make Pattern 256's name describe a working limit: sixteen steps in each
pattern and sixteen positions in the arrangement chain.

## Interaction

The arrangement screen keeps its top Calculator row as the four pattern
selectors. All sixteen keys in the remaining four rows now map directly to
chain positions in reading order. This makes every position visible and
addressable without paging.

Clone and Clear previously occupied the `*` and `=` cells. In Arrangement
mode they move to Core buttons A and B, respectively, while Core button C
returns to the pattern screen. The header shows these contextual controls and
the current Clone state.

Outside Arrangement mode, Core A and B retain their existing Settings and
Sounds behavior and remain weak/strong step modifiers on the Pattern screen.

## Model

`PatternChain` now owns fixed-size arrays of sixteen entries. Its sparse-chain
behavior is unchanged: only enabled positions play, disabling the current
position takes effect at the next boundary, and at least one position remains
enabled.

## Validation

```text
just test-native
pio test -d packages/step-trigger -e native
just build-sequencer
```

The native suite maps all sixteen lower Calculator keys and exercises a fully
enabled chain through positions 1–16 and back to position 1.

On hardware, open Arrangement with Core C, enable all sixteen lower cells,
assign visibly different patterns across the chain, and confirm that the
current-position marker visits every cell before wrapping. Confirm A toggles
Clone, B clears the queued pattern, and C returns to the Pattern screen.
