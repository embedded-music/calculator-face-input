#pragma once

// Distinguishes a short button tap from a hold used for one or more modified
// Calculator presses. No timing threshold is needed: using the modifier is
// what consumes the eventual tap action.
class HeldButtonGesture {
 public:
  void press() {
    pending_ = true;
    consumed_ = false;
  }

  bool modifierActive(bool physicallyPressed) const {
    return pending_ && physicallyPressed;
  }

  void consumeAsModifier() {
    if (pending_) consumed_ = true;
  }

  bool releaseAsTap() {
    const bool tap = pending_ && !consumed_;
    pending_ = false;
    consumed_ = false;
    return tap;
  }

 private:
  bool pending_ = false;
  bool consumed_ = false;
};
