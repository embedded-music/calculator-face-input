#include "PatternChain.h"

PatternChain::PatternChain() {
  enabled_[0] = true;
}

uint8_t PatternChain::patternAt(uint8_t position) const {
  return position < CHAIN_MAX_LENGTH ? patterns_[position] : 0;
}

bool PatternChain::positionEnabled(uint8_t position) const {
  return position < CHAIN_MAX_LENGTH && enabled_[position];
}

bool PatternChain::positionVisible(uint8_t position) const {
  return positionEnabled(position) || position == currentPosition_;
}

uint8_t PatternChain::nextEnabledPosition(uint8_t position) const {
  for (uint8_t offset = 1; offset <= CHAIN_MAX_LENGTH; offset++) {
    const uint8_t candidate = static_cast<uint8_t>(
        (position + offset) % CHAIN_MAX_LENGTH);
    if (enabled_[candidate]) return candidate;
  }
  return position;
}

void PatternChain::advance() {
  currentPosition_ = nextPosition();
  currentPattern_ = patterns_[currentPosition_];
}

void PatternChain::setNextPattern(uint8_t pattern) {
  patterns_[nextPosition()] = pattern;
}

bool PatternChain::togglePosition(uint8_t position) {
  if (position >= CHAIN_MAX_LENGTH) return false;
  if (enabled_[position] && length_ <= CHAIN_MIN_LENGTH) return false;

  enabled_[position] = !enabled_[position];
  length_ = static_cast<uint8_t>(length_ + (enabled_[position] ? 1 : -1));
  return true;
}
