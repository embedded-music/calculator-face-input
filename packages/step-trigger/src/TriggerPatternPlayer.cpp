#include "TriggerPatternPlayer.h"

bool TriggerPatternPlayer::begin(uint64_t nowUs, StepIntervals intervals,
                                 TriggerStart start) {
  cursor_.reset();
  const bool started = clock_.begin(nowUs, intervals.firstUs,
                                    intervals.secondUs);
  if (started && start == TriggerStart::EmitImmediately) emitCurrentStep();
  return started;
}

bool TriggerPatternPlayer::changeTiming(uint64_t nowUs,
                                        StepIntervals intervals,
                                        TriggerTimingChange change) {
  if (change == TriggerTimingChange::ResetFromNow) {
    return clock_.begin(nowUs, intervals.firstUs, intervals.secondUs);
  }
  return clock_.reschedulePreservingPhase(nowUs, intervals.firstUs,
                                           intervals.secondUs);
}

bool TriggerPatternPlayer::restart(uint64_t nowUs,
                                   StepIntervals intervals,
                                   TriggerStart start) {
  cursor_.reset();
  const bool started = clock_.begin(nowUs, intervals.firstUs,
                                    intervals.secondUs);
  if (started && start == TriggerStart::EmitImmediately) emitCurrentStep();
  return started;
}

TriggerPatternPlayerUpdate TriggerPatternPlayer::update(uint64_t nowUs) {
  TriggerPatternPlayerUpdate result;
  const ClockAdvance clockAdvance = clock_.poll(nowUs);
  const TriggerPatternCursorUpdate cursorUpdate = cursor_.advance(
      clockAdvance.elapsed_intervals, source_.currentPattern().length());
  result.elapsedSteps = cursorUpdate.elapsedSteps;
  result.previousStep = cursorUpdate.previousStep;
  result.currentStep = cursorUpdate.currentStep;
  result.completedCycles = cursorUpdate.completedCycles;
  if (!result.advanced()) return result;
  if (result.completedCycles > 0) source_.completedCycles(result.completedCycles);
  if (cursorUpdate.shouldEmit()) result.emittedEvents = emitCurrentStep();
  return result;
}

uint8_t TriggerPatternPlayer::emitCurrentStep() {
  return cursor_.emitCurrentStep(source_.currentPattern(), sink_);
}
