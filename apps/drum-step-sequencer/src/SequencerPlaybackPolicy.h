#pragma once

#include <stdint.h>

#include "PatternEditorState.h"

struct PlaybackStepDecision {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  bool patternBoundary = false;
  bool patternChanged = false;
  bool triggerCurrentStep = false;
};

class SequencerPlaybackPolicy {
 public:
  // Move logical time to the present. Only a single on-time step is eligible
  // for audio; overdue steps are intentionally discarded.
  static PlaybackStepDecision advance(PatternEditorState& editor,
                                       uint32_t elapsedSteps) {
    PlaybackStepDecision decision;
    decision.elapsedSteps = elapsedSteps;
    if (elapsedSteps == 0) return decision;

    const TriggerPatternCursorUpdate cursor =
        editor.advanceByElapsedSteps(elapsedSteps);
    decision.previousStep = cursor.previousStep;
    decision.patternBoundary = cursor.crossedCycle();
    decision.patternChanged = editor.patternChangedAtBoundary();
    decision.triggerCurrentStep = cursor.shouldEmit();
    return decision;
  }
};
