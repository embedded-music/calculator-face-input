#pragma once

#include <stdint.h>

class SequencerNoteSink {
 public:
  virtual ~SequencerNoteSink() = default;
  virtual void wake(uint32_t tailMs) = 0;
  virtual void noteOn(uint8_t midiNote, float velocity) = 0;
};
