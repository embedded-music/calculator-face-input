#include <unity.h>

#include "TriggerEventSink.h"
#include "TriggerPattern.h"
#include "TriggerPatternPlayer.h"

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

void test_player_emits_active_lanes_in_order() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(12));
  TEST_ASSERT_TRUE(pattern.setStepLevel(3, 7, StepLevel::Weak));
  TEST_ASSERT_TRUE(pattern.setStepLevel(1, 7, StepLevel::Strong));
  RecordingSink sink;

  TEST_ASSERT_EQUAL_UINT8(2,
                          TriggerPatternPlayer::emitStep(pattern, 7, sink));
  TEST_ASSERT_EQUAL_UINT8(2, sink.count);
  TEST_ASSERT_EQUAL_UINT8(1, sink.events[0].lane);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(sink.events[0].level));
  TEST_ASSERT_EQUAL_UINT8(3, sink.events[1].lane);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Weak),
                          static_cast<uint8_t>(sink.events[1].level));
}

void test_player_ignores_empty_and_out_of_range_steps() {
  TriggerPattern pattern;
  TEST_ASSERT_TRUE(pattern.setLength(4));
  RecordingSink sink;

  TEST_ASSERT_EQUAL_UINT8(0,
                          TriggerPatternPlayer::emitStep(pattern, 2, sink));
  TEST_ASSERT_EQUAL_UINT8(0,
                          TriggerPatternPlayer::emitStep(pattern, 4, sink));
  TEST_ASSERT_EQUAL_UINT8(0, sink.count);
}
}  // namespace

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_pattern_has_bounded_variable_length);
  RUN_TEST(test_shortening_clears_hidden_tail);
  RUN_TEST(test_player_emits_active_lanes_in_order);
  RUN_TEST(test_player_ignores_empty_and_out_of_range_steps);
  return UNITY_END();
}
