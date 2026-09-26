#pragma once

#include <stdint.h>

#include "PatternEditorState.h"
#include "SequencerPlaybackPolicy.h"
#include "SequencerClock.h"
#include "SequencerEventSink.h"

struct SequencerPlaybackUpdate {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  bool patternBoundary = false;
  bool patternChanged = false;
};

class SequencerPlayback {
 public:
  SequencerPlayback(PatternEditorState& editor, SequencerClock& stepClock,
                    SequencerEventSink& eventSink)
      : editor_(editor),
        stepClock_(stepClock),
        eventSink_(eventSink) {}

  SequencerPlaybackUpdate update(uint64_t nowUs);

 private:
  void triggerCurrentStep();

  PatternEditorState& editor_;
  SequencerClock& stepClock_;
  SequencerEventSink& eventSink_;
};
