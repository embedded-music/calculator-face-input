#pragma once

#include <stdint.h>

#include "AlternatingDeadlineClock.h"
#include "SequencerClock.h"

class SequencerStepClock final : public SequencerClock {
 public:
  bool begin(uint64_t nowUs, uint64_t straightIntervalUs, uint8_t swingPercent,
             bool swingActive) {
    if (straightIntervalUs == 0) return false;
    const IntervalPair intervals = intervalPair(straightIntervalUs,
                                                swingPercent, swingActive);
    return clock_.begin(nowUs, intervals.first, intervals.second);
  }

  bool reconfigure(uint64_t nowUs, uint64_t straightIntervalUs,
                   uint8_t swingPercent, bool swingActive) {
    if (straightIntervalUs == 0) return false;
    const IntervalPair intervals = intervalPair(straightIntervalUs,
                                                swingPercent, swingActive);
    return clock_.reschedulePreservingPhase(nowUs, intervals.first,
                                             intervals.second);
  }

  SequencerClockAdvance poll(uint64_t nowUs) override {
    return {clock_.poll(nowUs).elapsed_intervals};
  }

  uint64_t nextDeadlineUs() const { return clock_.nextDeadline(); }

 private:
  struct IntervalPair {
    uint64_t first;
    uint64_t second;
  };

  static IntervalPair intervalPair(uint64_t straightIntervalUs,
                                   uint8_t swingPercent, bool swingActive) {
    if (!swingActive) return {straightIntervalUs, straightIntervalUs};
    const uint64_t first =
        (2 * straightIntervalUs * swingPercent) / 100;
    return {first, 2 * straightIntervalUs - first};
  }

  AlternatingDeadlineClock clock_;
};
