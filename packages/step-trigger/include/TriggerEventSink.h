#pragma once

#include "TriggerEvent.h"

class TriggerEventSink {
 public:
  virtual ~TriggerEventSink() = default;
  virtual void trigger(const TriggerEvent& event) = 0;
};
