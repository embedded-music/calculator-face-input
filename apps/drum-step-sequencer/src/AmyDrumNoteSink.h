#pragma once

#include "AmyAudioActivityGate.h"
#include "AmySynthSlot.h"
#include "SequencerNoteSink.h"

class AmyDrumNoteSink final : public SequencerNoteSink {
 public:
  AmyDrumNoteSink(AmyAudioActivityGate& audioGate, AmySynthSlot& drumSlot)
      : audioGate_(audioGate), drumSlot_(drumSlot) {}

  void wake(uint32_t tailMs) override { audioGate_.wake(tailMs); }
  void noteOn(uint8_t midiNote, float velocity) override {
    drumSlot_.noteOn(midiNote, velocity);
  }

 private:
  AmyAudioActivityGate& audioGate_;
  AmySynthSlot& drumSlot_;
};
