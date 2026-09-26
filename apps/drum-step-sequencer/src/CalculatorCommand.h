#pragma once

#include <stdint.h>

enum class CalculatorCommandType : uint8_t { Ignore, SelectTrack, ToggleStep };

struct CalculatorCommand {
  CalculatorCommandType type = CalculatorCommandType::Ignore;
  uint8_t index = 0;
};

CalculatorCommand commandForCalculatorValue(uint8_t value);
bool soundIndexForCalculatorValue(uint8_t value, uint8_t& soundIndex);
bool patternIndexForCalculatorValue(uint8_t value, uint8_t& patternIndex);
