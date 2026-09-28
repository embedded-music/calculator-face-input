#include <unity.h>

#include "TriggerEventSink.h"
#include "TriggerPattern.h"
#include "TriggerPatternCell.h"
#include "TriggerPatternEmitter.h"
#include "TriggerPatternLoader.h"
#include "TriggerPatternPlayer.h"
#include "TriggerPatternCursor.h"

namespace {
class RecordingSink final : public TriggerEventSink {
 public:
  void trigger(const TriggerEvent& event) override {
    if (count < TriggerPattern::LANE_COUNT) events[count] = event;
    count++;
  }

  TriggerEvent events[TriggerPattern::LANE_COUNT]{};
  uint8_t count = 0;
};

class RecordingSource final : public TriggerPatternSource {
 public:
  const TriggerPattern& currentPattern() const override { return pattern; }
  void completedCycles(uint32_t count) override { completed += count; }

  TriggerPattern pattern;
  uint32_t completed = 0;
};

void test_pattern_has_bounded_variable_length() {
  TriggerPattern pattern;

  TEST_ASSERT_EQUAL_UINT8(1, pattern.length());
  TEST_ASSERT_FALSE(pattern.setLength(0));
  TEST_ASSERT_FALSE(pattern.setLength(TriggerPattern::STEP_CAPACITY + 1));
  TEST_ASSERT_TRUE(pattern.setLength(12));
  TEST_ASSERT_EQUAL_UINT8(12, pattern.length());
  TEST_ASSERT_TRUE(pattern.setStepLevel(0, 11, StepLevel::Normal));
  TEST_ASSERT_FALSE(pattern.setStepLevel(0, 12, StepLevel::Normal));
}

void test_shortening_clears_hidden_tail() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(16));
  TEST_ASSERT_TRUE(pattern.setStepLevel(2, 15, StepLevel::Strong));

  TEST_ASSERT_TRUE(pattern.setLength(12));
  TEST_ASSERT_TRUE(pattern.setLength(16));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Off),
                          static_cast<uint8_t>(pattern.stepLevel(2, 15)));
}

void test_emitter_emits_active_lanes_in_order() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(12));
  TEST_ASSERT_TRUE(pattern.setStepLevel(3, 7, StepLevel::Weak));
  TEST_ASSERT_TRUE(pattern.setStepLevel(1, 7, StepLevel::Strong));
  RecordingSink sink;

  TEST_ASSERT_EQUAL_UINT8(2,
                          TriggerPatternEmitter::emitStep(pattern, 7, sink));
  TEST_ASSERT_EQUAL_UINT8(2, sink.count);
  TEST_ASSERT_EQUAL_UINT8(1, sink.events[0].lane);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(sink.events[0].level));
  TEST_ASSERT_EQUAL_UINT8(3, sink.events[1].lane);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Weak),
                          static_cast<uint8_t>(sink.events[1].level));
}

void test_emitter_ignores_empty_and_out_of_range_steps() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(4));
  RecordingSink sink;

  TEST_ASSERT_EQUAL_UINT8(0,
                          TriggerPatternEmitter::emitStep(pattern, 2, sink));
  TEST_ASSERT_EQUAL_UINT8(0,
                          TriggerPatternEmitter::emitStep(pattern, 4, sink));
  TEST_ASSERT_EQUAL_UINT8(0, sink.count);
}

void test_cursor_advances_and_wraps_variable_cycles() {
  TriggerPatternCursor cursor;

  const TriggerPatternCursorUpdate first = cursor.advance(5, 12);
  TEST_ASSERT_EQUAL_UINT8(0, first.previousStep);
  TEST_ASSERT_EQUAL_UINT8(5, first.currentStep);
  TEST_ASSERT_EQUAL_UINT32(0, first.completedCycles);
  TEST_ASSERT_FALSE(first.shouldEmit());

  const TriggerPatternCursorUpdate wrapped = cursor.advance(19, 12);
  TEST_ASSERT_EQUAL_UINT8(5, wrapped.previousStep);
  TEST_ASSERT_EQUAL_UINT8(0, wrapped.currentStep);
  TEST_ASSERT_EQUAL_UINT32(2, wrapped.completedCycles);
}

void test_cursor_emits_only_current_step_on_request() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(4));
  TEST_ASSERT_TRUE(pattern.setStepLevel(2, 1, StepLevel::Strong));
  TriggerPatternCursor cursor;
  RecordingSink sink;

  const TriggerPatternCursorUpdate update =
      cursor.advance(1, pattern.length());
  TEST_ASSERT_TRUE(update.shouldEmit());
  TEST_ASSERT_EQUAL_UINT8(1, cursor.currentStep());
  TEST_ASSERT_EQUAL_UINT8(1, cursor.emitCurrentStep(pattern, sink));
  TEST_ASSERT_EQUAL_UINT8(2, sink.events[0].lane);
}

void test_loader_builds_pattern_from_sparse_cells() {
  constexpr TriggerPatternCell cells[] = {
      {0, 0, StepLevel::Strong},
      {2, 5, StepLevel::Weak},
  };
  TriggerPattern pattern;

  TEST_ASSERT_TRUE(TriggerPatternLoader::load(pattern, 12, cells));
  TEST_ASSERT_EQUAL_UINT8(12, pattern.length());
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(pattern.stepLevel(0, 0)));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Weak),
                          static_cast<uint8_t>(pattern.stepLevel(2, 5)));
}

void test_loader_rejects_invalid_cells_without_mutating_pattern() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(4));
  TEST_ASSERT_TRUE(pattern.setStepLevel(1, 2, StepLevel::Normal));
  constexpr TriggerPatternCell invalid[] = {
      {TriggerPattern::LANE_COUNT, 0, StepLevel::Strong},
  };

  TEST_ASSERT_FALSE(TriggerPatternLoader::load(pattern, 4, invalid));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Normal),
                          static_cast<uint8_t>(pattern.stepLevel(1, 2)));
}

void test_player_owns_clock_cursor_and_late_step_policy() {
  RecordingSource source;
  TEST_ASSERT_TRUE(source.pattern.setLength(4));
  TEST_ASSERT_TRUE(source.pattern.setStepLevel(0, 0, StepLevel::Strong));
  TEST_ASSERT_TRUE(source.pattern.setStepLevel(0, 1, StepLevel::Normal));
  RecordingSink sink;
  TriggerPatternPlayer player(source, sink);

  TEST_ASSERT_TRUE(player.begin(0, StepIntervals::constant(100),
                                TriggerStart::Silent));
  TEST_ASSERT_EQUAL_UINT8(0, sink.count);
  TEST_ASSERT_FALSE(player.update(99).advanced());

  const TriggerPatternPlayerUpdate onTime = player.update(100);
  TEST_ASSERT_EQUAL_UINT8(1, onTime.currentStep);
  TEST_ASSERT_TRUE(onTime.emitted());
  TEST_ASSERT_EQUAL_UINT8(1, sink.count);

  const TriggerPatternPlayerUpdate late = player.update(400);
  TEST_ASSERT_EQUAL_UINT32(3, late.elapsedSteps);
  TEST_ASSERT_EQUAL_UINT8(0, late.currentStep);
  TEST_ASSERT_EQUAL_UINT32(1, source.completed);
  TEST_ASSERT_FALSE(late.emitted());
  TEST_ASSERT_EQUAL_UINT8(1, sink.count);

  TEST_ASSERT_TRUE(player.restart(500, StepIntervals::constant(100),
                                  TriggerStart::EmitImmediately));
  TEST_ASSERT_EQUAL_UINT8(2, sink.count);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(sink.events[1].level));
}
}  // namespace

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_pattern_has_bounded_variable_length);
  RUN_TEST(test_shortening_clears_hidden_tail);
  RUN_TEST(test_emitter_emits_active_lanes_in_order);
  RUN_TEST(test_emitter_ignores_empty_and_out_of_range_steps);
  RUN_TEST(test_cursor_advances_and_wraps_variable_cycles);
  RUN_TEST(test_cursor_emits_only_current_step_on_request);
  RUN_TEST(test_loader_builds_pattern_from_sparse_cells);
  RUN_TEST(test_loader_rejects_invalid_cells_without_mutating_pattern);
  RUN_TEST(test_player_owns_clock_cursor_and_late_step_policy);
  return UNITY_END();
}
