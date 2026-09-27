#pragma once

#include <stdint.h>

constexpr uint16_t MIN_TEMPO_BPM = 40;
constexpr uint16_t MAX_TEMPO_BPM = 240;
constexpr uint16_t TEMPO_INCREMENT_BPM = 5;
constexpr uint8_t VOLUME_INCREMENT = 16;
constexpr uint8_t MIN_SWING_PERCENT = 50;
constexpr uint8_t MAX_SWING_PERCENT = 75;

enum class StepRate : uint8_t {
  Whole,
  Half,
  Quarter,
  QuarterTriplet,
  Eighth,
  EighthTriplet,
  Sixteenth,
  SixteenthTriplet,
  ThirtySecond,
  ThirtySecondTriplet,
};

class SequencerSettings {
 public:
  uint16_t tempoBpm() const { return tempoBpm_; }
  uint64_t stepIntervalUs() const;
  StepRate stepRate() const { return stepRate_; }
  const char* stepRateName() const;
  uint8_t speakerVolume() const { return speakerVolume_; }
  uint8_t swingPercent() const { return swingPercent_; }
  bool swingActive() const;

  bool decreaseTempo();
  bool increaseTempo();
  bool decreaseRate();
  bool increaseRate();
  bool decreaseVolume();
  bool increaseVolume();
  bool decreaseSwing();
  bool increaseSwing();

 private:
  uint16_t tempoBpm_ = 120;
  StepRate stepRate_ = StepRate::Sixteenth;
  uint8_t speakerVolume_ = 128;
  uint8_t swingPercent_ = MIN_SWING_PERCENT;
};
