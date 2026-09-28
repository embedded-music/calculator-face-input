#include "TriggerPatternCursor.h"

#include "TriggerPatternEmitter.h"

TriggerPatternCursorUpdate TriggerPatternCursor::advance(
    uint32_t elapsedSteps, uint8_t cycleLength) {
  TriggerPatternCursorUpdate update;
  update.elapsedSteps = elapsedSteps;
  update.previousStep = currentStep_;
  update.currentStep = currentStep_;
  if (elapsedSteps == 0 || cycleLength == 0) return update;

  update.completedCycles = elapsedSteps / cycleLength;
  const uint32_t partialStep =
      static_cast<uint32_t>(currentStep_) + (elapsedSteps % cycleLength);
  update.completedCycles += partialStep / cycleLength;
  currentStep_ = static_cast<uint8_t>(partialStep % cycleLength);
  update.currentStep = currentStep_;
  return update;
}

uint8_t TriggerPatternCursor::emitCurrentStep(
    const TriggerPattern& pattern, TriggerEventSink& sink) const {
  return TriggerPatternEmitter::emitStep(pattern, currentStep_, sink);
}
