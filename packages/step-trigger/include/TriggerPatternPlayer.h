#pragma once

#include <stdint.h>

#include "AlternatingDeadlineClock.h"
#include "TriggerEventSink.h"
#include "TriggerPatternCursor.h"

struct StepIntervals {
  uint64_t firstUs;
  uint64_t secondUs;

  static StepIntervals constant(uint64_t stepUs) {
    return {stepUs, stepUs};
  }

  static StepIntervals alternating(uint64_t firstUs, uint64_t secondUs) {
    return {firstUs, secondUs};
  }
};

enum class TriggerTimingChange : uint8_t { PreservePhase, ResetFromNow };
enum class TriggerStart : uint8_t { Silent, EmitImmediately };

class TriggerPatternSource {
 public:
  virtual ~TriggerPatternSource() = default;
  virtual const TriggerPattern& currentPattern() const = 0;
  virtual void completedCycles(uint32_t count) { (void)count; }
};

struct TriggerPatternPlayerUpdate {
  uint32_t elapsedSteps = 0;
  uint8_t previousStep = 0;
  uint8_t currentStep = 0;
  uint32_t completedCycles = 0;
  uint8_t emittedEvents = 0;
  bool advanced() const { return elapsedSteps > 0; }
  bool emitted() const { return emittedEvents > 0; }
};

class TriggerPatternPlayer {
 public:
  TriggerPatternPlayer(TriggerPatternSource& source, TriggerEventSink& sink)
      : source_(source), sink_(sink) {}
  bool begin(uint64_t nowUs, StepIntervals intervals, TriggerStart start);
  bool changeTiming(uint64_t nowUs, StepIntervals intervals,
                    TriggerTimingChange change);
  bool restart(uint64_t nowUs, StepIntervals intervals,
               TriggerStart start);
  TriggerPatternPlayerUpdate update(uint64_t nowUs);
  uint8_t emitCurrentStep();
  uint8_t currentStep() const { return cursor_.currentStep(); }

 private:
  TriggerPatternSource& source_;
  TriggerEventSink& sink_;
  AlternatingDeadlineClock clock_;
  TriggerPatternCursor cursor_;
};
