# Refresh sound selection when the pattern changes

## Observation

When the arrangement advanced to another pattern while the sound screen was
open, its selected sound still belonged to the previous pattern. Selecting a
new sound afterwards could leave both the old and new cells highlighted.

## Fix

Added a dedicated `drawSoundsPatternChange` path. At a pattern boundary it
redraws all sound cells and the footer while preserving the existing screen
header. This clears the old selection and derives the new highlight directly
from the active pattern, without a full-screen redraw on every playback step.

## Verification

```text
just build-sequencer
```

The Core Gray firmware build passed. The intended hardware check is to chain
patterns with different sounds, leave screen B open, and confirm that exactly
one sound cell remains yellow after each transition.
