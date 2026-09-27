#pragma once

#include <stdint.h>

#include "StepLevel.h"

// A logical instantaneous event from one pattern lane. Applications decide
// whether a lane addresses a drum voice, sample, MIDI note, or physical output.
struct TriggerEvent {
  uint8_t lane = 0;
  StepLevel level = StepLevel::Normal;
};
