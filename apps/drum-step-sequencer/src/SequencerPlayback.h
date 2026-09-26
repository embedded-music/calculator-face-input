#pragma once

#include <stdint.h>

#include "AmyAudioActivityGate.h"
#include "AmySynthSlot.h"
#include "DeadlineClock.h"
#include "PatternEditorState.h"

struct SequencerPlaybackUpdate {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  bool patternBoundary = false;
  bool patternChanged = false;
};

class SequencerPlayback {
 public:
  SequencerPlayback(PatternEditorState& editor, DeadlineClock& stepClock,
                    AmyAudioActivityGate& audioGate, AmySynthSlot& drumSlot)
      : editor_(editor),
        stepClock_(stepClock),
        audioGate_(audioGate),
        drumSlot_(drumSlot) {}

  SequencerPlaybackUpdate update(uint64_t nowUs);

 private:
  void triggerCurrentStep();

  PatternEditorState& editor_;
  DeadlineClock& stepClock_;
  AmyAudioActivityGate& audioGate_;
  AmySynthSlot& drumSlot_;
};
