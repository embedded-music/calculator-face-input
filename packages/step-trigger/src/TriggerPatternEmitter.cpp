#include "TriggerPatternEmitter.h"

uint8_t TriggerPatternEmitter::emitStep(const TriggerPattern& pattern,
                                        uint8_t step,
                                        TriggerEventSink& sink) {
  if (step >= pattern.length()) return 0;
  uint8_t emitted = 0;
  for (uint8_t lane = 0; lane < TriggerPattern::LANE_COUNT; lane++) {
    const StepLevel level = pattern.stepLevel(lane, step);
    if (level == StepLevel::Off) continue;
    sink.trigger(TriggerEvent{lane, level});
    emitted++;
  }
  return emitted;
}
