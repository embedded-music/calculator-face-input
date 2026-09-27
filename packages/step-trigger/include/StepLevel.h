#pragma once

#include <stdint.h>

// A stored pattern position is empty or carries one of three discrete trigger
// intensities. Trigger playback never emits an Off level.
enum class StepLevel : uint8_t {
  Off,
  Normal,
  Weak,
  Strong,
};
