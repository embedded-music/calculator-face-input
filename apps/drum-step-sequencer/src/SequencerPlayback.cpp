#include "SequencerPlayback.h"

#include <Arduino.h>

namespace {
constexpr float DRUM_VELOCITY = 1.0f;
constexpr uint32_t DRUM_TAIL_MS = 1500;
}  // namespace

void SequencerPlayback::triggerCurrentStep() {
  bool hasTrigger = false;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    if (editor_.stepActive(track, editor_.currentStep())) {
      if (!hasTrigger) audioGate_.wake(DRUM_TAIL_MS);
      drumSlot_.noteOn(editor_.soundForTrack(track).midiNote, DRUM_VELOCITY);
      hasTrigger = true;
    }
  }
}

SequencerPlaybackUpdate SequencerPlayback::update(uint64_t nowUs) {
  SequencerPlaybackUpdate result;
  result.elapsedSteps = stepClock_.poll(nowUs).elapsed_intervals;
  if (result.elapsedSteps == 0) return result;

  result.previousStep = editor_.currentStep();
  // Audio events whose deadlines are already in the past cannot be recovered
  // in real time. Advance to the present and discard overdue triggers.
  result.patternBoundary = editor_.advanceByElapsedSteps(result.elapsedSteps);
  result.patternChanged = editor_.patternChangedAtBoundary();
  if (result.patternChanged) {
    Serial.printf("pattern: switched current=%u next=%u\n",
                  editor_.currentPattern() + 1, editor_.nextPattern() + 1);
  }
  if (result.elapsedSteps == 1) {
    triggerCurrentStep();
  } else {
    Serial.printf("transport: skipped_steps count=%lu\n",
                  static_cast<unsigned long>(result.elapsedSteps));
  }
  return result;
}
