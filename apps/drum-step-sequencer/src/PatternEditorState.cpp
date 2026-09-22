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
