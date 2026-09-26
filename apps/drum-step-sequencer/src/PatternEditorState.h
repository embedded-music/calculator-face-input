#pragma once

#include <stdint.h>

#include "DrumSounds.h"
#include "PatternBank.h"
#include "PatternChain.h"

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
  bool cloneMode() const { return cloneMode_; }
  uint8_t chainLength() const { return chain_.length(); }
  uint8_t chainPosition() const { return chain_.currentPosition(); }
  uint8_t nextChainPosition() const { return chain_.nextPosition(); }
  bool chainPositionEnabled(uint8_t position) const {
    return chain_.positionEnabled(position);
  }
  bool chainPositionVisible(uint8_t position) const {
    return chain_.positionVisible(position);
  }
  uint8_t chainPatternAt(uint8_t position) const {
    return chain_.patternAt(position);
  }
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
  bool patternEmpty(uint8_t pattern) const;
  void selectTrack(uint8_t track);
  bool toggleStep(uint8_t step);
  // Move the logical playhead to the present after one or more deadlines.
  // Missed steps are intentionally not replayed as audio bursts.
  // Returns true when crossing the 16-step boundary. The current pattern may
  // stay the same while the chain position advances.
  bool advanceByElapsedSteps(uint32_t elapsedSteps);
  bool patternChangedAtBoundary() const { return patternChangedAtBoundary_; }
  void selectNextPattern(uint8_t pattern);
  void cloneCurrentPatternTo(uint8_t pattern);
  void clearPattern(uint8_t pattern);
  void toggleCloneMode() { cloneMode_ = !cloneMode_; }
  bool toggleChainPosition(uint8_t position);
  bool decreaseTempo();
  bool increaseTempo();
  bool decreaseRate();
  bool increaseRate();
  bool decreaseVolume();
  bool increaseVolume();
  void selectSound(uint8_t soundIndex);

 private:
  PatternBank patterns_;
  uint8_t selectedTrack_ = TRACK_COUNT - 1;
  uint8_t currentStep_ = 0;
  uint8_t currentPattern_ = 0;
  uint8_t nextPattern_ = 0;
  PatternChain chain_;
  bool patternChangedAtBoundary_ = false;
  bool cloneMode_ = false;
  uint16_t tempoBpm_ = 120;
  StepRate stepRate_ = StepRate::Sixteenth;
  uint8_t speakerVolume_ = 128;

};
