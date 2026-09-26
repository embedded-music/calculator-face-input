#pragma once

#include <stdint.h>

struct SequencerClockAdvance {
  uint32_t elapsedSteps = 0;
};

class SequencerClock {
 public:
  virtual ~SequencerClock() = default;
  virtual SequencerClockAdvance poll(uint64_t nowUs) = 0;
};
