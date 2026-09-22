#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>

namespace {
constexpr uint8_t CALCULATOR_I2C_ADDRESS = 0x08;
constexpr uint8_t CALCULATOR_INTERRUPT_PIN = 5;
constexpr uint32_t I2C_FREQUENCY_HZ = 100000;

constexpr uint8_t TRACK_COUNT = 4;
constexpr uint8_t STEP_COUNT = 16;

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
constexpr uint16_t COLOR_TEXT = TFT_WHITE;
constexpr uint16_t COLOR_MUTED_TEXT = 0x8410;

struct PatternEditorState {
  bool steps[TRACK_COUNT][STEP_COUNT]{};
  uint8_t selectedTrack = 0;
};

PatternEditorState editor;

int16_t cellWidth() {
  return (M5.Display.width() - LABEL_WIDTH - GRID_RIGHT_MARGIN) / STEP_COUNT;
}

int16_t rowHeight() {
  return (GRID_BOTTOM - GRID_TOP) / TRACK_COUNT;
}

void drawHeader() {
  M5.Display.fillRect(0, 0, M5.Display.width(), HEADER_HEIGHT, COLOR_HEADER);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(COLOR_TEXT, COLOR_HEADER);
  M5.Display.setCursor(8, 8);
  M5.Display.print("DRUM STEPS");

  M5.Display.setTextSize(1);
  M5.Display.setCursor(M5.Display.width() - 66, 12);
  M5.Display.print("PATTERN 1");
}

void drawStep(uint8_t track, uint8_t step) {
  const int16_t x = LABEL_WIDTH + step * cellWidth();
  const int16_t y = GRID_TOP + track * rowHeight();
  const bool selected = track == editor.selectedTrack;
  const uint16_t border = selected ? COLOR_SELECTED : COLOR_GRID;
  const uint16_t fill = editor.steps[track][step] ? COLOR_STEP_ON
                                                  : COLOR_STEP_OFF;

  M5.Display.fillRect(x + 2, y + 5, cellWidth() - 4, rowHeight() - 10, fill);
  M5.Display.drawRect(x + 1, y + 4, cellWidth() - 2, rowHeight() - 8, border);
}

void drawTrack(uint8_t track) {
  const int16_t y = GRID_TOP + track * rowHeight();
  const bool selected = track == editor.selectedTrack;

  M5.Display.fillRect(0, y, LABEL_WIDTH, rowHeight(), COLOR_BACKGROUND);
  if (selected) {
    M5.Display.fillRect(3, y + 7, LABEL_WIDTH - 8, rowHeight() - 14,
                       COLOR_SELECTED);
  }
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(selected ? COLOR_BACKGROUND : COLOR_TEXT,
                          selected ? COLOR_SELECTED : COLOR_BACKGROUND);
  M5.Display.setCursor(9, y + 14);
  M5.Display.printf("T%u", track + 1);

  for (uint8_t step = 0; step < STEP_COUNT; step++) {
    drawStep(track, step);
  }
}

void drawBeatMarkers() {
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(COLOR_MUTED_TEXT, COLOR_BACKGROUND);
  for (uint8_t beat = 0; beat < 4; beat++) {
    const int16_t x = LABEL_WIDTH + beat * 4 * cellWidth() + 3;
    M5.Display.setCursor(x, GRID_TOP - 10);
    M5.Display.printf("%u", beat + 1);
  }
}

void drawFooter() {
  M5.Display.fillRect(0, GRID_BOTTOM, M5.Display.width(),
                     M5.Display.height() - GRID_BOTTOM, COLOR_BACKGROUND);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
  M5.Display.setCursor(7, 207);
  M5.Display.print("AC M % /: track   grid: toggle step");
  M5.Display.setTextColor(COLOR_MUTED_TEXT, COLOR_BACKGROUND);
  M5.Display.setCursor(7, 222);
  M5.Display.print("Core A/B/C reserved for later slices");
}

void drawEditor() {
  M5.Display.startWrite();
  M5.Display.fillScreen(COLOR_BACKGROUND);
  drawHeader();
  drawBeatMarkers();
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    drawTrack(track);
  }
  drawFooter();
  M5.Display.endWrite();
}

bool calculatorAcknowledges() {
  Wire.beginTransmission(CALCULATOR_I2C_ADDRESS);
  return Wire.endTransmission() == 0;
}

bool readCalculatorByte(uint8_t& value) {
  const uint8_t received =
      Wire.requestFrom(CALCULATOR_I2C_ADDRESS, uint8_t{1});
  if (received != 1 || !Wire.available()) return false;
  value = Wire.read();
  return true;
}

int8_t trackForValue(uint8_t value) {
  constexpr uint8_t values[TRACK_COUNT] = {'A', 'M', '%', '/'};
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    if (values[track] == value) return track;
  }
  return -1;
}

int8_t stepForValue(uint8_t value) {
  constexpr uint8_t values[STEP_COUNT] = {
      '7', '8', '9', '*', '4', '5', '6', '-',
      '1', '2', '3', '+', '.', '0', '`', '='};
  for (uint8_t step = 0; step < STEP_COUNT; step++) {
    if (values[step] == value) return step;
  }
  return -1;
}

void handleCalculatorValue(uint8_t value) {
  const int8_t track = trackForValue(value);
  if (track >= 0) {
    const uint8_t previousTrack = editor.selectedTrack;
    editor.selectedTrack = track;
    M5.Display.startWrite();
    drawTrack(previousTrack);
    if (editor.selectedTrack != previousTrack) drawTrack(editor.selectedTrack);
    M5.Display.endWrite();
    Serial.printf("editor: action=select_track track=%u\n",
                  editor.selectedTrack + 1);
    return;
  }

  const int8_t step = stepForValue(value);
  if (step >= 0) {
    bool& active = editor.steps[editor.selectedTrack][step];
    active = !active;
    drawStep(editor.selectedTrack, step);
    Serial.printf("editor: action=toggle_step track=%u step=%u active=%s\n",
                  editor.selectedTrack + 1,
                  step + 1,
                  active ? "yes" : "no");
    return;
  }

  Serial.printf("editor: action=ignore value=0x%02X\n", value);
}

void reportCoreButtons() {
  if (M5.BtnA.wasPressed()) Serial.println("core_button: name=a action=pressed");
  if (M5.BtnA.wasReleased()) Serial.println("core_button: name=a action=released");
  if (M5.BtnB.wasPressed()) Serial.println("core_button: name=b action=pressed");
  if (M5.BtnB.wasReleased()) Serial.println("core_button: name=b action=released");
  if (M5.BtnC.wasPressed()) Serial.println("core_button: name=c action=pressed");
  if (M5.BtnC.wasReleased()) Serial.println("core_button: name=c action=released");
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  auto config = M5.config();
  config.internal_spk = false;
  config.internal_mic = false;
  M5.begin(config);
  M5.Display.setRotation(1);

  pinMode(CALCULATOR_INTERRUPT_PIN, INPUT_PULLUP);
  Wire.begin(21, 22, I2C_FREQUENCY_HZ);

  drawEditor();
  Serial.printf("drum_step_sequencer: calculator_detected=%s\n",
                calculatorAcknowledges() ? "yes" : "no");
}

void loop() {
  M5.update();
  reportCoreButtons();

  if (digitalRead(CALCULATOR_INTERRUPT_PIN) == LOW) {
    uint8_t value = 0;
    if (readCalculatorByte(value)) {
      handleCalculatorValue(value);
    } else {
      Serial.println("calculator: read_error");
    }
  }

  delay(1);
}
