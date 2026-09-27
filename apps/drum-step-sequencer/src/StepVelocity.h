#pragma once

#include "StepLevel.h"

// Leave clear perceptual space between the three authored step levels while
// reserving the synth's full velocity for strong accents.
constexpr float velocityForStepLevel(StepLevel level) {
  switch (level) {
    case StepLevel::Weak:
      return 0.45f;
    case StepLevel::Normal:
      return 0.70f;
    case StepLevel::Strong:
      return 1.0f;
    case StepLevel::Off:
      return 0.0f;
  }
  return 0.0f;
}
