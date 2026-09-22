#pragma once

#include <M5Unified.h>

#include "PatternEditorState.h"

class PatternEditorView {
 public:
  explicit PatternEditorView(M5GFX& display) : display_(display) {}
  void draw(const PatternEditorState& state);
  void drawTrack(const PatternEditorState& state, uint8_t track);
  void drawStep(const PatternEditorState& state, uint8_t track, uint8_t step);
  void drawPlayheadChange(const PatternEditorState& state,
                          uint8_t previousStep);

 private:
  int16_t cellWidth() const;
  int16_t rowHeight() const;
  void drawHeader();
  void drawBeatMarkers();
  void drawFooter();

  M5GFX& display_;
};
