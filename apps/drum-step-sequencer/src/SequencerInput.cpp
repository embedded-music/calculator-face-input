#include "SequencerInput.h"

#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>

#include "CalculatorCommand.h"
#include "CalculatorLink.h"

namespace {
bool readCalculatorByte(uint8_t& value) {
  const uint8_t received =
      Wire.requestFrom(CALCULATOR_I2C_ADDRESS, uint8_t{1});
  if (received != 1 || !Wire.available()) return false;
  value = Wire.read();
  return true;
}
}  // namespace

bool SequencerInput::rescheduleStepClock(uint64_t nowUs) {
  const bool rescheduled = stepClock_.reschedule(
      nowUs, editor_.stepIntervalUs(), IntervalChangePolicy::PreservePhase);
  if (!rescheduled) Serial.println("clock: reschedule_failed");
  return rescheduled;
}

void SequencerInput::handleCalculatorValue(uint8_t value, uint64_t nowUs) {
  if (mode_ == UiMode::Settings) {
    bool changed = false;
    if (value == '%') {
      changed = editor_.decreaseVolume();
      M5.Speaker.setVolume(editor_.speakerVolume());
    } else if (value == '/') {
      changed = editor_.increaseVolume();
      M5.Speaker.setVolume(editor_.speakerVolume());
    } else if (value == '9') {
      changed = editor_.decreaseTempo();
    } else if (value == '*') {
      changed = editor_.increaseTempo();
    } else if (value == '6') {
      changed = editor_.decreaseRate();
    } else if (value == '-') {
      changed = editor_.increaseRate();
    } else {
      return;
    }
    if (changed && (value == '9' || value == '*' || value == '6' || value == '-')) {
      rescheduleStepClock(nowUs);
    }
    if (changed) view_.drawSettingsValues(editor_);
    return;
  }

  if (mode_ == UiMode::Sounds) {
    uint8_t soundIndex = 0;
    if (!soundIndexForCalculatorValue(value, soundIndex)) return;
    const uint8_t previousSoundIndex = editor_.selectedSoundIndex();
    editor_.selectSound(soundIndex);
    view_.drawSoundsSelection(editor_, previousSoundIndex);
    const DrumSound& sound = editor_.selectedSound();
    Serial.printf("editor: action=select_sound track=%u midi_note=%u name=%s\n",
                  editor_.selectedTrack() + 1, sound.midiNote, sound.name);
    return;
  }

  const CalculatorCommand command = commandForCalculatorValue(value);
  if (command.type == CalculatorCommandType::SelectTrack) {
    const uint8_t previousTrack = editor_.selectedTrack();
    editor_.selectTrack(command.index);
    M5.Display.startWrite();
    view_.drawTrack(editor_, previousTrack);
    if (editor_.selectedTrack() != previousTrack) {
      view_.drawTrack(editor_, editor_.selectedTrack());
    }
    M5.Display.endWrite();
    Serial.printf("editor: action=select_track track=%u\n",
                  editor_.selectedTrack() + 1);
    return;
  }
  if (command.type == CalculatorCommandType::ToggleStep) {
    const bool active = editor_.toggleStep(command.index);
    view_.drawStep(editor_, editor_.selectedTrack(), command.index);
    Serial.printf("editor: action=toggle_step track=%u step=%u active=%s\n",
                  editor_.selectedTrack() + 1, command.index + 1,
                  active ? "yes" : "no");
    return;
  }
  Serial.printf("editor: action=ignore value=0x%02X\n", value);
}

void SequencerInput::reportCoreButtons() {
  if (M5.BtnA.wasPressed()) {
    mode_ = UiMode::Settings;
    Serial.println("core_button: name=a action=pressed");
    view_.drawSettings(editor_);
  }
  if (M5.BtnA.wasReleased()) {
    mode_ = M5.BtnB.isPressed() ? UiMode::Sounds : UiMode::Pattern;
    Serial.println("core_button: name=a action=released");
    if (mode_ == UiMode::Sounds) view_.drawSounds(editor_);
    else view_.draw(editor_);
  }
  if (M5.BtnB.wasPressed()) {
    mode_ = M5.BtnA.isPressed() ? UiMode::Settings : UiMode::Sounds;
    Serial.println("core_button: name=b action=pressed");
    if (mode_ == UiMode::Settings) view_.drawSettings(editor_);
    else view_.drawSounds(editor_);
  }
  if (M5.BtnB.wasReleased()) {
    mode_ = M5.BtnA.isPressed() ? UiMode::Settings : UiMode::Pattern;
    Serial.println("core_button: name=b action=released");
    if (mode_ == UiMode::Settings) view_.drawSettings(editor_);
    else view_.draw(editor_);
  }
  if (M5.BtnC.wasPressed()) {
    Serial.println("core_button: name=c action=pressed");
  }
  if (M5.BtnC.wasReleased()) {
    Serial.println("core_button: name=c action=released");
  }
}

void SequencerInput::update(uint64_t nowUs) {
  reportCoreButtons();
  if (digitalRead(CALCULATOR_INTERRUPT_PIN) != LOW) return;
  uint8_t value = 0;
  if (readCalculatorByte(value)) handleCalculatorValue(value, nowUs);
  else Serial.println("calculator: read_error");
}
