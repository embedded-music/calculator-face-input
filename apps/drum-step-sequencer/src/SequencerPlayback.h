#pragma once

#include <stdint.h>

#include "PatternEditorState.h"
#include "SequencerPlaybackPolicy.h"
#include "SequencerClock.h"
#include "SequencerNoteSink.h"

struct SequencerPlaybackUpdate {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  bool patternBoundary = false;
  bool patternChanged = false;
};

class SequencerPlayback {
 public:
  SequencerPlayback(PatternEditorState& editor, SequencerClock& stepClock,
                    SequencerNoteSink& noteSink)
      : editor_(editor),
        stepClock_(stepClock),
        noteSink_(noteSink) {}

  SequencerPlaybackUpdate update(uint64_t nowUs);

 private:
  void triggerCurrentStep();

  PatternEditorState& editor_;
  SequencerClock& stepClock_;
  SequencerNoteSink& noteSink_;
};
