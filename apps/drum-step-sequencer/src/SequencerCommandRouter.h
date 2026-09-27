#pragma once

#include <stdint.h>

#include "SequencerStepClock.h"
#include "PatternEditorState.h"
#include "PatternEditorView.h"
#include "SequencerInputEvent.h"
#include "UiMode.h"

class SequencerCommandRouter {
 public:
  SequencerCommandRouter(PatternEditorState& editor, PatternEditorView& view,
                         SequencerStepClock& stepClock)
      : editor_(editor), view_(view), stepClock_(stepClock) {}

  void dispatch(const SequencerInputEvent& event, uint64_t nowUs);
  UiMode mode() const { return mode_; }

 private:
  void handleCalculatorValue(const SequencerInputEvent& event,
                             uint64_t nowUs);
  void handleCoreButton(SequencerInputEventType button);
  bool rescheduleStepClock(uint64_t nowUs);

  PatternEditorState& editor_;
  PatternEditorView& view_;
  SequencerStepClock& stepClock_;
  UiMode mode_ = UiMode::Pattern;
};
