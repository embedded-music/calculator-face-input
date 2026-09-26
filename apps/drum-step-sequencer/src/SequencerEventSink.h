#pragma once

#include <stdint.h>

// Output boundary for events scheduled by the sequencer. The current drum
// implementation uses MIDI note numbers as sound ids, while a PCM backend
// could map the same ids to samples.
class SequencerEventSink {
 public:
  virtual ~SequencerEventSink() = default;
  virtual void wake(uint32_t tailMs) = 0;
  virtual void trigger(uint8_t soundId, float velocity) = 0;
};
