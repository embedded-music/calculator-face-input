#include <unity.h>

#include "../../apps/drum-step-sequencer/src/CalculatorCommand.cpp"
#include "../../apps/drum-step-sequencer/src/PatternBank.cpp"
#include "../../apps/drum-step-sequencer/src/PatternChain.cpp"
#include "../../apps/drum-step-sequencer/src/SequencerSettings.cpp"
#include "../../apps/drum-step-sequencer/src/SequencerPlaybackPolicy.h"
#include "../../apps/drum-step-sequencer/src/SequencerCommandMap.cpp"
#include "../../apps/drum-step-sequencer/src/SequencerPlayback.cpp"
#include "../../apps/drum-step-sequencer/src/SequencerStepClock.h"

class FakeClock final : public SequencerClock {
 public:
  uint32_t nextElapsedSteps = 0;
  SequencerClockAdvance poll(uint64_t) override {
    return {nextElapsedSteps};
  }
};

class FakeEventSink final : public SequencerEventSink {
 public:
  uint8_t noteCount = 0;
  uint8_t lastSoundId = 0;
  StepLevel lastLevel = StepLevel::Off;
  void trigger(const SequencerEvent& event) override {
    noteCount++;
    lastSoundId = event.soundId;
    lastLevel = event.level;
  }
};

void test_swing_clock_alternates_without_changing_pair_duration() {
  SequencerStepClock clock;
  TEST_ASSERT_TRUE(clock.begin(0, 100, 60, true));
  TEST_ASSERT_EQUAL_UINT64(120, clock.nextDeadlineUs());
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(120).elapsedSteps);
  TEST_ASSERT_EQUAL_UINT64(200, clock.nextDeadlineUs());
  TEST_ASSERT_EQUAL_UINT32(1, clock.poll(200).elapsedSteps);
  TEST_ASSERT_EQUAL_UINT64(320, clock.nextDeadlineUs());
  TEST_ASSERT_EQUAL_UINT32(2, clock.poll(400).elapsedSteps);
}

void test_triplet_rates_bypass_swing() {
  SequencerSettings settings;
  while (settings.decreaseRate()) {}
  while (settings.stepRateName()[0] != '1' || settings.stepRateName()[2] != '4') {
    settings.increaseRate();
  }
  settings.increaseRate();
  TEST_ASSERT_EQUAL_STRING("1/4T", settings.stepRateName());
  TEST_ASSERT_FALSE(settings.swingActive());
}
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

void test_settings_clamp_and_rate_boundaries() {
  SequencerSettings settings;

  while (settings.decreaseTempo()) {
  }
  TEST_ASSERT_EQUAL_UINT16(MIN_TEMPO_BPM, settings.tempoBpm());
  TEST_ASSERT_FALSE(settings.decreaseTempo());

  while (settings.increaseTempo()) {
  }
  TEST_ASSERT_EQUAL_UINT16(MAX_TEMPO_BPM, settings.tempoBpm());
  TEST_ASSERT_FALSE(settings.increaseTempo());

  while (settings.decreaseVolume()) {
  }
  TEST_ASSERT_EQUAL_UINT8(0, settings.speakerVolume());
  TEST_ASSERT_FALSE(settings.decreaseVolume());

  while (settings.increaseVolume()) {
  }
  TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, settings.speakerVolume());
  TEST_ASSERT_FALSE(settings.increaseVolume());

  while (settings.decreaseRate()) {
  }
  TEST_ASSERT_EQUAL_STRING("1/1", settings.stepRateName());
  TEST_ASSERT_FALSE(settings.decreaseRate());

  while (settings.increaseRate()) {
  }
  TEST_ASSERT_EQUAL_STRING("1/32T", settings.stepRateName());
  TEST_ASSERT_FALSE(settings.increaseRate());
}

void test_step_level_modifiers_replace_existing_level() {
  PatternEditorState state;
  state.selectTrack(0);

  TEST_ASSERT_TRUE(state.setStepLevel(2, StepLevel::Weak));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Weak),
                          static_cast<uint8_t>(state.stepLevel(0, 2)));

  TEST_ASSERT_TRUE(state.setStepLevel(2, StepLevel::Strong));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(state.stepLevel(0, 2)));

  TEST_ASSERT_TRUE(state.setStepLevel(2, StepLevel::Strong));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(state.stepLevel(0, 2)));

  TEST_ASSERT_FALSE(state.toggleStep(2));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Off),
                          static_cast<uint8_t>(state.stepLevel(0, 2)));
}

void test_playback_policy_triggers_only_on_time() {
  PatternEditorState state;

  const PlaybackStepDecision onTime =
      SequencerPlaybackPolicy::advance(state, 1);
  TEST_ASSERT_EQUAL_UINT8(0, onTime.previousStep);
  TEST_ASSERT_TRUE(onTime.triggerCurrentStep);
  TEST_ASSERT_EQUAL_UINT8(1, state.currentStep());

  const PlaybackStepDecision late =
      SequencerPlaybackPolicy::advance(state, 3);
  TEST_ASSERT_EQUAL_UINT8(1, late.previousStep);
  TEST_ASSERT_FALSE(late.triggerCurrentStep);
  TEST_ASSERT_EQUAL_UINT8(4, state.currentStep());
}

void test_playback_policy_reports_pattern_transition() {
  PatternEditorState state;
  state.selectNextPattern(1);

  const PlaybackStepDecision decision =
      SequencerPlaybackPolicy::advance(state, STEP_COUNT);
  TEST_ASSERT_TRUE(decision.patternBoundary);
  TEST_ASSERT_TRUE(decision.patternChanged);
  TEST_ASSERT_EQUAL_UINT8(1, state.currentPattern());
  TEST_ASSERT_FALSE(decision.triggerCurrentStep);
}

void test_command_map_routes_each_mode_semantically() {
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SequencerAction::ToggleStep),
                          static_cast<uint8_t>(
                              commandForMode(UiMode::Pattern, '7').action));
  TEST_ASSERT_EQUAL_UINT8(0, commandForMode(UiMode::Pattern, '7').index);

  const uint8_t settingKeys[] = {'A', 'M', '%', '/', '7', '8', '9', '*'};
  const SequencerAction settingActions[] = {
      SequencerAction::VolumeDown, SequencerAction::VolumeUp,
      SequencerAction::TempoDown, SequencerAction::TempoUp,
      SequencerAction::RateDown, SequencerAction::RateUp,
      SequencerAction::SwingDown, SequencerAction::SwingUp};
  for (uint8_t index = 0; index < 8; index++) {
    TEST_ASSERT_EQUAL_UINT8(
        static_cast<uint8_t>(settingActions[index]),
        static_cast<uint8_t>(
            commandForMode(UiMode::Settings, settingKeys[index]).action));
  }
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SequencerAction::SelectSound),
                          static_cast<uint8_t>(
                              commandForMode(UiMode::Sounds, '=').action));
  TEST_ASSERT_EQUAL_UINT8(19, commandForMode(UiMode::Sounds, '=').index);

  const SequencerCommand chain = commandForMode(UiMode::Arrangement, '0');
  TEST_ASSERT_EQUAL_UINT8(
      static_cast<uint8_t>(SequencerAction::ToggleChainPosition),
      static_cast<uint8_t>(chain.action));
  TEST_ASSERT_EQUAL_UINT8(10, chain.index);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(SequencerAction::ClearPattern),
                          static_cast<uint8_t>(
                              commandForMode(UiMode::Arrangement, '=').action));
}

void test_playback_with_fake_clock_and_event_sink() {
  PatternEditorState state;
  state.selectTrack(0);
  state.toggleStep(1);
  FakeClock clock;
  FakeEventSink sink;
  SequencerPlayback playback(state, clock, sink);

  clock.nextElapsedSteps = 1;
  const SequencerPlaybackUpdate onTime = playback.update(0);
  TEST_ASSERT_EQUAL_UINT32(1, onTime.elapsedSteps);
  TEST_ASSERT_EQUAL_UINT8(1, sink.noteCount);
  TEST_ASSERT_EQUAL_UINT8(state.soundForTrack(0).midiNote, sink.lastSoundId);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Normal),
                          static_cast<uint8_t>(sink.lastLevel));

  clock.nextElapsedSteps = 3;
  const SequencerPlaybackUpdate late = playback.update(0);
  TEST_ASSERT_EQUAL_UINT32(3, late.elapsedSteps);
  TEST_ASSERT_EQUAL_UINT8(1, sink.noteCount);
}

void test_playback_triggers_all_active_tracks_once_per_step() {
  PatternEditorState state;
  state.selectTrack(0);
  state.toggleStep(1);
  state.selectTrack(1);
  state.toggleStep(1);
  FakeClock clock;
  FakeEventSink sink;
  SequencerPlayback playback(state, clock, sink);

  clock.nextElapsedSteps = 1;
  const SequencerPlaybackUpdate update = playback.update(0);

  TEST_ASSERT_EQUAL_UINT8(0, update.previousStep);
  TEST_ASSERT_EQUAL_UINT8(2, sink.noteCount);
}

void test_playback_reports_chain_change_then_triggers_new_pattern() {
  PatternEditorState state;
  state.selectTrack(0);
  state.toggleStep(0);
  state.toggleChainPosition(1);
  state.selectNextPattern(1);
  state.clearPattern(1);

  FakeClock clock;
  FakeEventSink sink;
  SequencerPlayback playback(state, clock, sink);

  clock.nextElapsedSteps = STEP_COUNT;
  const SequencerPlaybackUpdate boundary = playback.update(0);
  TEST_ASSERT_TRUE(boundary.patternBoundary);
  TEST_ASSERT_TRUE(boundary.patternChanged);
  TEST_ASSERT_EQUAL_UINT8(0, sink.noteCount);

  state.selectTrack(0);
  state.toggleStep(1);
  clock.nextElapsedSteps = 1;
  const SequencerPlaybackUpdate nextStep = playback.update(0);
  TEST_ASSERT_FALSE(nextStep.patternBoundary);
  TEST_ASSERT_EQUAL_UINT8(1, sink.noteCount);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_swing_clock_alternates_without_changing_pair_duration);
  RUN_TEST(test_triplet_rates_bypass_swing);
  RUN_TEST(test_sparse_chain_advances_only_enabled_positions);
  RUN_TEST(test_current_chain_position_removal_is_quantized);
  RUN_TEST(test_clone_and_clear_preserve_sound_choices);
  RUN_TEST(test_settings_clamp_and_rate_boundaries);
  RUN_TEST(test_step_level_modifiers_replace_existing_level);
  RUN_TEST(test_playback_policy_triggers_only_on_time);
  RUN_TEST(test_playback_policy_reports_pattern_transition);
  RUN_TEST(test_command_map_routes_each_mode_semantically);
  RUN_TEST(test_playback_with_fake_clock_and_event_sink);
  RUN_TEST(test_playback_triggers_all_active_tracks_once_per_step);
  RUN_TEST(test_playback_reports_chain_change_then_triggers_new_pattern);
  return UNITY_END();
}
