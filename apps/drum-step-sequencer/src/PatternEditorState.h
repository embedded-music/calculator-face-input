#pragma once

#include <stdint.h>

#include "DrumSounds.h"

constexpr uint8_t TRACK_COUNT = 4;
constexpr uint8_t STEP_COUNT = 16;
constexpr uint8_t PATTERN_SLOT_COUNT = 4;
constexpr uint8_t CHAIN_MIN_LENGTH = 1;
constexpr uint8_t CHAIN_MAX_LENGTH = 12;
constexpr uint16_t MIN_TEMPO_BPM = 40;
constexpr uint16_t MAX_TEMPO_BPM = 240;
constexpr uint16_t TEMPO_INCREMENT_BPM = 5;
constexpr uint8_t VOLUME_INCREMENT = 16;

enum class StepRate : uint8_t {
  Whole,
  Half,
  Quarter,
  QuarterTriplet,
  Eighth,
  EighthTriplet,
  Sixteenth,
  SixteenthTriplet,
  ThirtySecond,
  ThirtySecondTriplet,
};

class PatternEditorState {
 public:
  PatternEditorState();
  uint8_t selectedTrack() const { return selectedTrack_; }
  uint8_t currentStep() const { return currentStep_; }
  uint8_t currentPattern() const { return currentPattern_; }
  uint8_t nextPattern() const { return nextPattern_; }
  uint8_t chainLength() const { return chainLength_; }
  uint8_t chainPosition() const { return chainPosition_; }
  uint8_t nextChainPosition() const { return nextEnabledPosition(chainPosition_); }
  bool chainPositionEnabled(uint8_t position) const {
    return position < CHAIN_MAX_LENGTH && chainEnabled_[position];
  }
  bool chainPositionVisible(uint8_t position) const {
    return chainPositionEnabled(position) || position == chainPosition_;
  }
  uint8_t chainPatternAt(uint8_t position) const;
  uint16_t tempoBpm() const { return tempoBpm_; }
  uint64_t stepIntervalUs() const;
  StepRate stepRate() const { return stepRate_; }
  const char* stepRateName() const;
  uint8_t speakerVolume() const { return speakerVolume_; }
  uint8_t selectedSoundIndex() const;
  const DrumSound& selectedSound() const {
    return DRUM_SOUNDS[selectedSoundIndex()];
  }
  const DrumSound& soundForTrack(uint8_t track) const;
  bool stepActive(uint8_t track, uint8_t step) const;
  void selectTrack(uint8_t track);
  bool toggleStep(uint8_t step);
  // Move the logical playhead to the present after one or more deadlines.
  // Missed steps are intentionally not replayed as audio bursts.
  // Returns true when crossing the 16-step boundary. The current pattern may
  // stay the same while the chain position advances.
  bool advanceByElapsedSteps(uint32_t elapsedSteps);
  bool patternChangedAtBoundary() const { return patternChangedAtBoundary_; }
  void selectNextPattern(uint8_t pattern);
  bool toggleChainPosition(uint8_t position);
  bool decreaseTempo();
  bool increaseTempo();
  bool decreaseRate();
  bool increaseRate();
  bool decreaseVolume();
  bool increaseVolume();
  void selectSound(uint8_t soundIndex);

 private:
  struct PatternData {
    bool steps[TRACK_COUNT][STEP_COUNT]{};
    uint8_t soundIndices[TRACK_COUNT]{};
  };

  PatternData patterns_[PATTERN_SLOT_COUNT]{};
  uint8_t selectedTrack_ = 0;
  uint8_t currentStep_ = 0;
  uint8_t currentPattern_ = 0;
  uint8_t nextPattern_ = 0;
  uint8_t chain_[CHAIN_MAX_LENGTH]{};
  bool chainEnabled_[CHAIN_MAX_LENGTH]{};
  uint8_t chainLength_ = CHAIN_MIN_LENGTH;
  uint8_t chainPosition_ = 0;
  bool patternChangedAtBoundary_ = false;
  uint16_t tempoBpm_ = 120;
  StepRate stepRate_ = StepRate::Sixteenth;
  uint8_t speakerVolume_ = 128;

  uint8_t nextEnabledPosition(uint8_t position) const;
};
