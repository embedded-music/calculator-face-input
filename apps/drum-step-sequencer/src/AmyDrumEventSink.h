#pragma once

#include "AmyAudioActivityGate.h"
#include "AmySynthSlot.h"
#include "SequencerEventSink.h"

class AmyDrumEventSink final : public SequencerEventSink {
 public:
  AmyDrumEventSink(AmyAudioActivityGate& audioGate, AmySynthSlot& drumSlot)
      : audioGate_(audioGate), drumSlot_(drumSlot) {}

  void wake(uint32_t tailMs) override { audioGate_.wake(tailMs); }
  void trigger(uint8_t soundId, float velocity) override {
    drumSlot_.noteOn(soundId, velocity);
  }

 private:
  AmyAudioActivityGate& audioGate_;
  AmySynthSlot& drumSlot_;
};
