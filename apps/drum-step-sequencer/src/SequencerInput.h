#pragma once

#include <stdint.h>

#include "HeldButtonGesture.h"
#include "SequencerCommandRouter.h"
#include "SequencerStepClock.h"

class SequencerInput {
 public:
  SequencerInput(PatternEditorState& editor, PatternEditorView& view,
                 SequencerStepClock& stepClock)
      : router_(editor, view, stepClock) {}

  void update(uint64_t nowUs);
  UiMode mode() const { return router_.mode(); }

 private:
  void reportCoreButtons();
  void reportPendingCoreButtonReleases();
  HeldButtonGesture buttonA_;
  HeldButtonGesture buttonB_;
  SequencerCommandRouter router_;
};
