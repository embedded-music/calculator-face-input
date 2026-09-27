# Extract the step-trigger package

## Goal

Promote the Calculator's proven step-level and simultaneous-trigger behavior
into an experimental PlatformIO package before building the umbrella preset
drummer. Use a reusable trigger-sequencer vocabulary rather than naming the
mechanism after drums or samples.

The package should be immediately useful to the Calculator while leaving
sound assignment, timing, arrangement, input, display, and AMY behavior in the
application.

## Boundary

The new `packages/step-trigger` library owns:

- `StepLevel`: off, weak, normal, and strong stored positions;
- `TriggerPattern`: four fixed lanes with bounded variable cycle length;
- `TriggerEvent`: one instantaneous lane-and-level result;
- `TriggerEventSink`: the destination-independent event boundary;
- `TriggerPatternPlayer`: emission of all active lanes at one position.

It deliberately does not know whether a lane triggers a drum, sample, MIDI
note, synth voice, envelope, or physical output. It also does not own BPM,
meter, swing, rates, deadlines, transport, pattern chains, sound catalogs,
audio lifecycle, or UI.

```text
clock and application position
             |
             v
     TriggerPatternPlayer
             |
             v
 TriggerEvent { lane, level }
             |
             v
 application routing and output
```

## Capacity and cycle length

A fixed sixteen-position type would have encoded the Calculator surface rather
than the reusable musical concept. `TriggerPattern` therefore separates:

- four fixed lanes;
- storage capacity of 32 positions;
- active cycle length from 1 through 32.

This keeps event paths allocation-free while allowing four-beat cycles such as
12 eighth-note triplets, 16 sixteenth notes, and 24 sixteenth-note triplets.
The package stores only the cycle length; the application decides what the
positions mean in meter and physical time.

Shortening a pattern clears the truncated tail. Lengthening it later cannot
silently resurrect previously hidden triggers.

## Calculator composition

Calculator `PatternBank` now composes one `TriggerPattern` into each of its four
pattern slots. Every Calculator pattern explicitly selects length 16. The
application continues to own the per-pattern sound index for each track:

```text
Calculator pattern slot
├── packaged TriggerPattern
└── Calculator sound index for each lane
```

The existing clone operation copies both components, while clear removes
triggers and preserves sound selections and cycle length.

`SequencerPlayback` delegates active-lane enumeration to
`TriggerPatternPlayer`. A local `CalculatorTriggerRouter` translates each lane
to the currently selected AMY-compatible sound id and forwards the existing
sound event. It wakes the audio backend once before the first event in a
position, preserving the previous lifecycle behavior for simultaneous hits.

The Calculator UI, pattern chain, fixed sixteen-key editor, step-rate and swing
settings, absolute clock, and AMY sink are otherwise unchanged.

## Tests

The new native package tests verify:

- lengths are limited to 1--32;
- a 12-position cycle accepts position 11 and rejects position 12;
- shortening and re-expanding a pattern leaves its truncated tail off;
- simultaneous active lanes emit in stable ascending lane order;
- empty and out-of-range positions emit no events.

The existing Calculator state and playback tests continue to verify cloning,
clearing, accent replacement, simultaneous tracks, pattern transitions,
late-step policy, command routing, swing, and triplet bypass through the new
package composition.

## Verification

```text
pio test -e native
pio run -e drum-step-sequencer
```

All 20 native tests passed: 16 existing Calculator and held-button tests plus
four new `step-trigger` tests. The complete Core Gray firmware build also
passed and linked `step-trigger@0.1.0` as a local PlatformIO dependency.

No intended hardware behavior changed in this extraction. The next umbrella
showcase can use shorter preset cycles and route the same neutral lane events
to its own AMY sound assignments.

## Hardware observation

The extracted package passed a Calculator hardware regression. Pattern
editing, playback, and the existing instrument behavior remained usable on the
Core Gray after the internal pattern and trigger path moved through
`step-trigger`.
