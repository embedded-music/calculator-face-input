#pragma once

#include <stdint.h>

constexpr uint8_t TRACK_COUNT = 4;
constexpr uint8_t STEP_COUNT = 16;

class PatternEditorState {
 public:
  uint8_t selectedTrack() const { return selectedTrack_; }
  uint8_t currentStep() const { return currentStep_; }
  bool stepActive(uint8_t track, uint8_t step) const;
  void selectTrack(uint8_t track);
  bool toggleStep(uint8_t step);
  void advanceStep();

 private:
  bool steps_[TRACK_COUNT][STEP_COUNT]{};
  uint8_t selectedTrack_ = 0;
  uint8_t currentStep_ = 0;
};
