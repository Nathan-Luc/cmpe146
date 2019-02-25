#pragma once 
#include "pinconn.hpp"
#include <cstdint>
#include "L0_LowLevel/LPC40xx.h"



class LabGPIO
{
 public:
  enum class Direction : uint8_t
  {
    kInput  = 0,
    kOutput = 1
  };
  enum class State : uint8_t
  {
    kLow  = 0,
    kHigh = 1
  };
  /// You should not modify any hardware registers at this point
  /// You should store the port and pin using the constructor.
  ///
  /// @param port - port number between 0 and 5
  /// @param pin - pin number between 0 and 32
  LabGPIO(uint8_t port, uint8_t pin);
  /// Sets this GPIO as an input
  void SetAsInput();
  /// Sets this GPIO as an output
  void SetAsOutput();
  /// Sets this GPIO as an input
  /// @param output - true => output, false => set pin to input
  void SetDirection(Direction direction);
  /// Set voltage of pin to HIGH
  void SetHigh();
  /// Set voltage of pin to LOW 
  void SetLow();
  /// Set pin state to high or low depending on the input state parameter.
  /// Has no effect if the pin is set as "input".
  ///
  /// @param state - State::kHigh => set pin high, State::kLow => set pin low
  void set(State state);
  /// Should return the state of the pin (input or output, doesn't matter)
  ///
  /// @return level of pin high => true, low => false
  State Read();
  /// Should return the state of the pin (input or output, doesn't matter)
  ///
  /// @return level of pin high => true, low => false
  bool ReadBool();
  void toggle();
  void setPulldown();
  void setPullup();
  void setInactive();
  void setRepeater();
 private:
    uint8_t SelPort;
    uint8_t SelPin;
  /// port, pin and any other variables should be placed here.
  /// NOTE: Pin state should NEVER be cached! Always check the hardware
  ///       registers for the actual value of the pin.
};