#include "PatternBank.h"

namespace {
constexpr uint8_t DEFAULT_SOUND_INDICES[TRACK_COUNT] = {0, 4, 10, 18};
}

PatternBank::PatternBank() {
  for (uint8_t pattern = 0; pattern < PATTERN_SLOT_COUNT; pattern++) {
    for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      patterns_[pattern].soundIndices[track] = DEFAULT_SOUND_INDICES[track];
    }
  }
}

bool PatternBank::stepActive(uint8_t pattern, uint8_t track,
                             uint8_t step) const {
  return pattern < PATTERN_SLOT_COUNT && track < TRACK_COUNT &&
         step < STEP_COUNT && patterns_[pattern].steps[track][step];
}

bool PatternBank::toggleStep(uint8_t pattern, uint8_t track, uint8_t step) {
  if (pattern >= PATTERN_SLOT_COUNT || track >= TRACK_COUNT ||
      step >= STEP_COUNT) {
    return false;
  }
  bool& active = patterns_[pattern].steps[track][step];
  active = !active;
  return active;
}

bool PatternBank::patternEmpty(uint8_t pattern) const {
  if (pattern >= PATTERN_SLOT_COUNT) return true;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint8_t step = 0; step < STEP_COUNT; step++) {
      if (patterns_[pattern].steps[track][step]) return false;
    }
  }
  return true;
}

uint8_t PatternBank::soundIndex(uint8_t pattern, uint8_t track) const {
  if (pattern >= PATTERN_SLOT_COUNT || track >= TRACK_COUNT) return 0;
  return patterns_[pattern].soundIndices[track];
}

const DrumSound& PatternBank::soundForTrack(uint8_t pattern,
                                             uint8_t track) const {
  return DRUM_SOUNDS[soundIndex(pattern, track)];
}

void PatternBank::selectSound(uint8_t pattern, uint8_t track,
                              uint8_t soundIndexValue) {
  if (pattern < PATTERN_SLOT_COUNT && track < TRACK_COUNT &&
      soundIndexValue < DRUM_SOUND_COUNT) {
    patterns_[pattern].soundIndices[track] = soundIndexValue;
  }
}

void PatternBank::clone(uint8_t sourcePattern, uint8_t targetPattern) {
  if (sourcePattern < PATTERN_SLOT_COUNT && targetPattern < PATTERN_SLOT_COUNT) {
    patterns_[targetPattern] = patterns_[sourcePattern];
  }
}

void PatternBank::clear(uint8_t pattern) {
  if (pattern >= PATTERN_SLOT_COUNT) return;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    for (uint8_t step = 0; step < STEP_COUNT; step++) {
      patterns_[pattern].steps[track][step] = false;
    }
  }
}
