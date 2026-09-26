#pragma once

#include "DeadlineClock.h"
#include "SequencerClock.h"

class DeadlineClockAdapter final : public SequencerClock {
 public:
  explicit DeadlineClockAdapter(DeadlineClock& clock) : clock_(clock) {}

  SequencerClockAdvance poll(uint64_t nowUs) override {
    return {clock_.poll(nowUs).elapsed_intervals};
  }

 private:
  DeadlineClock& clock_;
};
