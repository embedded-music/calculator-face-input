# Add accent levels to calculator step editing

## Goal

Make the new `StepLevel` states reachable without taking space away from the
16-step calculator grid.

## Interaction

- calculator key alone: toggle a `Normal` step;
- calculator key while A is held: toggle a `Weak` step;
- calculator key while B is held: toggle a `Strong` step;
- A+B together use the normal level rather than choosing ambiguously.

A and B now wait for release before toggling their settings/sounds screens. If
a calculator value arrives while one is held, the button is consumed as a
modifier instead. This preserves the existing one-button screen navigation
for short taps while making the modifier gesture possible.

## Implementation

The input event carries modifier flags, and the router chooses the active
`StepLevel` when editing a cell. Existing active cells still toggle off,
regardless of which modifier is held.

## Verification

```text
pio test -e native
just build-sequencer
```

Eleven native tests passed and the Core Gray firmware build passed. Hardware
validation remains: verify short A/B taps still change screens, and held A/B
plus calculator keys create visibly different cyan levels and audible levels.
