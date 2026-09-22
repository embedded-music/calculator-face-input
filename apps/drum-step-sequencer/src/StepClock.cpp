#include "StepClock.h"

void StepClock::begin(uint32_t nowMs) {
  nextStepMs_ = nowMs + intervalMs_;
  started_ = true;
}

uint8_t StepClock::elapsedSteps(uint32_t nowMs) {
  if (!started_) begin(nowMs);

  uint8_t elapsed = 0;
  while (static_cast<int32_t>(nowMs - nextStepMs_) >= 0) {
    nextStepMs_ += intervalMs_;
    if (elapsed < UINT8_MAX) elapsed++;
  }
  return elapsed;
}
