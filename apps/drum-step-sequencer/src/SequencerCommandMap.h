#pragma once

#include <stdint.h>

#include "UiMode.h"

enum class SequencerAction : uint8_t {
  Ignore,
  VolumeDown,
  VolumeUp,
  TempoDown,
  TempoUp,
  RateDown,
  RateUp,
  SelectSound,
  ToggleClone,
  ClearPattern,
  ToggleChainPosition,
  SelectPattern,
  SelectTrack,
  ToggleStep,
};

struct SequencerCommand {
  SequencerAction action = SequencerAction::Ignore;
  uint8_t index = 0;
};

SequencerCommand commandForMode(UiMode mode, uint8_t value);
