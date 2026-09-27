#include "SequencerCommandRouter.h"

#include <Arduino.h>
#include <esp_timer.h>
#include <M5Unified.h>

#include "SequencerCommandMap.h"

bool SequencerCommandRouter::rescheduleStepClock(uint64_t nowUs) {
  nowUs = static_cast<uint64_t>(esp_timer_get_time());
  const bool rescheduled = stepClock_.reconfigure(
      nowUs, editor_.stepIntervalUs(), editor_.swingPercent(),
      editor_.swingActive());
  if (!rescheduled) Serial.println("clock: reschedule_failed");
  return rescheduled;
}

void SequencerCommandRouter::handleCalculatorValue(
    const SequencerInputEvent& event, uint64_t nowUs) {
  const SequencerCommand command = commandForMode(mode_, event.value);
  if (mode_ == UiMode::Settings) {
    bool changed = false;
    if (command.action == SequencerAction::VolumeDown) {
      changed = editor_.decreaseVolume();
      M5.Speaker.setVolume(editor_.speakerVolume());
    } else if (command.action == SequencerAction::VolumeUp) {
      changed = editor_.increaseVolume();
      M5.Speaker.setVolume(editor_.speakerVolume());
    } else if (command.action == SequencerAction::TempoDown) {
      changed = editor_.decreaseTempo();
    } else if (command.action == SequencerAction::TempoUp) {
      changed = editor_.increaseTempo();
    } else if (command.action == SequencerAction::RateDown) {
      changed = editor_.decreaseRate();
    } else if (command.action == SequencerAction::RateUp) {
      changed = editor_.increaseRate();
    } else if (command.action == SequencerAction::SwingDown) {
      changed = editor_.decreaseSwing();
    } else if (command.action == SequencerAction::SwingUp) {
      changed = editor_.increaseSwing();
    } else {
      return;
    }
    if (changed && command.action != SequencerAction::VolumeDown &&
        command.action != SequencerAction::VolumeUp) {
      rescheduleStepClock(nowUs);
    }
    if (changed) view_.drawSettingsValues(editor_);
    return;
  }

  if (mode_ == UiMode::Sounds) {
    if (command.action != SequencerAction::SelectSound) return;
    const uint8_t previousSoundIndex = editor_.selectedSoundIndex();
    editor_.selectSound(command.index);
    view_.drawSoundsSelection(editor_, previousSoundIndex);
    const DrumSound& sound = editor_.selectedSound();
    Serial.printf("editor: action=select_sound track=%u midi_note=%u name=%s\n",
                  editor_.selectedTrack() + 1, sound.midiNote, sound.name);
    return;
  }

  if (mode_ == UiMode::Arrangement) {
    if (command.action == SequencerAction::ToggleClone) {
      editor_.toggleCloneMode();
      view_.drawArrangementValues(editor_);
      Serial.printf("arrangement: action=clone_mode value=%s\n",
                    editor_.cloneMode() ? "on" : "off");
      return;
    }
    if (command.action == SequencerAction::ClearPattern) {
      const uint8_t targetPattern = editor_.nextPattern();
      editor_.clearPattern(targetPattern);
      view_.drawArrangementValues(editor_);
      Serial.printf("arrangement: action=clear_pattern target=%u\n",
                    targetPattern + 1);
      return;
    }
    if (command.action == SequencerAction::ToggleChainPosition) {
        if (editor_.toggleChainPosition(command.index)) {
          view_.drawArrangementValues(editor_);
          Serial.printf(
              "arrangement: action=toggle_chain position=%u active=%s length=%u\n",
              command.index + 1,
              editor_.chainPositionEnabled(command.index) ? "yes" : "no",
              editor_.chainLength());
        }
        return;
    }
    if (command.action != SequencerAction::SelectPattern) return;
    const uint8_t patternIndex = command.index;
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

  if (command.action == SequencerAction::SelectTrack) {
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
  if (command.action == SequencerAction::ToggleStep) {
    const StepLevel level =
        event.modifierA == event.modifierB
            ? StepLevel::Normal
            : event.modifierA ? StepLevel::Weak : StepLevel::Strong;
    const bool modified = event.modifierA || event.modifierB;
    const bool active = modified
                            ? editor_.setStepLevel(command.index, level)
                            : editor_.toggleStep(command.index);
    view_.drawStep(editor_, editor_.selectedTrack(), command.index);
    Serial.printf("editor: action=toggle_step track=%u step=%u active=%s level=%u\n",
                  editor_.selectedTrack() + 1, command.index + 1,
                  active ? "yes" : "no", static_cast<unsigned>(level));
    return;
  }
  Serial.printf("editor: action=ignore value=0x%02X\n", event.value);
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
    handleCalculatorValue(event, nowUs);
  } else {
    handleCoreButton(event.type);
  }
}
