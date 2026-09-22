#include "PatternEditorState.h"

bool PatternEditorState::stepActive(uint8_t track, uint8_t step) const {
  return track < TRACK_COUNT && step < STEP_COUNT && steps_[track][step];
}

void PatternEditorState::selectTrack(uint8_t track) {
  if (track < TRACK_COUNT) selectedTrack_ = track;
}

bool PatternEditorState::toggleStep(uint8_t step) {
  if (step >= STEP_COUNT) return false;
  bool& active = steps_[selectedTrack_][step];
  active = !active;
  return active;
}

void PatternEditorState::advanceStep() {
  currentStep_ = (currentStep_ + 1) % STEP_COUNT;
}

void PatternEditorState::decreaseTempo() {
  tempoBpm_ = tempoBpm_ > MIN_TEMPO_BPM + TEMPO_INCREMENT_BPM
                  ? tempoBpm_ - TEMPO_INCREMENT_BPM
                  : MIN_TEMPO_BPM;
}

void PatternEditorState::increaseTempo() {
  tempoBpm_ = tempoBpm_ < MAX_TEMPO_BPM - TEMPO_INCREMENT_BPM
                  ? tempoBpm_ + TEMPO_INCREMENT_BPM
                  : MAX_TEMPO_BPM;
}

void PatternEditorState::decreaseVolume() {
  speakerVolume_ = speakerVolume_ > VOLUME_INCREMENT
                       ? speakerVolume_ - VOLUME_INCREMENT
                       : 0;
}

void PatternEditorState::increaseVolume() {
  speakerVolume_ = speakerVolume_ < UINT8_MAX - VOLUME_INCREMENT
                       ? speakerVolume_ + VOLUME_INCREMENT
                       : UINT8_MAX;
}
