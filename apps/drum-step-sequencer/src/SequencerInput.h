#pragma once

#include <stdint.h>

#include "DeadlineClock.h"
#include "PatternEditorState.h"
#include "PatternEditorView.h"
#include "UiMode.h"

class SequencerInput {
 public:
  SequencerInput(PatternEditorState& editor, PatternEditorView& view,
                 DeadlineClock& stepClock)
      : editor_(editor), view_(view), stepClock_(stepClock) {}

  void update(uint64_t nowUs);
  UiMode mode() const { return mode_; }

 private:
  void handleCalculatorValue(uint8_t value, uint64_t nowUs);
  void reportCoreButtons();
  bool rescheduleStepClock(uint64_t nowUs);

  PatternEditorState& editor_;
  PatternEditorView& view_;
  DeadlineClock& stepClock_;
  UiMode mode_ = UiMode::Pattern;
};
