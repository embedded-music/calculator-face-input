#include "PatternBank.h"

namespace {
constexpr uint8_t DEFAULT_SOUND_INDICES[TRACK_COUNT] = {0, 4, 10, 18};
}

PatternBank::PatternBank() {
  for (uint8_t pattern = 0; pattern < PATTERN_SLOT_COUNT; pattern++) {
    patterns_[pattern].triggers.setLength(STEP_COUNT);
    for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      patterns_[pattern].soundIndices[track] = DEFAULT_SOUND_INDICES[track];
    }
  }
}

bool PatternBank::stepActive(uint8_t pattern, uint8_t track,
                             uint8_t step) const {
  if (pattern >= PATTERN_SLOT_COUNT) return false;
  return patterns_[pattern].triggers.stepActive(track, step);
}

StepLevel PatternBank::stepLevel(uint8_t pattern, uint8_t track,
                                 uint8_t step) const {
  if (pattern >= PATTERN_SLOT_COUNT || track >= TRACK_COUNT ||
      step >= STEP_COUNT) {
    return StepLevel::Off;
  }
  return patterns_[pattern].triggers.stepLevel(track, step);
}

bool PatternBank::toggleStep(uint8_t pattern, uint8_t track, uint8_t step) {
  if (pattern >= PATTERN_SLOT_COUNT) return false;
  return patterns_[pattern].triggers.toggleStep(track, step);
}

bool PatternBank::setStepLevel(uint8_t pattern, uint8_t track, uint8_t step,
                               StepLevel level) {
  if (pattern >= PATTERN_SLOT_COUNT) return false;
  return patterns_[pattern].triggers.setStepLevel(track, step, level);
}

bool PatternBank::patternEmpty(uint8_t pattern) const {
  if (pattern >= PATTERN_SLOT_COUNT) return true;
  return patterns_[pattern].triggers.empty();
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
  patterns_[pattern].triggers.clear();
}

const TriggerPattern& PatternBank::triggerPattern(uint8_t pattern) const {
  return patterns_[pattern < PATTERN_SLOT_COUNT ? pattern : 0].triggers;
}
