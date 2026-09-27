#pragma once

#include "AmyAudioActivityGate.h"
#include "AmySynthSlot.h"
#include "SequencerEventSink.h"

class AmyDrumEventSink final : public SequencerEventSink {
 public:
  AmyDrumEventSink(AmyAudioActivityGate& audioGate, AmySynthSlot& drumSlot)
      : audioGate_(audioGate), drumSlot_(drumSlot) {}

  void wake(uint32_t tailMs) override { audioGate_.wake(tailMs); }
  void trigger(const SequencerEvent& event) override {
    drumSlot_.noteOn(event.soundId, velocityFor(event.level));
  }

 private:
  static float velocityFor(StepLevel level) {
    switch (level) {
      case StepLevel::Weak:
        return 0.5f;
      case StepLevel::Normal:
      case StepLevel::Strong:
        return 1.0f;
      case StepLevel::Off:
        return 0.0f;
    }
    return 0.0f;
  }

  AmyAudioActivityGate& audioGate_;
  AmySynthSlot& drumSlot_;
};
