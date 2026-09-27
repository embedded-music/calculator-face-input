#include "PatternEditorView.h"

#include "CalculatorKeyLayout.h"

namespace {
constexpr int16_t HEADER_HEIGHT = 34;
constexpr uint16_t COLOR_BACKGROUND = TFT_BLACK;
constexpr uint16_t COLOR_HEADER = 0x18C3;
constexpr uint16_t COLOR_GRID = 0x4208;
constexpr uint16_t COLOR_STEP_OFF = 0x2104;
constexpr uint16_t COLOR_STEP_ON = TFT_CYAN;
constexpr uint16_t COLOR_STEP_PENDING = 0x03EF;
constexpr uint16_t COLOR_SELECTED = TFT_YELLOW;
constexpr uint16_t COLOR_SELECTED_MUTED = 0x7BE0;
constexpr uint16_t COLOR_PLAYHEAD = TFT_MAGENTA;
constexpr uint16_t COLOR_TEXT = TFT_WHITE;
constexpr uint16_t COLOR_MUTED_TEXT = 0x8410;
constexpr uint16_t COLOR_VOLUME = 0x2DDF;
constexpr uint16_t COLOR_TEMPO = 0xFFE0;
constexpr uint16_t COLOR_RATE = 0xF81F;
constexpr uint16_t COLOR_SWING = 0xFD20;
constexpr const char* PATTERN_KEYS[] = {"AC", "M", "%", "/"};
}  // namespace

void PatternEditorView::drawSettingsLegends(const PatternEditorState& state) {
  constexpr int16_t LEGEND_Y = 204;
  constexpr int16_t LEGEND_WIDTH = 72;
  const uint16_t legendColors[] = {COLOR_VOLUME, COLOR_TEMPO, COLOR_RATE,
                                   COLOR_SWING};
  const char* legendLabels[] = {"VOLUME", "TEMPO", "RATE", "SWING"};
  for (uint8_t index = 0; index < 4; index++) {
    const int16_t x = 8 + index * 76;
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
    if (index == 3) {
      if (state.swingActive()) display_.printf("%u%%", state.swingPercent());
      else display_.print("--");
    }
  }
}

void PatternEditorView::drawSettings(const PatternEditorState& state) {
  constexpr int16_t KEY_X = 8, KEY_Y = 42, KEY_WIDTH = 74, KEY_HEIGHT = 32;
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
      if (row == 0 && column >= 2) fill = COLOR_VOLUME;
      if (row == 1 && column >= 2) fill = COLOR_TEMPO;
      if (row == 2 && column >= 2) fill = COLOR_RATE;
      if (row == 3 && column >= 2) fill = COLOR_SWING;
      const int16_t x = KEY_X + column * KEY_WIDTH;
      const int16_t y = KEY_Y + row * KEY_HEIGHT;
      display_.fillRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3, fill);
      display_.drawRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3,
                        fill == COLOR_STEP_OFF ? COLOR_GRID : COLOR_PLAYHEAD);
      display_.setTextColor(fill == COLOR_STEP_OFF ? COLOR_TEXT : COLOR_BACKGROUND,
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

void PatternEditorView::drawSoundCell(const PatternEditorState& state,
                                      uint8_t soundIndex) {
  constexpr int16_t KEY_X = 8, KEY_Y = 42, KEY_WIDTH = 74, KEY_HEIGHT = 27;
  const uint8_t row = soundIndex / 4;
  const uint8_t column = soundIndex % 4;
  const int16_t x = KEY_X + column * KEY_WIDTH;
  const int16_t y = KEY_Y + row * KEY_HEIGHT;
  const bool selected = soundIndex == state.selectedSoundIndex();
  const uint16_t fill = selected ? COLOR_SELECTED : COLOR_STEP_OFF;
  display_.fillRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3, fill);
  display_.drawRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3,
                    selected ? COLOR_PLAYHEAD : COLOR_GRID);
  display_.setTextSize(1);
  display_.setTextColor(selected ? COLOR_BACKGROUND : COLOR_TEXT, fill);
  display_.setCursor(x + 4, y + 5);
  display_.print(CALCULATOR_KEY_LABELS[row][column]);
  display_.setTextDatum(MC_DATUM);
  display_.drawString(DRUM_SOUNDS[soundIndex].displayName,
                      x + (KEY_WIDTH - 3) / 2, y + 18);
  display_.setTextDatum(TL_DATUM);
}

void PatternEditorView::drawSoundsFooter(const PatternEditorState& state) {
  constexpr int16_t FOOTER_Y = 184;
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
}

void PatternEditorView::drawSounds(const PatternEditorState& state) {
  display_.startWrite();
  display_.fillScreen(COLOR_BACKGROUND);
  display_.fillRect(0, 0, display_.width(), HEADER_HEIGHT, COLOR_HEADER);
  display_.setTextSize(2);
  display_.setTextColor(COLOR_TEXT, COLOR_HEADER);
  display_.setCursor(8, 8);
  display_.print("SOUNDS");
  for (uint8_t index = 0; index < DRUM_SOUND_COUNT; index++) drawSoundCell(state, index);
  drawSoundsFooter(state);
  display_.endWrite();
}

void PatternEditorView::drawSoundsSelection(const PatternEditorState& state,
                                            uint8_t previousSoundIndex) {
  display_.startWrite();
  if (previousSoundIndex < DRUM_SOUND_COUNT) drawSoundCell(state, previousSoundIndex);
  drawSoundCell(state, state.selectedSoundIndex());
  drawSoundsFooter(state);
  display_.endWrite();
}

void PatternEditorView::drawSoundsPatternChange(
    const PatternEditorState& state) {
  display_.startWrite();
  for (uint8_t index = 0; index < DRUM_SOUND_COUNT; index++) {
    drawSoundCell(state, index);
  }
  drawSoundsFooter(state);
  display_.endWrite();
}

void PatternEditorView::drawArrangement(const PatternEditorState& state) {
  display_.startWrite();
  display_.fillScreen(COLOR_BACKGROUND);
  display_.fillRect(0, 0, display_.width(), HEADER_HEIGHT, COLOR_HEADER);
  display_.setTextSize(2);
  display_.setTextColor(COLOR_TEXT, COLOR_HEADER);
  display_.setCursor(8, 8);
  display_.print("ARRANGEMENT");
  drawArrangementValuesContent(state);
  display_.endWrite();
}

void PatternEditorView::drawArrangementValuesContent(
    const PatternEditorState& state) {
  constexpr int16_t KEY_X = 8, KEY_Y = 42, KEY_WIDTH = 74, KEY_HEIGHT = 38;
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t column = 0; column < 4; column++) {
      const int16_t x = KEY_X + column * KEY_WIDTH;
      const int16_t y = KEY_Y + row * KEY_HEIGHT;
      uint16_t fill = COLOR_STEP_OFF;
      const char* label = CALCULATOR_KEY_LABELS[row][column];
      bool chainCell = false;
      uint8_t chainPosition = 0;
      if (row == 0) {
        fill = state.patternEmpty(column) ? COLOR_SELECTED_MUTED : COLOR_SELECTED;
        label = PATTERN_KEYS[column];
      } else if (row == 1 && column == 3) {
        label = state.cloneMode() ? "Clone ON" : "Clone OFF";
        if (state.cloneMode()) fill = COLOR_TEXT;
      } else if (row == 4 && column == 3) {
        label = "Clear";
      } else if (column < 3) {
        chainCell = true;
        chainPosition = static_cast<uint8_t>((row - 1) * 3 + column);
        const bool enabled = state.chainPositionEnabled(chainPosition);
        const bool pendingRemoval = !enabled && chainPosition == state.chainPosition();
        fill = enabled ? COLOR_STEP_ON
                       : pendingRemoval ? COLOR_STEP_PENDING : COLOR_STEP_OFF;
        label = PATTERN_KEYS[state.chainPatternAt(chainPosition)];
      }
      display_.fillRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3, fill);
      display_.drawRect(x, y, KEY_WIDTH - 3, KEY_HEIGHT - 3, COLOR_GRID);
      display_.setTextSize((row == 1 || row == 4) && column == 3 ? 1 : 2);
      display_.setTextColor(fill == COLOR_STEP_OFF ? COLOR_TEXT : COLOR_BACKGROUND, fill);
      display_.setTextDatum(MC_DATUM);
      display_.drawString(label, x + (KEY_WIDTH - 3) / 2, y + (KEY_HEIGHT - 3) / 2);
      display_.setTextDatum(TL_DATUM);
      if (chainCell && state.chainPositionVisible(chainPosition)) {
        const bool current = chainPosition == state.chainPosition();
        const bool next = chainPosition == state.nextChainPosition();
        if (current || next) {
          display_.fillRect(x + 3, y + KEY_HEIGHT - 8, KEY_WIDTH - 9, 4,
                            current ? COLOR_PLAYHEAD : COLOR_SELECTED);
        }
      }
    }
  }
}

void PatternEditorView::drawArrangementValues(const PatternEditorState& state) {
  display_.startWrite();
  drawArrangementValuesContent(state);
  display_.endWrite();
}
