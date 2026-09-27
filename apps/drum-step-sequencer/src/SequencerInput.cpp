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
    pendingButtonA_ = true;
  }
  if (M5.BtnB.wasPressed()) {
    pendingButtonB_ = true;
  }
  if (M5.BtnC.wasPressed()) {
    router_.dispatch({SequencerInputEventType::ButtonC}, 0);
  }
}

void SequencerInput::reportPendingCoreButtonReleases() {
  if (pendingButtonA_ && M5.BtnA.wasReleased()) {
    pendingButtonA_ = false;
    router_.dispatch({SequencerInputEventType::ButtonA}, 0);
  }
  if (pendingButtonB_ && M5.BtnB.wasReleased()) {
    pendingButtonB_ = false;
    router_.dispatch({SequencerInputEventType::ButtonB}, 0);
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
    event.modifierA = pendingButtonA_ && M5.BtnA.isPressed();
    event.modifierB = pendingButtonB_ && M5.BtnB.isPressed();
    if (event.modifierA) pendingButtonA_ = false;
    if (event.modifierB) pendingButtonB_ = false;
    router_.dispatch(event, nowUs);
  } else {
    Serial.println("calculator: read_error");
  }
  reportPendingCoreButtonReleases();
}
