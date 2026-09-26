#pragma once

#include <stdint.h>

#include "DeadlineClock.h"
#include "SequencerCommandRouter.h"

class SequencerInput {
 public:
  SequencerInput(PatternEditorState& editor, PatternEditorView& view,
                 DeadlineClock& stepClock)
      : router_(editor, view, stepClock) {}

  void update(uint64_t nowUs);
  UiMode mode() const { return router_.mode(); }

 private:
  void reportCoreButtons();
  SequencerCommandRouter router_;
};
