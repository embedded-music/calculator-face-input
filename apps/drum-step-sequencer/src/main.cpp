#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>

#include "AmyAudioActivityGate.h"
#include "AmyM5SpeakerBridge.h"
#include "AmySynthSlot.h"
#include "CalculatorCommand.h"
#include "PatternEditorState.h"
#include "PatternEditorView.h"
#include "StepClock.h"

namespace {
constexpr uint8_t CALCULATOR_I2C_ADDRESS = 0x08;
constexpr uint8_t CALCULATOR_INTERRUPT_PIN = 5;
constexpr uint32_t I2C_FREQUENCY_HZ = 100000;
constexpr uint8_t AMY_SYNTH_ID = 1;
constexpr uint8_t AMY_DRUM_VOICES = 1;
constexpr uint16_t AMY_GM_DRUM_PATCH = 258;
constexpr float DRUM_VELOCITY = 1.0f;
constexpr uint32_t DRUM_TAIL_MS = 1500;

PatternEditorState editor;
PatternEditorView view(M5.Display);
StepClock stepClock(60000 / (120 * 4));
AmyM5SpeakerBridge amyBridge;
AmyAudioActivityGate audioGate(amyBridge);
AmySynthSlot drumSlot;

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

void handleCalculatorValue(uint8_t value) {
  if (M5.BtnA.isPressed()) {
    if (value == '-') {
      editor.decreaseVolume();
      M5.Speaker.setVolume(editor.speakerVolume());
    } else if (value == '+') {
      editor.increaseVolume();
      M5.Speaker.setVolume(editor.speakerVolume());
    } else if (value == '/') {
      editor.decreaseTempo();
      stepClock.setInterval(60000UL / (editor.tempoBpm() * 4UL));
    } else if (value == '*') {
      editor.increaseTempo();
      stepClock.setInterval(60000UL / (editor.tempoBpm() * 4UL));
    } else {
      return;
    }
    view.drawFooter(editor, ControlLayer::Settings);
    return;
  }

  if (M5.BtnB.isPressed()) {
    const CalculatorCommand command = commandForCalculatorValue(value);
    uint8_t soundIndex = DRUM_SOUND_COUNT;
    if (command.type == CalculatorCommandType::SelectTrack) {
      soundIndex = command.index;
    } else if (command.type == CalculatorCommandType::ToggleStep) {
      soundIndex = TRACK_COUNT + command.index;
    }
    if (soundIndex >= DRUM_SOUND_COUNT) return;
    editor.selectSound(soundIndex);
    view.drawFooter(editor, ControlLayer::Sound);
    const DrumSound& sound = editor.selectedSound();
    Serial.printf("editor: action=select_sound track=%u midi_note=%u name=%s\n",
                  editor.selectedTrack() + 1, sound.midiNote, sound.name);
    return;
  }

  if (M5.BtnC.isPressed()) return;

  const CalculatorCommand command = commandForCalculatorValue(value);
  if (command.type == CalculatorCommandType::SelectTrack) {
    const uint8_t previousTrack = editor.selectedTrack();
    editor.selectTrack(command.index);
    M5.Display.startWrite();
    view.drawTrack(editor, previousTrack);
    if (editor.selectedTrack() != previousTrack) {
      view.drawTrack(editor, editor.selectedTrack());
    }
    M5.Display.endWrite();
    Serial.printf("editor: action=select_track track=%u\n",
                  editor.selectedTrack() + 1);
    return;
  }
  if (command.type == CalculatorCommandType::ToggleStep) {
    const bool active = editor.toggleStep(command.index);
    view.drawStep(editor, editor.selectedTrack(), command.index);
    Serial.printf("editor: action=toggle_step track=%u step=%u active=%s\n",
                  editor.selectedTrack() + 1, command.index + 1,
                  active ? "yes" : "no");
    return;
  }
  Serial.printf("editor: action=ignore value=0x%02X\n", value);
}

void reportCoreButtons() {
  if (M5.BtnA.wasPressed()) {
    Serial.println("core_button: name=a action=pressed");
    view.drawFooter(editor, ControlLayer::Settings);
  }
  if (M5.BtnA.wasReleased()) {
    Serial.println("core_button: name=a action=released");
    view.drawFooter(editor, ControlLayer::Default);
  }
  if (M5.BtnB.wasPressed()) {
    Serial.println("core_button: name=b action=pressed");
    view.drawFooter(editor, ControlLayer::Sound);
  }
  if (M5.BtnB.wasReleased()) {
    Serial.println("core_button: name=b action=released");
    view.drawFooter(editor, M5.BtnA.isPressed() ? ControlLayer::Settings
                                                : ControlLayer::Default);
  }
  if (M5.BtnC.wasPressed()) Serial.println("core_button: name=c action=pressed");
  if (M5.BtnC.wasReleased()) {
    Serial.println("core_button: name=c action=released");
    view.drawFooter(editor, M5.BtnA.isPressed() ? ControlLayer::Settings
                                                : ControlLayer::Default);
  }
}

void triggerCurrentStep() {
  bool hasTrigger = false;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    if (editor.stepActive(track, editor.currentStep())) {
      if (!hasTrigger) audioGate.wake(DRUM_TAIL_MS);
      drumSlot.noteOn(editor.soundForTrack(track).midiNote, DRUM_VELOCITY);
      hasTrigger = true;
    }
  }
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);
  auto config = M5.config();
  config.internal_spk = true;
  config.internal_mic = false;
  M5.begin(config);
  M5.Display.setRotation(1);
  pinMode(CALCULATOR_INTERRUPT_PIN, INPUT_PULLUP);
  Wire.begin(21, 22, I2C_FREQUENCY_HZ);
  view.draw(editor);
  amyBridge.begin();
  M5.Speaker.setVolume(editor.speakerVolume());
  drumSlot.begin(AMY_SYNTH_ID, AMY_DRUM_VOICES, AMY_GM_DRUM_PATCH);
  stepClock.begin(millis());
  Serial.printf("drum_step_sequencer: calculator_detected=%s\n",
                calculatorAcknowledges() ? "yes" : "no");
}

void loop() {
  M5.update();
  reportCoreButtons();

  const uint8_t elapsedSteps = stepClock.elapsedSteps(millis());
  if (elapsedSteps > 0) {
    const uint8_t previousStep = editor.currentStep();
    for (uint8_t count = 0; count < elapsedSteps; count++) editor.advanceStep();
    triggerCurrentStep();
    view.drawPlayheadChange(editor, previousStep);
  }

  if (digitalRead(CALCULATOR_INTERRUPT_PIN) == LOW) {
    uint8_t value = 0;
    if (readCalculatorByte(value)) handleCalculatorValue(value);
    else Serial.println("calculator: read_error");
  }
  audioGate.update(false);
  delay(1);
}
