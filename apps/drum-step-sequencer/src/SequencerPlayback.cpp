#include "SequencerPlayback.h"

namespace {
constexpr uint32_t DRUM_TAIL_MS = 1500;
}  // namespace

void SequencerPlayback::triggerCurrentStep() {
  bool hasTrigger = false;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    const StepLevel level = editor_.stepLevel(track, editor_.currentStep());
    if (level != StepLevel::Off) {
      if (!hasTrigger) eventSink_.wake(DRUM_TAIL_MS);
      eventSink_.trigger(
          SequencerEvent{editor_.soundForTrack(track).midiNote, level});
      hasTrigger = true;
    }
  }
}

SequencerPlaybackUpdate SequencerPlayback::update(uint64_t nowUs) {
  SequencerPlaybackUpdate result;
  result.elapsedSteps = stepClock_.poll(nowUs).elapsedSteps;
  if (result.elapsedSteps == 0) return result;

  const PlaybackStepDecision decision =
      SequencerPlaybackPolicy::advance(editor_, result.elapsedSteps);
  result.previousStep = decision.previousStep;
  result.patternBoundary = decision.patternBoundary;
  result.patternChanged = decision.patternChanged;
  if (decision.triggerCurrentStep) {
    triggerCurrentStep();
  }
  return result;
}
