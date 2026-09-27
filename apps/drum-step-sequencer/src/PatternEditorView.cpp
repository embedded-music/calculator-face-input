#include "PatternEditorView.h"

#include "CalculatorKeyLayout.h"

namespace {
constexpr int16_t HEADER_HEIGHT = 34;
constexpr int16_t LABEL_WIDTH = 34;
constexpr int16_t GRID_TOP = 46;
constexpr int16_t GRID_BOTTOM = 198;
constexpr int16_t GRID_RIGHT_MARGIN = 4;
constexpr uint16_t COLOR_BACKGROUND = TFT_BLACK;
constexpr uint16_t COLOR_HEADER = 0x18C3;
constexpr uint16_t COLOR_GRID = 0x4208;
constexpr uint16_t COLOR_STEP_OFF = 0x2104;
constexpr uint16_t COLOR_STEP_WEAK = 0x03EF;
constexpr uint16_t COLOR_STEP_NORMAL = 0x05FF;
constexpr uint16_t COLOR_STEP_STRONG = TFT_CYAN;
constexpr uint16_t COLOR_STEP_PENDING = 0x03EF;
constexpr uint16_t COLOR_SELECTED = TFT_YELLOW;
constexpr uint16_t COLOR_SELECTED_MUTED = 0x7BE0;
constexpr uint16_t COLOR_PLAYHEAD = TFT_MAGENTA;
constexpr uint16_t COLOR_TEXT = TFT_WHITE;
constexpr uint16_t COLOR_MUTED_TEXT = 0x8410;
constexpr uint16_t COLOR_VOLUME = 0x2DDF;
constexpr uint16_t COLOR_TEMPO = 0xFFE0;
constexpr uint16_t COLOR_RATE = 0xF81F;
constexpr const char* PATTERN_KEYS[] = {"AC", "M", "%", "/"};
}  // namespace

int16_t PatternEditorView::cellWidth() const {
  return (display_.width() - LABEL_WIDTH - GRID_RIGHT_MARGIN) / STEP_COUNT;
}

int16_t PatternEditorView::rowHeight() const {
  return (GRID_BOTTOM - GRID_TOP) / TRACK_COUNT;
}

void PatternEditorView::drawHeader(const PatternEditorState& state) {
  display_.fillRect(0, 0, display_.width(), HEADER_HEIGHT, COLOR_HEADER);
  display_.setTextSize(2);
  display_.setTextColor(COLOR_TEXT, COLOR_HEADER);
  display_.setCursor(8, 8);
  display_.print("DRUM STEPS");
  display_.setTextSize(1);
  display_.setCursor(display_.width() - 72, 12);
  display_.printf("PATTERN %s", PATTERN_KEYS[state.currentPattern()]);
}

void PatternEditorView::drawStep(const PatternEditorState& state,
                                 uint8_t track, uint8_t step) {
  const int16_t x = LABEL_WIDTH + step * cellWidth();
  const int16_t y = GRID_TOP + track * rowHeight();
  const uint16_t border = track == state.selectedTrack() ? COLOR_SELECTED
                                                         : COLOR_GRID;
  const StepLevel level = state.stepLevel(track, step);
  const uint16_t fill = level == StepLevel::Weak
                            ? COLOR_STEP_WEAK
                            : level == StepLevel::Strong
                                  ? COLOR_STEP_STRONG
                                  : level == StepLevel::Normal
                                        ? COLOR_STEP_NORMAL
                                        : COLOR_STEP_OFF;
  display_.fillRect(x + 2, y + 5, cellWidth() - 4, rowHeight() - 10, fill);
  display_.drawRect(x + 1, y + 4, cellWidth() - 2, rowHeight() - 8, border);
  if (step == state.currentStep()) drawPlayheadIndicator(state, track, step, true);
}

void PatternEditorView::drawPlayheadIndicator(const PatternEditorState& state,
                                               uint8_t track, uint8_t step,
                                               bool visible) {
  const int16_t x = LABEL_WIDTH + step * cellWidth();
  const int16_t y = GRID_TOP + track * rowHeight();
  const StepLevel level = state.stepLevel(track, step);
  const uint16_t fill = level == StepLevel::Weak
                            ? COLOR_STEP_WEAK
                            : level == StepLevel::Strong
                                  ? COLOR_STEP_STRONG
                                  : level == StepLevel::Normal
                                        ? COLOR_STEP_NORMAL
                                        : COLOR_STEP_OFF;
  display_.fillRect(x + 3, y + rowHeight() - 8, cellWidth() - 6, 3,
                    visible ? COLOR_PLAYHEAD : fill);
}

void PatternEditorView::drawPlayheadChange(const PatternEditorState& state,
                                           uint8_t previousStep) {
  display_.startWrite();
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    drawPlayheadIndicator(state, track, previousStep, false);
    drawPlayheadIndicator(state, track, state.currentStep(), true);
  }
  display_.endWrite();
}

void PatternEditorView::drawTrack(const PatternEditorState& state,
                                  uint8_t track) {
  constexpr const char* TRACK_KEYS[] = {"AC", "M", "%", "/"};
  const int16_t y = GRID_TOP + track * rowHeight();
  const bool selected = track == state.selectedTrack();
  display_.fillRect(0, y, LABEL_WIDTH, rowHeight(), COLOR_BACKGROUND);
  if (selected) {
    display_.fillRect(3, y + 7, LABEL_WIDTH - 8, rowHeight() - 14,
                      COLOR_SELECTED);
  }
  display_.setTextSize(2);
  display_.setTextColor(selected ? COLOR_BACKGROUND : COLOR_TEXT,
                        selected ? COLOR_SELECTED : COLOR_BACKGROUND);
  display_.setCursor(7, y + 10);
  display_.print(TRACK_KEYS[track]);
  for (uint8_t step = 0; step < STEP_COUNT; step++) drawStep(state, track, step);
}

void PatternEditorView::drawBeatMarkers() {
  display_.setTextSize(1);
  display_.setTextColor(COLOR_MUTED_TEXT, COLOR_BACKGROUND);
  for (uint8_t beat = 0; beat < 4; beat++) {
    display_.setCursor(LABEL_WIDTH + beat * 4 * cellWidth() + 3, GRID_TOP - 10);
    display_.printf("%u", beat + 1);
  }
}

void PatternEditorView::drawFooter(const PatternEditorState& state) {
  display_.fillRect(0, GRID_BOTTOM, display_.width(),
                    display_.height() - GRID_BOTTOM, COLOR_BACKGROUND);
  display_.setTextSize(1);
  display_.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  display_.setCursor(7, 207);
  display_.setTextSize(2);
  display_.setCursor(7, 209);
  display_.printf("BPM %u  %s  vol %u%%", state.tempoBpm(),
                  state.stepRateName(),
                  (static_cast<uint16_t>(state.speakerVolume()) * 100) / 255);
}

void PatternEditorView::draw(const PatternEditorState& state) {
  display_.startWrite();
  display_.fillScreen(COLOR_BACKGROUND);
  drawHeader(state);
  drawBeatMarkers();
  for (uint8_t track = 0; track < TRACK_COUNT; track++) drawTrack(state, track);
  drawFooter(state);
  display_.endWrite();
}

void PatternEditorView::drawPatternChange(const PatternEditorState& state) {
  display_.startWrite();
  drawHeader(state);
  drawBeatMarkers();
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    drawTrack(state, track);
  }
  drawFooter(state);
  display_.endWrite();
}
