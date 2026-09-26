# 029 — Queue the next pattern

## Goal

Give the Core Gray's C button a first arrangement-oriented screen without
introducing a full pattern chain yet.

## Interaction

The C button toggles the arrangement screen. It displays:

```text
Current = AC
Next    = AC
Clone   = off
```

The Calculator's four track keys select the next pattern slot:

```text
AC -> pattern 1    M -> pattern 2    % -> pattern 3    / -> pattern 4
```

Selecting a slot changes only `Next`. When the sequencer crosses the end of
the sixteen-step cycle, `Current` becomes `Next`, and `Next` remains equal to
the new current pattern instead of becoming empty.

The main pattern header now identifies the current slot as `PATTERN AC`,
`PATTERN M`, `PATTERN %`, or `PATTERN /`.

## Implementation boundary

`PatternEditorState` now owns four fixed-size pattern slots, while the
deadline clock remains unaware of pattern identity. The state reports whether
an elapsed-step advance crossed a pattern boundary; the sequencer then logs
the switch before triggering the newly current pattern. Clone is displayed as
an intentionally inactive future command, not implemented yet.

## Verification

- Build with `just build-sequencer`.
- Flash the Core Gray, press C, select each Calculator track key, and confirm
  that only `Next` changes before step 16.
- Confirm that the switch occurs at the cycle boundary and the main header
  follows the new current slot.
