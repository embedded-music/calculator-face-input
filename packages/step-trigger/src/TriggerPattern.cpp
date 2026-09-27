#include "TriggerPattern.h"

bool TriggerPattern::setLength(uint8_t length) {
  if (length == 0 || length > STEP_CAPACITY) return false;

  if (length < length_) {
    for (uint8_t lane = 0; lane < LANE_COUNT; lane++) {
      for (uint8_t step = length; step < length_; step++) {
        steps_[lane][step] = StepLevel::Off;
      }
    }
  }
  length_ = length;
  return true;
}

bool TriggerPattern::stepActive(uint8_t lane, uint8_t step) const {
  return stepLevel(lane, step) != StepLevel::Off;
}

StepLevel TriggerPattern::stepLevel(uint8_t lane, uint8_t step) const {
  if (lane >= LANE_COUNT || step >= length_) return StepLevel::Off;
  return steps_[lane][step];
}

bool TriggerPattern::toggleStep(uint8_t lane, uint8_t step) {
  if (lane >= LANE_COUNT || step >= length_) return false;
  StepLevel& level = steps_[lane][step];
  level = level == StepLevel::Off ? StepLevel::Normal : StepLevel::Off;
  return level != StepLevel::Off;
}

bool TriggerPattern::setStepLevel(uint8_t lane, uint8_t step,
                                  StepLevel level) {
  if (lane >= LANE_COUNT || step >= length_) return false;
  steps_[lane][step] = level;
  return level != StepLevel::Off;
}

bool TriggerPattern::empty() const {
  for (uint8_t lane = 0; lane < LANE_COUNT; lane++) {
    for (uint8_t step = 0; step < length_; step++) {
      if (steps_[lane][step] != StepLevel::Off) return false;
    }
  }
  return true;
}

void TriggerPattern::clear() {
  for (uint8_t lane = 0; lane < LANE_COUNT; lane++) {
    for (uint8_t step = 0; step < STEP_CAPACITY; step++) {
      steps_[lane][step] = StepLevel::Off;
    }
  }
}
