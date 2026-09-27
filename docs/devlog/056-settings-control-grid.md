# Consolidate the settings controls into two rows

## Goal

Make the four Settings pairs easier to scan and operate by filling two complete
Calculator rows instead of placing every pair in the two right-hand columns.

## Mapping

| Calculator keys | Setting |
| --- | --- |
| `A` / `M` | volume down / up |
| `%` / `/` | tempo down / up |
| `7` / `8` | rate down / up |
| `9` / `*` | swing down / up |

The colored key cells and the four value cards retain the same setting colors.
Only input placement changes; timing and value ranges remain unchanged.

## Verification

```text
pio test -e native
just build-sequencer
```

Hardware validation remains: open Settings with A, exercise all four pairs,
and confirm every colored key changes only its matching value card.

## Hardware result

Validated on the Core Gray with the Calculator Face. The two-row control grid
matched the display and all four pairs operated their intended setting. The
layout was accepted as the completed Settings redistribution slice.
