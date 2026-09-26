#include "SequencerCommandRouter.h"

#include <Arduino.h>
#include <esp_timer.h>
#include <M5Unified.h>

#include "CalculatorCommand.h"

bool SequencerCommandRouter::rescheduleStepClock(uint64_t nowUs) {
  nowUs = static_cast<uint64_t>(esp_timer_get_time());
  const bool rescheduled = stepClock_.reschedule(
      nowUs, editor_.stepIntervalUs(), IntervalChangePolicy::PreservePhase);
  if (!rescheduled) Serial.println("clock: reschedule_failed");
  return rescheduled;
}

void SequencerCommandRouter::handleCalculatorValue(uint8_t value,
                                                   uint64_t nowUs) {
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

  if (mode_ == UiMode::Arrangement) {
    if (value == '*') {
      editor_.toggleCloneMode();
      view_.drawArrangementValues(editor_);
      Serial.printf("arrangement: action=clone_mode value=%s\n",
                    editor_.cloneMode() ? "on" : "off");
      return;
    }
    if (value == '=') {
      const uint8_t targetPattern = editor_.nextPattern();
      editor_.clearPattern(targetPattern);
      view_.drawArrangementValues(editor_);
      Serial.printf("arrangement: action=clear_pattern target=%u\n",
                    targetPattern + 1);
      return;
    }
    const uint8_t chainValues[] = {
        '7', '8', '9', '4', '5', '6', '1', '2', '3', '.', '0', '`'};
    for (uint8_t position = 0; position < CHAIN_MAX_LENGTH; position++) {
      if (chainValues[position] == value) {
        if (editor_.toggleChainPosition(position)) {
          view_.drawArrangementValues(editor_);
          Serial.printf(
              "arrangement: action=toggle_chain position=%u active=%s length=%u\n",
              position + 1,
              editor_.chainPositionEnabled(position) ? "yes" : "no",
              editor_.chainLength());
        }
        return;
      }
    }
    if (value == '*' || value == '-' || value == '+' || value == '=') return;
    uint8_t patternIndex = 0;
    if (!patternIndexForCalculatorValue(value, patternIndex)) return;
    if (editor_.cloneMode()) {
      editor_.cloneCurrentPatternTo(patternIndex);
      Serial.printf("arrangement: action=clone_pattern source=%u target=%u\n",
                    editor_.currentPattern() + 1, patternIndex + 1);
    }
    editor_.selectNextPattern(patternIndex);
    view_.drawArrangementValues(editor_);
    Serial.printf("arrangement: action=queue_pattern next=%u\n",
                  editor_.nextPattern() + 1);
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

void SequencerCommandRouter::handleCoreButton(SequencerInputEventType button) {
  if (button == SequencerInputEventType::ButtonA) {
    mode_ = mode_ == UiMode::Settings ? UiMode::Pattern : UiMode::Settings;
    Serial.println("core_button: name=a action=pressed");
    if (mode_ == UiMode::Settings) view_.drawSettings(editor_);
    else view_.draw(editor_);
  } else if (button == SequencerInputEventType::ButtonB) {
    mode_ = mode_ == UiMode::Sounds ? UiMode::Pattern : UiMode::Sounds;
    Serial.println("core_button: name=b action=pressed");
    if (mode_ == UiMode::Sounds) view_.drawSounds(editor_);
    else view_.draw(editor_);
  } else if (button == SequencerInputEventType::ButtonC) {
    mode_ = mode_ == UiMode::Arrangement ? UiMode::Pattern : UiMode::Arrangement;
    Serial.println("core_button: name=c action=pressed");
    if (mode_ == UiMode::Arrangement) view_.drawArrangement(editor_);
    else view_.draw(editor_);
  }
}

void SequencerCommandRouter::dispatch(const SequencerInputEvent& event,
                                      uint64_t nowUs) {
  if (event.type == SequencerInputEventType::CalculatorValue) {
    handleCalculatorValue(event.value, nowUs);
  } else {
    handleCoreButton(event.type);
  }
}
