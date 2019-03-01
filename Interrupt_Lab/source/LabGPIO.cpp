#include "LabGPIO.hpp"

    inline static LPC_GPIO_TypeDef * GPIO[6] = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4, LPC_GPIO5};
 
  LabGPIO::LabGPIO(uint8_t port, uint8_t pin){
    SelPort = port;
    SelPin = pin;
 
  }
  void LabGPIO::SetAsInput(){
  /// Sets this GPIO as an input
  
  GPIO[SelPort]->DIR &= ~(0<<SelPin);
  
  }

  void LabGPIO::SetAsOutput(){
    /// Sets this GPIO as an output
       GPIO[SelPort]-> DIR |= (1<<SelPin);
     
  }
  void LabGPIO::SetDirection(Direction direction){
  /// Sets this GPIO as an input
  /// @param output - true => output, false => set pin to input
  if(direction == Direction::kOutput) SetAsOutput();
  else SetAsInput();
   }
  

  void LabGPIO::SetHigh(){
       /// Set voltage of pin to HIGH
        GPIO[SelPort]-> SET = (1<<SelPin);
    
  }
  void LabGPIO::SetLow(){
        /// Set voltage of pin to LOW
            GPIO[SelPort]-> CLR = (1<<SelPin);
   
  }
  void LabGPIO::set(State state){
      /// Set pin state to high or low depending on the input state parameter.
  /// Has no effect if the pin is set as "input".
  ///
  /// @param state - State::kHigh => set pin high, State::kLow => set pin low
  if((GPIO[SelPort]->DIR) && (state == State::kHigh)) SetHigh();
  else SetLow();
  
  }
  LabGPIO::State LabGPIO::Read(){
       /// Should return the state of the pin (input or output, doesn't matter)
  ///
      /// @return level of pin high => true, low => false
     
     return (GPIO[SelPort]->PIN & (1<<SelPin)) ? State::kHigh : State::kLow;
    
  }
 bool LabGPIO::ReadBool(){
  /// Should return the state of the pin (input or output, doesn't matter)
  ///
  /// @return level of pin high => true, low => false
  return(GPIO[SelPort]->PIN & (1<<SelPin));
 
  
  }
 void LabGPIO::toggle(){
       if(!ReadBool())
           SetHigh();
       else SetLow();
   
  }
  void LabGPIO::setPulldown(){ //this and functions below only work for the 4 switches on the SJ2 board
      pc->pc_pulldown(SelPort, SelPin);

  }
void LabGPIO::setPullup(){
    pc->pc_pullup(SelPort, SelPin);
  }
void LabGPIO::setInactive(){
    pc->pc_inactive(SelPort, SelPin);
  }
void LabGPIO::setRepeater(){
   pc->pc_repeater(SelPort, SelPin);
  }