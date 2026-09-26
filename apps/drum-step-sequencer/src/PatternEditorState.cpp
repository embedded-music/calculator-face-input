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
constexpr uint8_t DEFAULT_SOUND_INDICES[TRACK_COUNT] = {0, 4, 10, 18};
}  // namespace

PatternEditorState::PatternEditorState() {
  for (uint8_t pattern = 0; pattern < PATTERN_SLOT_COUNT; pattern++) {
    for (uint8_t track = 0; track < TRACK_COUNT; track++) {
      patterns_[pattern].soundIndices[track] = DEFAULT_SOUND_INDICES[track];
    }
  }
}

bool PatternEditorState::stepActive(uint8_t track, uint8_t step) const {
  return track < TRACK_COUNT && step < STEP_COUNT &&
         patterns_[currentPattern_].steps[track][step];
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
  return DRUM_SOUNDS[
      patterns_[currentPattern_].soundIndices[track < TRACK_COUNT ? track : 0]];
}

uint8_t PatternEditorState::selectedSoundIndex() const {
  return patterns_[currentPattern_].soundIndices[selectedTrack_];
}

void PatternEditorState::selectTrack(uint8_t track) {
  if (track < TRACK_COUNT) selectedTrack_ = track;
}

bool PatternEditorState::toggleStep(uint8_t step) {
  if (step >= STEP_COUNT) return false;
  bool& active = patterns_[currentPattern_].steps[selectedTrack_][step];
  active = !active;
  return active;
}

bool PatternEditorState::advanceByElapsedSteps(uint32_t elapsedSteps) {
  if (elapsedSteps == 0) return false;
  const bool crossedBoundary =
      elapsedSteps >= STEP_COUNT || currentStep_ + elapsedSteps >= STEP_COUNT;
  currentStep_ = static_cast<uint8_t>(
      (currentStep_ + (elapsedSteps % STEP_COUNT)) % STEP_COUNT);
  if (crossedBoundary) currentPattern_ = nextPattern_;
  return crossedBoundary;
}

void PatternEditorState::selectNextPattern(uint8_t pattern) {
  if (pattern < PATTERN_SLOT_COUNT) nextPattern_ = pattern;
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
    patterns_[currentPattern_].soundIndices[selectedTrack_] = soundIndex;
  }
}
