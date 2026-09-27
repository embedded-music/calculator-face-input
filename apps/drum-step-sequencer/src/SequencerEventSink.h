#pragma once

#include <stdint.h>

#include "StepLevel.h"

// The minimal event emitted by the current drum sequencer. The current drum
// implementation uses MIDI note numbers as sound ids, while a PCM backend
// could map the same ids to samples.
struct SequencerEvent {
  uint8_t soundId = 0;
  StepLevel level = StepLevel::Normal;
};

// Output boundary for events scheduled by the sequencer.
class SequencerEventSink {
 public:
  virtual ~SequencerEventSink() = default;
  virtual void wake(uint32_t tailMs) = 0;
  virtual void trigger(const SequencerEvent& event) = 0;
};
