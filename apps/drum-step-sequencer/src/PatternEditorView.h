#pragma once

#include <M5Unified.h>

#include "PatternEditorState.h"

class PatternEditorView {
 public:
  explicit PatternEditorView(M5GFX& display) : display_(display) {}
  void draw(const PatternEditorState& state);
  void drawSettings(const PatternEditorState& state);
  void drawSettingsValues(const PatternEditorState& state);
  void drawSounds(const PatternEditorState& state);
  void drawSoundsSelection(const PatternEditorState& state,
                           uint8_t previousSoundIndex);
  void drawArrangement(const PatternEditorState& state);
  void drawTrack(const PatternEditorState& state, uint8_t track);
  void drawStep(const PatternEditorState& state, uint8_t track, uint8_t step);
  void drawPlayheadChange(const PatternEditorState& state,
                          uint8_t previousStep);
  void drawFooter(const PatternEditorState& state);

 private:
  int16_t cellWidth() const;
  int16_t rowHeight() const;
  void drawHeader(const PatternEditorState& state);
  void drawBeatMarkers();
  void drawSettingsLegends(const PatternEditorState& state);
  void drawSoundCell(const PatternEditorState& state, uint8_t soundIndex);
  void drawSoundsFooter(const PatternEditorState& state);
  void drawPlayheadIndicator(const PatternEditorState& state, uint8_t track,
                             uint8_t step, bool visible);

  M5GFX& display_;
};
