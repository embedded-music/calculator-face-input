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
constexpr uint16_t COLOR_STEP_ON = TFT_CYAN;
constexpr uint16_t COLOR_SELECTED = TFT_YELLOW;
constexpr uint16_t COLOR_PLAYHEAD = TFT_MAGENTA;
constexpr uint16_t COLOR_TEXT = TFT_WHITE;
constexpr uint16_t COLOR_MUTED_TEXT = 0x8410;
constexpr uint16_t COLOR_VOLUME = 0x2DDF;
constexpr uint16_t COLOR_TEMPO = 0xFFE0;
constexpr uint16_t COLOR_RATE = 0xF81F;
}  // namespace

int16_t PatternEditorView::cellWidth() const {
  return (display_.width() - LABEL_WIDTH - GRID_RIGHT_MARGIN) / STEP_COUNT;
}

int16_t PatternEditorView::rowHeight() const {
  return (GRID_BOTTOM - GRID_TOP) / TRACK_COUNT;
}

void PatternEditorView::drawHeader() {
  display_.fillRect(0, 0, display_.width(), HEADER_HEIGHT, COLOR_HEADER);
  display_.setTextSize(2);
  display_.setTextColor(COLOR_TEXT, COLOR_HEADER);
  display_.setCursor(8, 8);
  display_.print("DRUM STEPS");
  display_.setTextSize(1);
  display_.setCursor(display_.width() - 66, 12);
  display_.print("PATTERN 1");
}

void PatternEditorView::drawStep(const PatternEditorState& state,
                                 uint8_t track, uint8_t step) {
  const int16_t x = LABEL_WIDTH + step * cellWidth();
  const int16_t y = GRID_TOP + track * rowHeight();
  const uint16_t border = track == state.selectedTrack() ? COLOR_SELECTED
                                                         : COLOR_GRID;
  const uint16_t fill = state.stepActive(track, step) ? COLOR_STEP_ON
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
  const uint16_t fill = state.stepActive(track, step) ? COLOR_STEP_ON
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
  drawHeader();
  drawBeatMarkers();
  for (uint8_t track = 0; track < TRACK_COUNT; track++) drawTrack(state, track);
  drawFooter(state);
  display_.endWrite();
}

void PatternEditorView::drawSettingsLegends(const PatternEditorState& state) {
  constexpr int16_t LEGEND_Y = 204;
  constexpr int16_t LEGEND_WIDTH = 96;
  const uint16_t legendColors[] = {COLOR_VOLUME, COLOR_TEMPO, COLOR_RATE};
  const char* legendLabels[] = {"VOLUME", "TEMPO", "RATE"};
  for (uint8_t index = 0; index < 3; index++) {
    const int16_t x = 8 + index * 104;
    display_.fillRect(x, LEGEND_Y, LEGEND_WIDTH, 29, legendColors[index]);
    display_.setTextColor(COLOR_BACKGROUND, legendColors[index]);
    display_.setTextSize(1);
    display_.setCursor(x + 5, LEGEND_Y + 4);
    display_.print(legendLabels[index]);
    display_.setTextSize(2);
    display_.setCursor(x + 5, LEGEND_Y + 14);
    if (index == 0) display_.printf("%u", state.speakerVolume());
    if (index == 1) display_.printf("%u", state.tempoBpm());
    if (index == 2) display_.print(state.stepRateName());
  }
}

void PatternEditorView::drawSettings(const PatternEditorState& state) {
  constexpr int16_t KEY_X = 8;
  constexpr int16_t KEY_Y = 42;
  constexpr int16_t KEY_WIDTH = 74;
  constexpr int16_t KEY_HEIGHT = 32;

  display_.startWrite();
  display_.fillScreen(COLOR_BACKGROUND);
  display_.fillRect(0, 0, display_.width(), HEADER_HEIGHT, COLOR_HEADER);
  display_.setTextSize(2);
  display_.setTextColor(COLOR_TEXT, COLOR_HEADER);
  display_.setCursor(8, 8);
  display_.print("SETTINGS");

  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t column = 0; column < 4; column++) {
      uint16_t fill = COLOR_STEP_OFF;
      const char* key = CALCULATOR_KEY_LABELS[row][column];
      if ((row == 0 && (column == 2 || column == 3))) fill = COLOR_VOLUME;
      if ((row == 1 && (column == 2 || column == 3))) fill = COLOR_TEMPO;
      if ((row == 2 && (column == 2 || column == 3))) fill = COLOR_RATE;

      const int16_t x = KEY_X + column * KEY_WIDTH;
      const int16_t y = KEY_Y + row * KEY_HEIGHT;
      display_.fillRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3, fill);
      display_.drawRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3,
                        fill == COLOR_STEP_OFF ? COLOR_GRID : COLOR_PLAYHEAD);
      display_.setTextColor(fill == COLOR_STEP_OFF ? COLOR_TEXT
                                                   : COLOR_BACKGROUND,
                            fill);
      display_.setTextSize(2);
      display_.setCursor(x + 31, y + 5);
      display_.print(key);
    }
  }

  drawSettingsLegends(state);

  display_.endWrite();
}

void PatternEditorView::drawSettingsValues(const PatternEditorState& state) {
  display_.startWrite();
  drawSettingsLegends(state);
  display_.endWrite();
}

void PatternEditorView::drawSounds(const PatternEditorState& state) {
  constexpr int16_t KEY_X = 8;
  constexpr int16_t KEY_Y = 42;
  constexpr int16_t KEY_WIDTH = 74;
  constexpr int16_t KEY_HEIGHT = 27;
  constexpr int16_t FOOTER_Y = 184;

  display_.startWrite();
  display_.fillScreen(COLOR_BACKGROUND);
  display_.fillRect(0, 0, display_.width(), HEADER_HEIGHT, COLOR_HEADER);
  display_.setTextSize(2);
  display_.setTextColor(COLOR_TEXT, COLOR_HEADER);
  display_.setCursor(8, 8);
  display_.print("SOUNDS");

  for (uint8_t index = 0; index < DRUM_SOUND_COUNT; index++) {
    const uint8_t row = index / 4;
    const uint8_t column = index % 4;
    const int16_t x = KEY_X + column * KEY_WIDTH;
    const int16_t y = KEY_Y + row * KEY_HEIGHT;
    const bool selected = index == state.selectedSoundIndex();
    const uint16_t fill = selected ? COLOR_SELECTED : COLOR_STEP_OFF;
    display_.fillRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3, fill);
    display_.drawRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3,
                      selected ? COLOR_PLAYHEAD : COLOR_GRID);
    display_.setTextSize(1);
    display_.setTextColor(selected ? COLOR_BACKGROUND : COLOR_TEXT, fill);
    display_.setCursor(x + 4, y + 5);
    display_.print(CALCULATOR_KEY_LABELS[row][column]);
    display_.setCursor(x + (KEY_WIDTH - 3) / 2, y + 19);
    display_.setTextColor(selected ? COLOR_BACKGROUND : COLOR_TEXT, fill);
    display_.setTextDatum(MC_DATUM);
    display_.drawString(DRUM_SOUNDS[index].displayName,
                        x + (KEY_WIDTH - 3) / 2,
                        y + 18);
    display_.setTextDatum(TL_DATUM);
  }

  display_.fillRect(0, FOOTER_Y, display_.width(),
                    display_.height() - FOOTER_Y, COLOR_BACKGROUND);
  display_.setTextColor(COLOR_MUTED_TEXT, COLOR_BACKGROUND);
  display_.setTextSize(1);
  display_.setCursor(8, 198);
  display_.printf("TRACK %u   CURRENT", state.selectedTrack() + 1);
  display_.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  display_.setTextSize(2);
  display_.setCursor(8, 207);
  display_.print(state.selectedSound().name);
  display_.endWrite();
}
