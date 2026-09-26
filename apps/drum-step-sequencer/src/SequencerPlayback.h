#pragma once

#include <stdint.h>

#include "DeadlineClock.h"
#include "PatternEditorState.h"
#include "SequencerPlaybackPolicy.h"
#include "SequencerNoteSink.h"

struct SequencerPlaybackUpdate {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  bool patternBoundary = false;
  bool patternChanged = false;
};

class SequencerPlayback {
 public:
  SequencerPlayback(PatternEditorState& editor, DeadlineClock& stepClock,
                    SequencerNoteSink& noteSink)
      : editor_(editor),
        stepClock_(stepClock),
        noteSink_(noteSink) {}

  SequencerPlaybackUpdate update(uint64_t nowUs);

 private:
  void triggerCurrentStep();

  PatternEditorState& editor_;
  DeadlineClock& stepClock_;
  SequencerNoteSink& noteSink_;
};
