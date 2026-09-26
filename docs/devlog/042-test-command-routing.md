# 042 — Test semantic command routing

## Goal

Verify that each UI mode translates physical calculator values into explicit
semantic actions before the router mutates application state.

## Changes

`SequencerCommandMap` now maps calculator values to actions such as
`ToggleStep`, `SelectSound`, `ToggleChainPosition`, `SelectPattern`, tempo and
volume changes, clone, and clear. `SequencerCommandRouter` consumes this map;
the physical input adapter remains unaware of these mode-specific meanings.

Native tests cover representative values in all four modes, including the
ambiguous keys whose meaning changes between Pattern, Sounds, and Arrangement.

## Verification

```sh
pio test -e native
just build-sequencer
```

The suite now has seven passing tests and the Core Gray firmware still builds.
