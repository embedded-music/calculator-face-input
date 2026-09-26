#include <unity.h>

#include "../../apps/drum-step-sequencer/src/PatternBank.cpp"
#include "../../apps/drum-step-sequencer/src/PatternChain.cpp"
#include "../../apps/drum-step-sequencer/src/PatternEditorState.cpp"

void test_sparse_chain_advances_only_enabled_positions() {
  PatternEditorState state;

  TEST_ASSERT_EQUAL_UINT8(1, state.chainLength());
  TEST_ASSERT_TRUE(state.chainPositionEnabled(0));

  TEST_ASSERT_TRUE(state.toggleChainPosition(10));
  TEST_ASSERT_EQUAL_UINT8(2, state.chainLength());
  TEST_ASSERT_EQUAL_UINT8(10, state.nextChainPosition());

  TEST_ASSERT_TRUE(state.advanceByElapsedSteps(STEP_COUNT));
  TEST_ASSERT_EQUAL_UINT8(10, state.chainPosition());
  TEST_ASSERT_EQUAL_UINT8(0, state.nextChainPosition());

  TEST_ASSERT_TRUE(state.advanceByElapsedSteps(STEP_COUNT));
  TEST_ASSERT_EQUAL_UINT8(0, state.chainPosition());
}

void test_current_chain_position_removal_is_quantized() {
  PatternEditorState state;
  TEST_ASSERT_TRUE(state.toggleChainPosition(10));
  TEST_ASSERT_TRUE(state.advanceByElapsedSteps(STEP_COUNT));
  TEST_ASSERT_EQUAL_UINT8(10, state.chainPosition());

  TEST_ASSERT_TRUE(state.toggleChainPosition(10));
  TEST_ASSERT_FALSE(state.chainPositionEnabled(10));
  TEST_ASSERT_EQUAL_UINT8(10, state.chainPosition());
  TEST_ASSERT_TRUE(state.chainPositionVisible(10));

  TEST_ASSERT_TRUE(state.advanceByElapsedSteps(STEP_COUNT));
  TEST_ASSERT_EQUAL_UINT8(0, state.chainPosition());
  TEST_ASSERT_FALSE(state.chainPositionEnabled(10));
}

void test_clone_and_clear_preserve_sound_choices() {
  PatternEditorState state;
  const uint8_t originalSound = state.soundForTrack(0).midiNote;
  state.selectTrack(0);
  state.toggleStep(0);
  state.cloneCurrentPatternTo(1);
  TEST_ASSERT_TRUE(state.patternEmpty(0) == false);
  TEST_ASSERT_TRUE(state.stepActive(0, 0));

  state.selectNextPattern(1);
  state.advanceByElapsedSteps(STEP_COUNT);
  TEST_ASSERT_TRUE(state.stepActive(0, 0));
  TEST_ASSERT_EQUAL_UINT8(originalSound, state.soundForTrack(0).midiNote);

  state.clearPattern(1);
  TEST_ASSERT_TRUE(state.patternEmpty(1));
  TEST_ASSERT_EQUAL_UINT8(originalSound, state.soundForTrack(0).midiNote);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_sparse_chain_advances_only_enabled_positions);
  RUN_TEST(test_current_chain_position_removal_is_quantized);
  RUN_TEST(test_clone_and_clear_preserve_sound_choices);
  return UNITY_END();
}
