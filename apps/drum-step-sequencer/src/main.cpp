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

namespace {
constexpr uint8_t AMY_SYNTH_ID = 1;
constexpr uint8_t AMY_DRUM_VOICES = 1;
constexpr uint16_t AMY_GM_DRUM_PATCH = 258;
constexpr float DRUM_VELOCITY = 1.0f;
constexpr uint32_t DRUM_TAIL_MS = 1500;

PatternEditorState editor;
PatternEditorView view(M5.Display);
DeadlineClock stepClock;
SequencerInput input(editor, view, stepClock);
AmyM5SpeakerBridge amyBridge;
AmyAudioActivityGate audioGate(amyBridge);
AmySynthSlot drumSlot;

bool calculatorAcknowledges() {
  Wire.beginTransmission(CALCULATOR_I2C_ADDRESS);
  return Wire.endTransmission() == 0;
}

void triggerCurrentStep() {
  bool hasTrigger = false;
  for (uint8_t track = 0; track < TRACK_COUNT; track++) {
    if (editor.stepActive(track, editor.currentStep())) {
      if (!hasTrigger) audioGate.wake(DRUM_TAIL_MS);
      drumSlot.noteOn(editor.soundForTrack(track).midiNote, DRUM_VELOCITY);
      hasTrigger = true;
    }
  }
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

  const uint32_t elapsedSteps = stepClock.poll(nowUs).elapsed_intervals;
  if (elapsedSteps > 0) {
    const uint8_t previousStep = editor.currentStep();
    // Audio events whose deadlines are already in the past cannot be
    // recovered in real time. Advance the logical position to the present
    // and discard all overdue triggers. When more than one event elapsed,
    // wait for the next absolute deadline before emitting audio again; an
    // immediate trigger here would compress the first interval after recovery.
    editor.advanceByElapsedSteps(elapsedSteps);
    if (elapsedSteps == 1) {
      triggerCurrentStep();
    } else {
      Serial.printf("transport: skipped_steps count=%lu\n",
                    static_cast<unsigned long>(elapsedSteps));
    }
    if (input.mode() == UiMode::Pattern) {
      view.drawPlayheadChange(editor, previousStep);
    }
  }

  audioGate.update(false);
  delay(1);
}
