#pragma once

#include <stdint.h>

#include "TriggerEventSink.h"
#include "TriggerPattern.h"

struct TriggerPatternCursorUpdate {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  uint8_t currentStep = 0;
  uint32_t completedCycles = 0;

  bool crossedCycle() const { return completedCycles > 0; }
  bool shouldEmit() const { return elapsedSteps == 1; }
};

// Owns the playhead within one repeating pattern, but not time. A deadline
// clock or any other scheduler supplies the number of elapsed steps.
class TriggerPatternCursor {
 public:
  uint8_t currentStep() const { return currentStep_; }
  void reset() { currentStep_ = 0; }

  TriggerPatternCursorUpdate advance(uint32_t elapsedSteps,
                                     uint8_t cycleLength);
  uint8_t emitCurrentStep(const TriggerPattern& pattern,
                          TriggerEventSink& sink) const;

 private:
  uint8_t currentStep_ = 0;
};
