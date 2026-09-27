#pragma once

#include <stdint.h>

// A pattern cell is either empty or an active hit with one of three
// intensities. Calculator editing creates Normal hits, or Weak and Strong hits
// when the corresponding Core button modifier is held.
enum class StepLevel : uint8_t {
  Off,
  Normal,
  Weak,
  Strong,
};
