#pragma once

#include <stdint.h>

enum class SequencerInputEventType : uint8_t {
  CalculatorValue,
  ButtonA,
  ButtonB,
  ButtonC,
};

struct SequencerInputEvent {
  SequencerInputEventType type;
  uint8_t value = 0;
};
