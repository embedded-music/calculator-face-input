#pragma once

#include "AmyAudioActivityGate.h"
#include "AmySynthSlot.h"
#include "SequencerEventSink.h"
#include "StepVelocity.h"

class AmyDrumEventSink final : public SequencerEventSink {
 public:
  AmyDrumEventSink(AmyAudioActivityGate& audioGate, AmySynthSlot& drumSlot)
      : audioGate_(audioGate), drumSlot_(drumSlot) {}

  void wake(uint32_t tailMs) override { audioGate_.wake(tailMs); }
  void trigger(const SequencerEvent& event) override {
    drumSlot_.noteOn(event.soundId, velocityForStepLevel(event.level));
  }

 private:
  AmyAudioActivityGate& audioGate_;
  AmySynthSlot& drumSlot_;
};
