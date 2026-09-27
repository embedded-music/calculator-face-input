# step-trigger

`step-trigger` provides fixed-size, platform-independent trigger patterns for
embedded music applications.

A pattern contains four lanes, storage for at most 32 positions, and an active
cycle length from 1 to 32. Each stored position is off, weak, normal, or strong.
Playback emits lane-and-level events without assigning those lanes to drums,
samples, MIDI notes, synth voices, or physical trigger outputs.

The package deliberately does not own tempo, meter, swing, deadlines,
transport, pattern arrangement, sound assignment, audio, or UI.

```cpp
#include <TriggerPattern.h>
#include <TriggerPatternPlayer.h>

TriggerPattern pattern;
pattern.setLength(12);
pattern.setStepLevel(0, 0, StepLevel::Strong);
pattern.setStepLevel(1, 3, StepLevel::Normal);

// TriggerPatternPlayer::emitStep(pattern, position, sink);
```

The fixed 32-position capacity covers common four-beat grids including 12
eighth-note triplets, 16 sixteenth notes, and 24 sixteenth-note triplets while
remaining allocation-free.
