#pragma once

#include <stdint.h>

#include "StepLevel.h"

class TriggerPattern {
 public:
  static constexpr uint8_t LANE_COUNT = 4;
  static constexpr uint8_t STEP_CAPACITY = 32;

  uint8_t length() const { return length_; }
  bool setLength(uint8_t length);

  bool stepActive(uint8_t lane, uint8_t step) const;
  StepLevel stepLevel(uint8_t lane, uint8_t step) const;
  bool toggleStep(uint8_t lane, uint8_t step);
  bool setStepLevel(uint8_t lane, uint8_t step, StepLevel level);
  bool empty() const;
  void clear();

 private:
  StepLevel steps_[LANE_COUNT][STEP_CAPACITY]{};
  uint8_t length_ = 1;
};
