# Silent step clock

## Goal

Add the continuously running transport needed by later pattern changes without
introducing audio or Core-button commands. The display should make timing
observable as a playhead moving over the sixteen steps.

## Design

`StepClock` is a small monotonic-millisecond clock. At 120 BPM, each sixteenth
note lasts 125 ms. Deadlines advance from the preceding deadline rather than
from the polling time, avoiding cumulative drift. If the loop is delayed, the
clock reports every elapsed step and the state catches up.

The current step belongs to `PatternEditorState`. Initial hardware testing
showed that white cell outlines were difficult to distinguish from active cyan
steps and selected yellow tracks. The view therefore draws a narrow magenta
bar inside every cell of the current column. Track selection remains yellow,
active steps remain cyan, and the view redraws only the old and new playhead
columns. There are no per-tick Serial logs, keeping the event loop suitable for
later audio work.

Core A/B/C remain diagnostic-only. Their mute, clear, and pattern commands are
deferred until the relevant application state exists.

## Verification

```sh
just build-sequencer
just upload-sequencer
```

On the Core Gray, verify that the magenta playhead:

1. starts immediately and traverses all sixteen columns repeatedly;
2. completes one pattern in approximately two seconds at 120 BPM;
3. remains smooth while tracks are selected and steps are toggled;
4. does not change the existing Calculator mappings or Core-button logs.

## Hardware result

Validated on the original Core Gray with the Calculator Face. The clock moved
regularly through all sixteen steps while editing remained responsive. The
initial white-outline playhead was hard to see over cyan active cells and the
yellow selected row; the replacement magenta bar remained clearly visible in
all three cell states and was accepted on hardware.
