#pragma once

#include <stddef.h>
#include <stdint.h>

#include "TriggerPattern.h"
#include "TriggerPatternCell.h"

class TriggerPatternLoader {
 public:
  static bool load(TriggerPattern& pattern, uint8_t length,
                   const TriggerPatternCell* cells, size_t cellCount);

  template <size_t CellCount>
  static bool load(TriggerPattern& pattern, uint8_t length,
                   const TriggerPatternCell (&cells)[CellCount]) {
    return load(pattern, length, cells, CellCount);
  }
};
