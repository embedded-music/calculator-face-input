#pragma once

#include <stdint.h>

#include "SequencerClock.h"

class SequencerStepClock final : public SequencerClock {
 public:
  bool begin(uint64_t nowUs, uint64_t straightIntervalUs, uint8_t swingPercent,
             bool swingActive) {
    if (straightIntervalUs == 0) return false;
    straightIntervalUs_ = straightIntervalUs;
    swingPercent_ = swingPercent;
    swingActive_ = swingActive;
    nextStep_ = 1;
    nextDeadlineUs_ = nowUs + intervalBefore(nextStep_);
    started_ = true;
    return true;
  }

  bool reconfigure(uint64_t nowUs, uint64_t straightIntervalUs,
                   uint8_t swingPercent, bool swingActive) {
    if (!started_ || straightIntervalUs == 0) return false;
    const uint64_t oldInterval = intervalBefore(nextStep_);
    const uint64_t remaining = nextDeadlineUs_ > nowUs ? nextDeadlineUs_ - nowUs : 0;
    straightIntervalUs_ = straightIntervalUs;
    swingPercent_ = swingPercent;
    swingActive_ = swingActive;
    const uint64_t newInterval = intervalBefore(nextStep_);
    nextDeadlineUs_ = nowUs +
        (remaining * newInterval + oldInterval / 2) / oldInterval;
    return true;
  }

  SequencerClockAdvance poll(uint64_t nowUs) override {
    uint32_t elapsed = 0;
    while (started_ && nowUs >= nextDeadlineUs_ && elapsed < UINT32_MAX) {
      elapsed++;
      nextStep_ = static_cast<uint8_t>((nextStep_ + 1) % 16);
      nextDeadlineUs_ += intervalBefore(nextStep_);
    }
    return {elapsed};
  }

  uint64_t nextDeadlineUs() const { return nextDeadlineUs_; }

 private:
  uint64_t intervalBefore(uint8_t step) const {
    if (!swingActive_) return straightIntervalUs_;
    const uint64_t longInterval =
        (2 * straightIntervalUs_ * swingPercent_) / 100;
    return step % 2 == 1 ? longInterval : 2 * straightIntervalUs_ - longInterval;
  }

  uint64_t straightIntervalUs_ = 0;
  uint64_t nextDeadlineUs_ = 0;
  uint8_t swingPercent_ = 50;
  uint8_t nextStep_ = 1;
  bool swingActive_ = false;
  bool started_ = false;
};
