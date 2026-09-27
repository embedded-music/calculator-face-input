#pragma once

#include <stdint.h>

#include "TriggerEventSink.h"
#include "TriggerPattern.h"

class TriggerPatternPlayer {
 public:
  // Emits active lanes in ascending lane order and returns their count.
  static uint8_t emitStep(const TriggerPattern& pattern, uint8_t step,
                          TriggerEventSink& sink);
};
