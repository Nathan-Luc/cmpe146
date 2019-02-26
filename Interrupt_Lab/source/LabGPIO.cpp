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
    switch(SelPort){
  case 0:
    if(direction == Direction::kOutput) SetAsOutput();
    else SetAsInput();
    break;
  case 1:
    if(direction == Direction::kOutput) SetAsOutput();
    else  SetAsInput();
    break;
  case 2:
    if( direction == Direction::kOutput) SetAsOutput();
    else SetAsInput();
    break;
  case 3:
    if(direction == Direction::kOutput) SetAsOutput();
    else  SetAsInput();
    break;
  case 4:
    if(direction == Direction::kOutput) SetAsOutput();
    else SetAsInput();
    break;
  case 5:
    if(direction == Direction::kOutput) SetAsOutput();
    else SetAsInput();
    break;
  }
 
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
  switch(SelPort){
  case 0:
    if( ( LPC_GPIO0->DIR) && (state == State::kHigh)) SetHigh();
    else if( (LPC_GPIO0->DIR) && (state == State::kLow)) SetLow();
    break;
  case 1:
    if( (LPC_GPIO1->DIR ) && (state == State::kHigh)) SetHigh();
    else if((LPC_GPIO1-> DIR) && (state == State::kLow)) SetLow();
    break;
  case 2:
    if( (LPC_GPIO2-> DIR ) && (state == State::kHigh)) SetHigh();
    else if((LPC_GPIO2-> DIR) && (state == State::kLow)) SetLow();
    break;
  case 3:
    if( (LPC_GPIO3-> DIR ) && (state == State::kHigh)) SetHigh();
    else if((LPC_GPIO3-> DIR ) && (state == State::kLow)) SetLow();
    break;
  case 4:
    if( (LPC_GPIO4-> DIR ) && (state == State::kHigh)) SetHigh();
    else if((LPC_GPIO4-> DIR ) && (state == State::kLow)) SetLow();
    break;
  case 5:
    if( (LPC_GPIO5-> DIR ) && (state == State::kHigh)) SetHigh();
    else if((LPC_GPIO5-> DIR ) && (state == State::kLow)) SetLow();
    break;
  }
 
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
       
    switch(SelPort){
    case 0:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
        break;
    case 1:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
        break;
    case 2:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
        break;
    case 3:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
        break;
    case 4:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
        break;
    case 5:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
        break;
    }
  }
  void LabGPIO::setPulldown(){ //this and functions below only work for the 4 switches on the SJ2 board
   switch(SelPort){
   case 0:
    if(SelPin == 29)
   {   
       LPC_IOCON->P0_29 &= ~(0b11<<3);
       LPC_IOCON->P0_29 |= (0b1<<3);
   }
   else if ( SelPin == 30)
   {
       LPC_IOCON->P0_30 &= ~(0b11<<3);
       LPC_IOCON->P0_30 |= (0b1<<3);
   }
     //fall through
   case 1:   
   if(SelPin == 19)
   {   
       LPC_IOCON->P1_19 &= ~(0b11<<3);
       LPC_IOCON->P1_19 |= (0b1<<3);
   }
   else if ( SelPin == 15)
   {
       LPC_IOCON->P1_15 &= ~(0b11<<3);
       LPC_IOCON->P1_15 |= (0b1<<3);
   }
  
   }
  }
void LabGPIO::setPullup(){
   switch(SelPort){
   case 0:
    if(SelPin == 29)
   {   
       LPC_IOCON->P0_29 &= ~(0b11<<3);
       LPC_IOCON->P0_29 |= (0b1<<4);
   }
   else if ( SelPin == 30)
   {
       LPC_IOCON->P0_30 &= ~(0b11<<3);
       LPC_IOCON->P0_30 |= (0b1<<4);
   }
     //fall through
   case 1:   
   if(SelPin == 19)
   {   
       LPC_IOCON->P1_19 &= ~(0b11<<3);
       LPC_IOCON->P1_19 |= (0b1<<4);
   }
   else if ( SelPin == 15)
   {
       LPC_IOCON->P1_15 &= ~(0b11<<3);
       LPC_IOCON->P1_15 |= (0b1<<4);
   }
  
   }
  }
   void LabGPIO::setInactive(){
   switch(SelPort){
   case 0:
    if(SelPin == 29)
   {   
       LPC_IOCON->P0_29 &= ~(0b11<<3);

   }
   else if ( SelPin == 30)
   {
       LPC_IOCON->P0_30 &= ~(0b11<<3);
     
   }
   //fall through
   case 1:   
   if(SelPin == 19)
   {   
       LPC_IOCON->P1_19 &= ~(0b11<<3);
       
   }
   else if ( SelPin == 15)
   {
       LPC_IOCON->P1_15 &= ~(0b11<<3);
       
   }
  
   }
  }
   void LabGPIO::setRepeater(){
   switch(SelPort){
   case 0:
    if(SelPin == 29)
   {   
       LPC_IOCON->P0_29 &= ~(0b11<<3);
       LPC_IOCON->P0_29 |=  (0b11<<3);

   }
   else if ( SelPin == 30)
   {
       LPC_IOCON->P0_30 &= ~(0b11<<3);
       LPC_IOCON->P0_30 |=  (0b11<<3);
     
   }
     //fall through
   case 1:   
   if(SelPin == 19)
   {   
       LPC_IOCON->P1_19 &= ~(0b11<<3);
       LPC_IOCON->P1_19 |=  (0b11<<3);
       
   }
   else if ( SelPin == 15)
   {
       LPC_IOCON->P1_15 &= ~(0b11<<3);
       LPC_IOCON->P1_15 |=  (0b11<<3);
       
   }
    
   }
  }