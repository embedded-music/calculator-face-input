# Use neutral names for sequencer output events

## Goal

Keep the playback boundary useful for both AMY MIDI-style drum events and a
future PCM sample backend.

## Change

Renamed `SequencerNoteSink` to `SequencerEventSink` and its `noteOn` operation
to `trigger`. The current `AmyDrumEventSink` still translates the sound id to
AMY's MIDI-note API, but that implementation detail no longer leaks into the
sequencer's output contract. A PCM sink can map the same trigger to a sample,
while a melodic backend may later expose a richer note event type of its own.

## Verification

The existing native playback test and the Core Gray firmware build are rerun
after the rename.
