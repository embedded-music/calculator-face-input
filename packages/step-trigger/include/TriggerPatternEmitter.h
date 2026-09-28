#pragma once
#include <stdint.h>
#include "TriggerEventSink.h"
#include "TriggerPattern.h"

class TriggerPatternEmitter {
 public:
  static uint8_t emitStep(const TriggerPattern& pattern, uint8_t step,
                          TriggerEventSink& sink);
};
