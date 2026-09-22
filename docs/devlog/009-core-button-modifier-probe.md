# Core-button modifier probe

## Goal

Test whether holding a Core Gray button while releasing Calculator keys is a
comfortable way to expose more controls without introducing menus or consuming
the sixteen step keys during normal editing.

## Interaction

This first modifier slice implements only two reversible settings:

- hold Core A and release Calculator `-` or `+` to change speaker volume by 16
  on M5Unified's 0-255 scale;
- hold Core C and release Calculator `-` or `+` to change tempo by 5 BPM within
  40-240 BPM.

While A or C is held, the footer changes to name the active control layer and
show its current value. Calculator keys without a meaning in that layer are
ignored, preventing accidental pattern edits. Core B remains diagnostic-only;
Calculator input is ignored while it is held so a later Edit layer can be
introduced safely.

No action is attached to releasing a Core button, so a modifier chord cannot
also trigger an unrelated short-press command. Normal Calculator editing
returns as soon as the modifier is released.

Tempo changes update future clock intervals without resetting the current
step or restarting the clock. Volume changes update M5Unified directly and do
not alter AMY event velocity.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray:

1. hold A, press and release `-` and `+`, and confirm the footer and audible
   volume change while the corresponding steps do not toggle;
2. hold C and use `-` and `+`, confirming the footer and playhead rate change;
3. release each modifier and confirm `-` and `+` resume toggling their normal
   steps;
4. try unsupported Calculator keys while holding A, B, and C and confirm the
   pattern is unchanged;
5. evaluate whether keeping the Core button held until the Calculator key is
   released is physically comfortable enough to extend this model.

## Hardware result

Validated on the original Core Gray with the Calculator Face. Both modifier
chords were comfortable and reliable: A with `-`/`+` changed volume, C with
`-`/`+` changed tempo, the contextual footer made the active layer clear, and
normal step editing resumed correctly after release. This supports extending
the modifier model to mute, clear, and pattern controls in later slices.
