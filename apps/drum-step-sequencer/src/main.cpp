#include <Arduino.h>
#include <esp_timer.h>
#include <M5Unified.h>
#include <Wire.h>

#include "AmyAudioActivityGate.h"
#include "AmyM5SpeakerBridge.h"
#include "AmySynthSlot.h"
#include "CalculatorLink.h"
#include "DeadlineClock.h"
#include "PatternEditorState.h"
#include "PatternEditorView.h"
#include "SequencerInput.h"
#include "SequencerPlayback.h"

namespace {
constexpr uint8_t AMY_SYNTH_ID = 1;
constexpr uint8_t AMY_DRUM_VOICES = 1;
constexpr uint16_t AMY_GM_DRUM_PATCH = 258;
PatternEditorState editor;
PatternEditorView view(M5.Display);
DeadlineClock stepClock;
SequencerInput input(editor, view, stepClock);
AmyM5SpeakerBridge amyBridge;
AmyAudioActivityGate audioGate(amyBridge);
AmySynthSlot drumSlot;
SequencerPlayback playback(editor, stepClock, audioGate, drumSlot);

bool calculatorAcknowledges() {
  Wire.beginTransmission(CALCULATOR_I2C_ADDRESS);
  return Wire.endTransmission() == 0;
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);
  auto config = M5.config();
  config.internal_spk = true;
  config.internal_mic = false;
  M5.begin(config);
  M5.Display.setRotation(1);
  pinMode(CALCULATOR_INTERRUPT_PIN, INPUT_PULLUP);
  Wire.begin(CALCULATOR_SDA_PIN, CALCULATOR_SCL_PIN,
            CALCULATOR_I2C_FREQUENCY_HZ);
  view.draw(editor);
  amyBridge.begin();
  M5.Speaker.setVolume(editor.speakerVolume());
  drumSlot.begin(AMY_SYNTH_ID, AMY_DRUM_VOICES, AMY_GM_DRUM_PATCH);
  const bool clockStarted = stepClock.begin(
      static_cast<uint64_t>(esp_timer_get_time()), editor.stepIntervalUs());
  if (!clockStarted) Serial.println("clock: begin_failed");
  Serial.printf("drum_step_sequencer: calculator_detected=%s\n",
                calculatorAcknowledges() ? "yes" : "no");
}

void loop() {
  M5.update();
  const uint64_t nowUs = static_cast<uint64_t>(esp_timer_get_time());
  input.update(nowUs);

  const SequencerPlaybackUpdate update = playback.update(nowUs);
  if (update.elapsedSteps > 0) {
    if (input.mode() == UiMode::Pattern) {
      if (update.patternChanged) view.drawPatternChange(editor);
      else view.drawPlayheadChange(editor, update.previousStep);
    } else if (update.patternBoundary && input.mode() == UiMode::Arrangement) {
      view.drawArrangementValues(editor);
    }
  }

  audioGate.update(false);
  delay(1);
}
