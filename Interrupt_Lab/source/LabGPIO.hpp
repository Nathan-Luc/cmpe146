#pragma once 
#include <cstdint>
#include "L0_LowLevel/interrupt.hpp"
#include "L0_LowLevel/LPC40xx.h"
#include "pinconn.hpp"
#include "utility/log.hpp"
#include "utility/time.hpp"

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
  enum class Edge
  {
    kNone = 0,
    kRising,
    kFalling,
    kBoth
  };
  static constexpr size_t kPorts = 2;
  static constexpr size_t kPins = 32; 

  void AttachInterruptHandler(IsrPointer isr, Edge edge);
  LabGPIO(uint8_t port, uint8_t pin);
  void SetAsInput();  /// Sets this GPIO as an input
  void SetAsOutput();/// Sets this GPIO as an output
  void SetDirection(Direction direction); 
  void SetHigh();  /// Set voltage of pin to HIGH
  void SetLow(); /// Set voltage of pin to LOW 
  /// Set pin state to high or low depending on the input state parameter.
  /// Has no effect if the pin is set as "input".
  /// @param state - State::kHigh => set pin high, State::kLow => set pin low
  void set(State state);
  /// Should return the state of the pin (input or output, doesn't matter)
  /// @return level of pin high => true, low => false
  State Read();
  /// Should return the state of the pin (input or output, doesn't matter)
  /// @return level of pin high => true, low => false
  bool ReadBool();
  void toggle(); //toggle LED light
  void setPulldown();// setPulldown resistors
  void setPullup(); // setPullup resistors
  void setInactive(); // make pin inactive resistors
  void setRepeater(); // set resistor to repeat state
  void IntEdge(Edge edge);
 private:
   // Statically allocated a lookup table matrix here of function pointers 
  // to avoid dynamic allocation.
  // 
  // Upon AttachInterruptHandler(), you will store the user's function callback
  // in this matrix.
  //
  // Upon the GPIO interrupt, you will use this matrix to find and invoke the
  // appropriate callback.
  //
  // Initialize everything to nullptr.
  static  IsrPointer pin_isr_map[kPorts][kPins];

  static void GpioInterruptHandler();
  
  static void EnableInterrupts();
    uint8_t SelPort;
    uint8_t SelPin;
    uint8_t intport;
    IsrPointer map;
    pinconn *pc;
  /// port, pin and any other variables should be placed here.
  /// NOTE: Pin state should NEVER be cached! Always check the hardware
  ///       registers for the actual value of the pin.
};