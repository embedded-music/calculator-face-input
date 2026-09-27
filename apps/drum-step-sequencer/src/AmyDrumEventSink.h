#pragma once

#include "AmyTriggerOutput.h"
#include "SequencerEventSink.h"

class AmyDrumEventSink final : public SequencerEventSink {
 public:
  AmyDrumEventSink(AmyAudioActivityGate& audioGate, AmySynthSlot& drumSlot)
      : output_(audioGate, drumSlot) {}

  void trigger(const SequencerEvent& event) override {
    output_.trigger(event.soundId, event.level);
  }

 private:
  AmyTriggerOutput output_;
};
