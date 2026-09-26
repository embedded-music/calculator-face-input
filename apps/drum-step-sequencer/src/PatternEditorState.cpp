#include "PatternEditorState.h"

#include <cstddef>

namespace {
struct RateRatio {
  uint8_t beatsNumerator;
  uint8_t beatsDenominator;
  const char* name;
};

constexpr RateRatio RATE_RATIOS[] = {
    {4, 1, "1/1"},       {2, 1, "1/2"},       {1, 1, "1/4"},
    {2, 3, "1/4T"},      {1, 2, "1/8"},       {1, 3, "1/8T"},
    {1, 4, "1/16"},      {1, 6, "1/16T"},     {1, 8, "1/32"},
    {1, 12, "1/32T"},
};

constexpr size_t RATE_COUNT = sizeof(RATE_RATIOS) / sizeof(RATE_RATIOS[0]);
}  // namespace

PatternEditorState::PatternEditorState() = default;

bool PatternEditorState::stepActive(uint8_t track, uint8_t step) const {
  return patterns_.stepActive(currentPattern_, track, step);
}

bool PatternEditorState::patternEmpty(uint8_t pattern) const {
  return patterns_.patternEmpty(pattern);
}

uint64_t PatternEditorState::stepIntervalUs() const {
  const RateRatio& ratio = RATE_RATIOS[static_cast<size_t>(stepRate_)];
  return (60000000ULL * ratio.beatsNumerator) /
         (static_cast<uint64_t>(tempoBpm_) * ratio.beatsDenominator);
}

const char* PatternEditorState::stepRateName() const {
  return RATE_RATIOS[static_cast<size_t>(stepRate_)].name;
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

bool PatternEditorState::decreaseTempo() {
  const uint16_t previous = tempoBpm_;
  tempoBpm_ = tempoBpm_ > MIN_TEMPO_BPM + TEMPO_INCREMENT_BPM
                  ? tempoBpm_ - TEMPO_INCREMENT_BPM
                  : MIN_TEMPO_BPM;
  return tempoBpm_ != previous;
}

bool PatternEditorState::increaseTempo() {
  const uint16_t previous = tempoBpm_;
  tempoBpm_ = tempoBpm_ < MAX_TEMPO_BPM - TEMPO_INCREMENT_BPM
                  ? tempoBpm_ + TEMPO_INCREMENT_BPM
                  : MAX_TEMPO_BPM;
  return tempoBpm_ != previous;
}

bool PatternEditorState::decreaseRate() {
  const uint8_t index = static_cast<uint8_t>(stepRate_);
  if (index == 0) return false;
  stepRate_ = static_cast<StepRate>(index - 1);
  return true;
}

bool PatternEditorState::increaseRate() {
  const uint8_t index = static_cast<uint8_t>(stepRate_);
  if (index + 1 >= RATE_COUNT) return false;
  stepRate_ = static_cast<StepRate>(index + 1);
  return true;
}

bool PatternEditorState::decreaseVolume() {
  const uint8_t previous = speakerVolume_;
  speakerVolume_ = speakerVolume_ > VOLUME_INCREMENT
                       ? speakerVolume_ - VOLUME_INCREMENT
                       : 0;
  return speakerVolume_ != previous;
}

bool PatternEditorState::increaseVolume() {
  const uint8_t previous = speakerVolume_;
  speakerVolume_ = speakerVolume_ < UINT8_MAX - VOLUME_INCREMENT
                       ? speakerVolume_ + VOLUME_INCREMENT
                       : UINT8_MAX;
  return speakerVolume_ != previous;
}

void PatternEditorState::selectSound(uint8_t soundIndex) {
  if (soundIndex < DRUM_SOUND_COUNT) {
    patterns_.selectSound(currentPattern_, selectedTrack_, soundIndex);
  }
}
