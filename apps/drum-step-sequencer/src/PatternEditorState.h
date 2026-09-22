#pragma once

#include <stdint.h>

constexpr uint8_t TRACK_COUNT = 4;
constexpr uint8_t STEP_COUNT = 16;
constexpr uint16_t MIN_TEMPO_BPM = 40;
constexpr uint16_t MAX_TEMPO_BPM = 240;
constexpr uint16_t TEMPO_INCREMENT_BPM = 5;
constexpr uint8_t VOLUME_INCREMENT = 16;

class PatternEditorState {
 public:
  uint8_t selectedTrack() const { return selectedTrack_; }
  uint8_t currentStep() const { return currentStep_; }
  uint16_t tempoBpm() const { return tempoBpm_; }
  uint8_t speakerVolume() const { return speakerVolume_; }
  bool stepActive(uint8_t track, uint8_t step) const;
  void selectTrack(uint8_t track);
  bool toggleStep(uint8_t step);
  void advanceStep();
  void decreaseTempo();
  void increaseTempo();
  void decreaseVolume();
  void increaseVolume();

 private:
  bool steps_[TRACK_COUNT][STEP_COUNT]{};
  uint8_t selectedTrack_ = 0;
  uint8_t currentStep_ = 0;
  uint16_t tempoBpm_ = 120;
  uint8_t speakerVolume_ = 128;
};
