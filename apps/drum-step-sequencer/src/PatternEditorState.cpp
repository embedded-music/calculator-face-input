#include "PatternEditorState.h"

PatternEditorState::PatternEditorState() = default;

bool PatternEditorState::stepActive(uint8_t track, uint8_t step) const {
  return patterns_.stepActive(currentPattern_, track, step);
}

bool PatternEditorState::patternEmpty(uint8_t pattern) const {
  return patterns_.patternEmpty(pattern);
}

const DrumSound& PatternEditorState::soundForTrack(uint8_t track) const {
  return patterns_.soundForTrack(currentPattern_, track);
}

uint8_t PatternEditorState::selectedSoundIndex() const {
  return patterns_.soundIndex(currentPattern_, selectedTrack_);
}

void PatternEditorState::selectTrack(uint8_t track) {
  if (track < TRACK_COUNT) selectedTrack_ = track;
}

bool PatternEditorState::toggleStep(uint8_t step) {
  return patterns_.toggleStep(currentPattern_, selectedTrack_, step);
}

bool PatternEditorState::advanceByElapsedSteps(uint32_t elapsedSteps) {
  patternChangedAtBoundary_ = false;
  if (elapsedSteps == 0) return false;
  const bool crossedBoundary =
      elapsedSteps >= STEP_COUNT || currentStep_ + elapsedSteps >= STEP_COUNT;
  currentStep_ = static_cast<uint8_t>(
      (currentStep_ + (elapsedSteps % STEP_COUNT)) % STEP_COUNT);
  if (crossedBoundary) {
    chain_.advance();
    const uint8_t nextCurrentPattern = chain_.currentPattern();
    patternChangedAtBoundary_ = currentPattern_ != nextCurrentPattern;
    currentPattern_ = nextCurrentPattern;
    nextPattern_ = chain_.nextPattern();
  }
  return crossedBoundary;
}

void PatternEditorState::selectNextPattern(uint8_t pattern) {
  if (pattern >= PATTERN_SLOT_COUNT) return;
  chain_.setNextPattern(pattern);
  nextPattern_ = pattern;
}

void PatternEditorState::cloneCurrentPatternTo(uint8_t pattern) {
  if (pattern >= PATTERN_SLOT_COUNT) return;
  patterns_.clone(currentPattern_, pattern);
}

void PatternEditorState::clearPattern(uint8_t pattern) {
  patterns_.clear(pattern);
}

bool PatternEditorState::toggleChainPosition(uint8_t position) {
  const bool changed = chain_.togglePosition(position);
  nextPattern_ = chain_.nextPattern();
  return changed;
}

void PatternEditorState::selectSound(uint8_t soundIndex) {
  if (soundIndex < DRUM_SOUND_COUNT) {
    patterns_.selectSound(currentPattern_, selectedTrack_, soundIndex);
  }
}
