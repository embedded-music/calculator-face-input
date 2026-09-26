#include "SequencerSettings.h"

#include <cstddef>

namespace {
struct RateRatio {
  uint8_t beatsNumerator;
  uint8_t beatsDenominator;
  const char* name;
};

constexpr RateRatio RATE_RATIOS[] = {
    {4, 1, "1/1"},       {2, 1, "1/2"},       {1, 1, "1/4"},
    {2, 3, "1/4T"},      {1, 2, "1/8"},       {1, 3, "1/8T"},
    {1, 4, "1/16"},      {1, 6, "1/16T"},     {1, 8, "1/32"},
    {1, 12, "1/32T"},
};

constexpr size_t RATE_COUNT = sizeof(RATE_RATIOS) / sizeof(RATE_RATIOS[0]);
}  // namespace

uint64_t SequencerSettings::stepIntervalUs() const {
  const RateRatio& ratio = RATE_RATIOS[static_cast<size_t>(stepRate_)];
  return (60000000ULL * ratio.beatsNumerator) /
         (static_cast<uint64_t>(tempoBpm_) * ratio.beatsDenominator);
}

const char* SequencerSettings::stepRateName() const {
  return RATE_RATIOS[static_cast<size_t>(stepRate_)].name;
}

bool SequencerSettings::decreaseTempo() {
  const uint16_t previous = tempoBpm_;
  tempoBpm_ = tempoBpm_ > MIN_TEMPO_BPM + TEMPO_INCREMENT_BPM
                  ? tempoBpm_ - TEMPO_INCREMENT_BPM
                  : MIN_TEMPO_BPM;
  return tempoBpm_ != previous;
}

bool SequencerSettings::increaseTempo() {
  const uint16_t previous = tempoBpm_;
  tempoBpm_ = tempoBpm_ < MAX_TEMPO_BPM - TEMPO_INCREMENT_BPM
                  ? tempoBpm_ + TEMPO_INCREMENT_BPM
                  : MAX_TEMPO_BPM;
  return tempoBpm_ != previous;
}

bool SequencerSettings::decreaseRate() {
  const uint8_t index = static_cast<uint8_t>(stepRate_);
  if (index == 0) return false;
  stepRate_ = static_cast<StepRate>(index - 1);
  return true;
}

bool SequencerSettings::increaseRate() {
  const uint8_t index = static_cast<uint8_t>(stepRate_);
  if (index + 1 >= RATE_COUNT) return false;
  stepRate_ = static_cast<StepRate>(index + 1);
  return true;
}

bool SequencerSettings::decreaseVolume() {
  const uint8_t previous = speakerVolume_;
  speakerVolume_ = speakerVolume_ > VOLUME_INCREMENT
                       ? speakerVolume_ - VOLUME_INCREMENT
                       : 0;
  return speakerVolume_ != previous;
}

bool SequencerSettings::increaseVolume() {
  const uint8_t previous = speakerVolume_;
  speakerVolume_ = speakerVolume_ < UINT8_MAX - VOLUME_INCREMENT
                       ? speakerVolume_ + VOLUME_INCREMENT
                       : UINT8_MAX;
  return speakerVolume_ != previous;
}
