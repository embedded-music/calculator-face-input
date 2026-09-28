#pragma once

#include <stdint.h>

#include "StepLevel.h"

// One value at the intersection of a trigger lane and a pattern step.
// Sparse preset and persistence formats can use cells to populate the dense
// fixed-capacity TriggerPattern runtime representation.
struct TriggerPatternCell {
  uint8_t lane;
  uint8_t step;
  StepLevel level;
};
