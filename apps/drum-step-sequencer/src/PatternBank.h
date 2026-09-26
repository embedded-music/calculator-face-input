#pragma once

#include <stdint.h>

#include "DrumSounds.h"

constexpr uint8_t TRACK_COUNT = 4;
constexpr uint8_t STEP_COUNT = 16;
constexpr uint8_t PATTERN_SLOT_COUNT = 4;

class PatternBank {
 public:
  PatternBank();

  bool stepActive(uint8_t pattern, uint8_t track, uint8_t step) const;
  bool toggleStep(uint8_t pattern, uint8_t track, uint8_t step);
  bool patternEmpty(uint8_t pattern) const;
  uint8_t soundIndex(uint8_t pattern, uint8_t track) const;
  const DrumSound& soundForTrack(uint8_t pattern, uint8_t track) const;
  void selectSound(uint8_t pattern, uint8_t track, uint8_t soundIndex);
  void clone(uint8_t sourcePattern, uint8_t targetPattern);
  void clear(uint8_t pattern);

 private:
  struct PatternData {
    bool steps[TRACK_COUNT][STEP_COUNT]{};
    uint8_t soundIndices[TRACK_COUNT]{};
  };

  PatternData patterns_[PATTERN_SLOT_COUNT]{};
};
