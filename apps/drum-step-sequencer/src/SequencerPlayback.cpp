#include "SequencerPlayback.h"

class CalculatorTriggerRouter final : public TriggerEventSink {
 public:
  CalculatorTriggerRouter(PatternEditorState& editor,
                          SequencerEventSink& output)
      : editor_(editor), output_(output) {}

  void trigger(const TriggerEvent& event) override {
    output_.trigger(
        SequencerEvent{editor_.soundForTrack(event.lane).midiNote, event.level});
  }

 private:
  PatternEditorState& editor_;
  SequencerEventSink& output_;
};

void SequencerPlayback::triggerCurrentStep() {
  CalculatorTriggerRouter router(editor_, eventSink_);
  editor_.emitCurrentStep(router);
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
