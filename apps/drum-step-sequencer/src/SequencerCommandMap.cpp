#include "SequencerCommandMap.h"

#include "CalculatorCommand.h"
#include "PatternChain.h"

SequencerCommand commandForMode(UiMode mode, uint8_t value) {
  if (mode == UiMode::Settings) {
    if (value == '%') return {SequencerAction::VolumeDown, 0};
    if (value == '/') return {SequencerAction::VolumeUp, 0};
    if (value == '9') return {SequencerAction::TempoDown, 0};
    if (value == '*') return {SequencerAction::TempoUp, 0};
    if (value == '6') return {SequencerAction::RateDown, 0};
    if (value == '-') return {SequencerAction::RateUp, 0};
    return {};
  }

  if (mode == UiMode::Sounds) {
    uint8_t soundIndex = 0;
    return soundIndexForCalculatorValue(value, soundIndex)
               ? SequencerCommand{SequencerAction::SelectSound, soundIndex}
               : SequencerCommand{};
  }

  if (mode == UiMode::Arrangement) {
    if (value == '*') return {SequencerAction::ToggleClone, 0};
    if (value == '=') return {SequencerAction::ClearPattern, 0};
    constexpr uint8_t chainValues[] = {
        '7', '8', '9', '4', '5', '6', '1', '2', '3', '.', '0', '`'};
    for (uint8_t position = 0; position < CHAIN_MAX_LENGTH; position++) {
      if (chainValues[position] == value) {
        return {SequencerAction::ToggleChainPosition, position};
      }
    }
    uint8_t patternIndex = 0;
    return patternIndexForCalculatorValue(value, patternIndex)
               ? SequencerCommand{SequencerAction::SelectPattern, patternIndex}
               : SequencerCommand{};
  }

  const CalculatorCommand calculatorCommand = commandForCalculatorValue(value);
  if (calculatorCommand.type == CalculatorCommandType::SelectTrack) {
    return {SequencerAction::SelectTrack, calculatorCommand.index};
  }
  if (calculatorCommand.type == CalculatorCommandType::ToggleStep) {
    return {SequencerAction::ToggleStep, calculatorCommand.index};
  }
  return {};
}
