#pragma once

#include <stdint.h>

class StepClock {
 public:
  explicit StepClock(uint32_t intervalMs) : intervalMs_(intervalMs) {}

  void begin(uint32_t nowMs);
  uint8_t elapsedSteps(uint32_t nowMs);

 private:
  uint32_t intervalMs_;
  uint32_t nextStepMs_ = 0;
  bool started_ = false;
};
