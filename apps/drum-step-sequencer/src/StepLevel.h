#pragma once

#include <stdint.h>

// A pattern cell is either empty or an active hit with one of three
// intensities. Editing currently creates/removes Normal hits; the weaker and
// stronger levels are reserved for the accent interaction.
enum class StepLevel : uint8_t {
  Off,
  Normal,
  Weak,
  Strong,
};
