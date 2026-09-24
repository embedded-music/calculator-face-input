#pragma once

#include <stdint.h>

#include "DrumSounds.h"

constexpr uint8_t TRACK_COUNT = 4;
constexpr uint8_t STEP_COUNT = 16;
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
  uint8_t selectedTrack() const { return selectedTrack_; }
  uint8_t currentStep() const { return currentStep_; }
  uint16_t tempoBpm() const { return tempoBpm_; }
  uint64_t stepIntervalUs() const;
  StepRate stepRate() const { return stepRate_; }
  const char* stepRateName() const;
  uint8_t speakerVolume() const { return speakerVolume_; }
  uint8_t selectedSoundIndex() const {
    return soundIndices_[selectedTrack_];
  }
  const DrumSound& selectedSound() const {
    return DRUM_SOUNDS[selectedSoundIndex()];
  }
  const DrumSound& soundForTrack(uint8_t track) const;
  bool stepActive(uint8_t track, uint8_t step) const;
  void selectTrack(uint8_t track);
  bool toggleStep(uint8_t step);
  void advanceStep();
  bool decreaseTempo();
  bool increaseTempo();
  bool decreaseRate();
  bool increaseRate();
  bool decreaseVolume();
  bool increaseVolume();
  void selectSound(uint8_t soundIndex);

 private:
  bool steps_[TRACK_COUNT][STEP_COUNT]{};
  uint8_t selectedTrack_ = 0;
  uint8_t currentStep_ = 0;
  uint16_t tempoBpm_ = 120;
  StepRate stepRate_ = StepRate::Sixteenth;
  uint8_t speakerVolume_ = 128;
  uint8_t soundIndices_[TRACK_COUNT] = {0, 4, 10, 18};
};
