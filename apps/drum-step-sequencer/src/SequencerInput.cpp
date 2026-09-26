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
    router_.dispatch({SequencerInputEventType::ButtonA}, 0);
  }
  if (M5.BtnB.wasPressed()) {
    router_.dispatch({SequencerInputEventType::ButtonB}, 0);
  }
  if (M5.BtnC.wasPressed()) {
    router_.dispatch({SequencerInputEventType::ButtonC}, 0);
  }
}

void SequencerInput::update(uint64_t nowUs) {
  reportCoreButtons();
  if (digitalRead(CALCULATOR_INTERRUPT_PIN) != LOW) return;
  uint8_t value = 0;
  if (readCalculatorByte(value)) {
    router_.dispatch({SequencerInputEventType::CalculatorValue, value}, nowUs);
  } else {
    Serial.println("calculator: read_error");
  }
}
