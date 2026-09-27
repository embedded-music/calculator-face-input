#include <unity.h>

#include "../../apps/drum-step-sequencer/src/HeldButtonGesture.h"

void test_held_button_modifies_multiple_keys_and_consumes_tap() {
  HeldButtonGesture button;
  button.press();

  TEST_ASSERT_TRUE(button.modifierActive(true));
  button.consumeAsModifier();
  TEST_ASSERT_TRUE(button.modifierActive(true));
  button.consumeAsModifier();

  TEST_ASSERT_FALSE(button.releaseAsTap());
  TEST_ASSERT_FALSE(button.modifierActive(false));
}

void test_unused_held_button_releases_as_tap() {
  HeldButtonGesture button;
  button.press();

  TEST_ASSERT_TRUE(button.modifierActive(true));
  TEST_ASSERT_TRUE(button.releaseAsTap());
  TEST_ASSERT_FALSE(button.releaseAsTap());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_held_button_modifies_multiple_keys_and_consumes_tap);
  RUN_TEST(test_unused_held_button_releases_as_tap);
  return UNITY_END();
}
