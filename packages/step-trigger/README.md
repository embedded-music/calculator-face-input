# step-trigger

`step-trigger` provides fixed-size, platform-independent trigger patterns for
embedded music applications.

A pattern contains four lanes, storage for at most 32 steps, and an active
cycle length from 1 to 32. Each stored cell is off, weak, normal, or strong.
Playback emits lane-and-level events without assigning those lanes to drums,
samples, MIDI notes, synth voices, or physical trigger outputs.

`TriggerPatternCell` and `TriggerPatternLoader` provide a sparse representation
for compiled presets and future persistence decoders while `TriggerPattern`
remains the dense runtime grid.

`TriggerPatternCursor` owns the playhead within one variable-length cycle. It
accepts elapsed-step counts from an external clock, reports completed cycles,
and suppresses emission after missed deadlines. `TriggerPatternPlayer` composes
that cursor with an alternating deadline clock and a pattern source, hiding the
usual clock/poll/advance/emit plumbing from applications. Applications still
own musical timing values, pattern arrangement, sound assignment, audio, and
UI.

```cpp
#include <TriggerPattern.h>
#include <TriggerPatternPlayer.h>

TriggerPattern pattern;
pattern.setLength(12);
pattern.setStepLevel(0, 0, StepLevel::Strong);
pattern.setStepLevel(1, 3, StepLevel::Normal);

class PatternSource : public TriggerPatternSource {
 public:
  const TriggerPattern& currentPattern() const override { return pattern; }
};

PatternSource source;
TriggerPatternPlayer player(source, sink);
player.begin(nowUs, StepIntervals::constant(stepUs),
             TriggerStart::EmitImmediately);
// Call player.update(nowUs) from the application loop.
```

The fixed 32-step capacity covers common four-beat grids including 12
eighth-note triplets, 16 sixteenth notes, and 24 sixteenth-note triplets while
remaining allocation-free.

Run the package tests and create a registry-compatible archive with:

```text
pio test -e native
pio pkg pack --output /tmp
```
