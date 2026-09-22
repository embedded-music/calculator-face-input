# Calculator drum step sequencer plan

## Goal

Build the repository's second app as a self-contained musical instrument using
the original Calculator Face, an M5Stack Core Gray, its display, and its speaker.
It should not require the Arturia controller or another MIDI input device.

The Face's release-oriented protocol is treated as a useful constraint. Keys
edit deliberate sequencer state rather than attempting live note-on and
note-off performance.

This chapter records the intended interaction. It does not yet introduce code,
a reusable sequencer package, or an umbrella contract.

## Physical layout

The first Calculator row selects one of four drum tracks. The remaining four
rows form a spatial 4-by-4 grid of sixteen steps:

```text
AC   M    %    /       tracks 1-4

7    8    9    *       steps 1-4
4    5    6    -       steps 5-8
1    2    3    +       steps 9-12
.    0   +/-   =       steps 13-16
```

An initial sound set can map the four tracks to kick, snare, closed hi-hat,
and open hi-hat. The exact AMY PCM notes and gains must be validated on the
Core Gray rather than fixed by this planning slice.

A short press of a track key selects that track. A short press of a step key
toggles the corresponding step in the selected track. The long values emitted
by Calculator `AC` and `=` are not required and can be ignored by this app.

## Core button gestures

Each Core button has one short and one long gesture:

| Button | Short press | Hold |
| --- | --- | --- |
| A | mute/unmute selected track | mute/unmute all tracks |
| B | clear selected track | clear current pattern |
| C | queue next pattern | queue previous pattern |

Short actions are recognized only when the button is released. Crossing the
hold threshold executes only the long action; the later release must not also
execute the short action. This is especially important for mute and destructive
clear operations.

The sequencer starts playing at boot and remains clocked continuously. There is
no play/stop command in the initial interaction.

## Patterns

One pattern contains four tracks of sixteen steps. Storage has a fixed embedded
capacity, initially proposed as eight patterns, while the number of patterns
revealed to the performer grows with use. No dynamic allocation is needed in
the event or audio paths.

The pattern model follows these invariants:

- at least one pattern always exists;
- created patterns form a contiguous sequence;
- a new pattern is created only when advancing from the non-empty last pattern;
- a last pattern with no active steps does not allow further advancement;
- clearing a pattern never removes it;
- an empty intermediate pattern does not block navigation to later patterns;
- mute state does not affect whether a pattern is considered empty;
- reaching fixed capacity prevents further creation and is shown as `MAX`.

Advancing from a non-empty final pattern creates one empty pattern and queues
it. Pattern changes become audible only at a sixteen-step boundary. Until a
change is queued, the current pattern loops. The display should distinguish
the currently playing pattern from a queued destination.

There is intentionally no remove-pattern command. A cleared last pattern simply
becomes the available space for new composition, avoiding hidden destructive
gestures and unnecessary storage management.

## Display responsibilities

The display should make the state observable without becoming the primary
input surface. At minimum it shows:

- four rows of sixteen steps;
- the currently selected track;
- the current playhead step;
- per-track and global mute state;
- current and queued pattern numbers;
- fixed tempo and capacity feedback such as `MAX`.

The first implementation can use a fixed tempo, proposed as 120 BPM. Tempo,
swing, persistence, pattern duplication, and sound-kit selection remain later
questions rather than requirements for the baseline.

## Architecture boundary

The app may initially keep its clock, patterns, UI policy, and AMY integration
local. Calculator bytes are hardware facts; their meaning as tracks and steps
belongs to this app. Pattern navigation is a composition interaction, not yet
a MIDI Bank Select equivalent or shared umbrella contract.

A reusable sequencer or paged-selection contract should be extracted only if a
second real consumer exposes the same stable concepts.

The umbrella's
[interaction semantics radar](https://github.com/fczuardi/embedded-music-experiments/blob/main/docs/design/interaction-semantics-radar.md)
keeps the wider comparison with samplers and sequencers. Koala and the Roland
P-6 distinguish one-shot triggers from gate-controlled voices, while the P-6
also treats probability, gate time, micro-timing, sub-steps, and motion as
sequencer data. Teenage Engineering Pocket Operators show how a sixteen-key
grid can switch coherently between sound selection, step editing, effects, and
pattern arrangement without making those concepts one generic command.

For this app, that comparison reinforces a deliberately small first model:
Calculator release events edit boolean steps, and active steps produce one-shot
drum triggers. Richer step expression remains research rather than a baseline
requirement. The Face's lack of key-down events therefore does not prevent a
useful first instrument; it primarily rules out faithful gate performance from
those keys.

## Incremental validation

Implementation should proceed in hardware-testable slices:

1. **Visual pattern editor:** select four tracks, toggle sixteen steps, render
   the selected track and pattern, and verify Calculator event mapping.
2. **Continuous clock:** start at boot, advance a visible sixteen-step playhead
   at a fixed tempo, and keep editing responsive.
3. **One audible track:** schedule a kick for active steps and verify stable
   timing through the Core Gray speaker.
4. **Four drum tracks:** validate the chosen AMY PCM drum sounds, mixing, and
   render budget on hardware.
5. **Core gestures:** add short/hold discrimination for mute and clear without
   double execution on release.
6. **Patterns:** add fixed-capacity, incrementally revealed patterns and
   boundary-quantized navigation.

Each slice should build and receive its stated Core Gray validation before the
next one expands the interaction.
