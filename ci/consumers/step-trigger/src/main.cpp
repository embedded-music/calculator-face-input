#include "TriggerEventSink.h"
#include "TriggerPattern.h"
#include "TriggerPatternEmitter.h"

class ConsumerSink final : public TriggerEventSink {
 public:
  void trigger(const TriggerEvent& event) override {
    lastLane = event.lane;
    lastLevel = event.level;
    count++;
  }

  uint8_t lastLane = 0;
  StepLevel lastLevel = StepLevel::Off;
  uint8_t count = 0;
};

int main() {
  TriggerPattern pattern;
  pattern.setLength(12);
  pattern.setStepLevel(2, 11, StepLevel::Strong);

  ConsumerSink sink;
  const uint8_t emitted = TriggerPatternEmitter::emitStep(pattern, 11, sink);
  return emitted == 1 && sink.count == 1 && sink.lastLane == 2 &&
                 sink.lastLevel == StepLevel::Strong
             ? 0
             : 1;
}
