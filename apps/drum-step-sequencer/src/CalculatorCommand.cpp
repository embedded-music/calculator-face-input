#include "CalculatorCommand.h"

CalculatorCommand commandForCalculatorValue(uint8_t value) {
  constexpr uint8_t trackValues[] = {'A', 'M', '%', '/'};
  constexpr uint8_t stepValues[] = {
      '7', '8', '9', '*', '4', '5', '6', '-',
      '1', '2', '3', '+', '.', '0', '`', '='};

  for (uint8_t index = 0; index < sizeof(trackValues); index++) {
    if (trackValues[index] == value) {
      return {CalculatorCommandType::SelectTrack, index};
    }
  }
  for (uint8_t index = 0; index < sizeof(stepValues); index++) {
    if (stepValues[index] == value) {
      return {CalculatorCommandType::ToggleStep, index};
    }
  }
  return {};
}

bool soundIndexForCalculatorValue(uint8_t value, uint8_t& soundIndex) {
  constexpr uint8_t soundValues[] = {
      'A', 'M', '%', '/', '7', '8', '9', '*', '4', '5', '6', '-',
      '1', '2', '3', '+', '.', '0', '`', '=',
  };
  for (uint8_t index = 0; index < sizeof(soundValues); index++) {
    if (soundValues[index] == value) {
      soundIndex = index;
      return true;
    }
  }
  return false;
}
