#include "TriggerPatternLoader.h"

bool TriggerPatternLoader::load(TriggerPattern& pattern, uint8_t length,
                                const TriggerPatternCell* cells,
                                size_t cellCount) {
  if (length == 0 || length > TriggerPattern::STEP_CAPACITY) return false;
  if (cellCount > 0 && cells == nullptr) return false;

  for (size_t index = 0; index < cellCount; index++) {
    if (cells[index].lane >= TriggerPattern::LANE_COUNT ||
        cells[index].step >= length) {
      return false;
    }
  }

  pattern.clear();
  pattern.setLength(length);
  for (size_t index = 0; index < cellCount; index++) {
    pattern.setStepLevel(cells[index].lane, cells[index].step,
                         cells[index].level);
  }
  return true;
}
