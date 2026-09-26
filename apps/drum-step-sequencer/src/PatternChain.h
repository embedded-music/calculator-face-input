#pragma once

#include <stdint.h>

constexpr uint8_t CHAIN_MIN_LENGTH = 1;
constexpr uint8_t CHAIN_MAX_LENGTH = 12;

class PatternChain {
 public:
  PatternChain();

  uint8_t length() const { return length_; }
  uint8_t currentPosition() const { return currentPosition_; }
  uint8_t nextPosition() const { return nextEnabledPosition(currentPosition_); }
  uint8_t currentPattern() const { return currentPattern_; }
  uint8_t nextPattern() const { return patterns_[nextPosition()]; }
  uint8_t patternAt(uint8_t position) const;
  bool positionEnabled(uint8_t position) const;
  bool positionVisible(uint8_t position) const;

  void advance();
  void setNextPattern(uint8_t pattern);
  bool togglePosition(uint8_t position);

 private:
  uint8_t nextEnabledPosition(uint8_t position) const;

  uint8_t patterns_[CHAIN_MAX_LENGTH]{};
  bool enabled_[CHAIN_MAX_LENGTH]{};
  uint8_t length_ = CHAIN_MIN_LENGTH;
  uint8_t currentPosition_ = 0;
  uint8_t currentPattern_ = 0;
};
