#include "SequencerPlayback.h"

#include "TriggerPatternPlayer.h"

namespace {
constexpr uint32_t DRUM_TAIL_MS = 1500;

class CalculatorTriggerRouter final : public TriggerEventSink {
 public:
  CalculatorTriggerRouter(PatternEditorState& editor,
                          SequencerEventSink& output)
      : editor_(editor), output_(output) {}

  void trigger(const TriggerEvent& event) override {
    if (!started_) {
      output_.wake(DRUM_TAIL_MS);
      started_ = true;
    }
    output_.trigger(
        SequencerEvent{editor_.soundForTrack(event.lane).midiNote, event.level});
  }

 private:
  PatternEditorState& editor_;
  SequencerEventSink& output_;
  bool started_ = false;
};
}  // namespace

void SequencerPlayback::triggerCurrentStep() {
  CalculatorTriggerRouter router(editor_, eventSink_);
  TriggerPatternPlayer::emitStep(editor_.currentTriggerPattern(),
                                 editor_.currentStep(), router);
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
