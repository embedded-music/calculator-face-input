#include "SequencerInput.h"

#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>

#include "CalculatorLink.h"
#include "SequencerInputEvent.h"

namespace {
bool readCalculatorByte(uint8_t& value) {
  const uint8_t received = Wire.requestFrom(CALCULATOR_I2C_ADDRESS, uint8_t{1});
  if (received != 1 || !Wire.available()) return false;
  value = Wire.read();
  return true;
}
}  // namespace

void SequencerInput::reportCoreButtons() {
  if (M5.BtnA.wasPressed()) {
    buttonA_.press();
  }
  if (M5.BtnB.wasPressed()) {
    buttonB_.press();
  }
  if (M5.BtnC.wasPressed()) {
    router_.dispatch({SequencerInputEventType::ButtonC}, 0);
  }
}

void SequencerInput::reportPendingCoreButtonReleases() {
  if (M5.BtnA.wasReleased()) {
    if (buttonA_.releaseAsTap()) {
      router_.dispatch({SequencerInputEventType::ButtonA}, 0);
    }
  }
  if (M5.BtnB.wasReleased()) {
    if (buttonB_.releaseAsTap()) {
      router_.dispatch({SequencerInputEventType::ButtonB}, 0);
    }
  }
}

void SequencerInput::update(uint64_t nowUs) {
  reportCoreButtons();
  if (digitalRead(CALCULATOR_INTERRUPT_PIN) != LOW) {
    reportPendingCoreButtonReleases();
    return;
  }
  uint8_t value = 0;
  if (readCalculatorByte(value)) {
    SequencerInputEvent event{SequencerInputEventType::CalculatorValue, value};
    event.modifierA = buttonA_.modifierActive(M5.BtnA.isPressed());
    event.modifierB = buttonB_.modifierActive(M5.BtnB.isPressed());
    if (event.modifierA) buttonA_.consumeAsModifier();
    if (event.modifierB) buttonB_.consumeAsModifier();
    router_.dispatch(event, nowUs);
  } else {
    Serial.println("calculator: read_error");
  }
  reportPendingCoreButtonReleases();
}
