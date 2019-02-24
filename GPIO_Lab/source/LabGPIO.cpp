

#include "LabGPIO.hpp"
 
  LabGPIO::LabGPIO(uint8_t port, uint8_t pin){
    SelPort = port;
    SelPin = pin;
 
  }
  void LabGPIO::SetAsInput(){
  /// Sets this GPIO as an input
  switch(SelPort){
    case 0:
       LPC_GPIO0-> DIR &= ~(0<<SelPin);
       break;
    case 1:
       LPC_GPIO1-> DIR &= ~(0<<SelPin);
       break;
    case 2: 
       LPC_GPIO2-> DIR &= ~(0<<SelPin);
       break;
    case 3:
       LPC_GPIO3-> DIR &= ~(0<<SelPin);
       break;
    case 4:
       LPC_GPIO4-> DIR &= ~(0<<SelPin);
        break;
    case 5:
       LPC_GPIO5-> DIR &= ~(0<<SelPin);
       break;
  }
  }

  void LabGPIO::SetAsOutput(){
    /// Sets this GPIO as an output
      switch(SelPort){
    case 0:
       LPC_GPIO0-> DIR |= (1<<SelPin);
       break;
    case 1:
       LPC_GPIO1-> DIR |= (1<<SelPin);
       break;
    case 2:
       LPC_GPIO2-> DIR |= (1<<SelPin);
       break;
    case 3:
       LPC_GPIO3-> DIR |= (1<<SelPin);
       break;
    case 4:
       LPC_GPIO4-> DIR |= (1<<SelPin);
        break;
    case 5:
       LPC_GPIO5-> DIR |= (1<<SelPin);
       break;
  }
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
     switch(SelPort){
    case 0:
       LPC_GPIO0-> SET = (1<<SelPin);
       break;
    case 1:
       LPC_GPIO1-> SET = (1<<SelPin);
       break;
    case 2:
       LPC_GPIO2-> SET = (1<<SelPin);
       break;
    case 3:
       LPC_GPIO3-> SET = (1<<SelPin);
       break;   
    case 4:
       LPC_GPIO4-> SET = (1<<SelPin);
        break;
    case 5:
       LPC_GPIO5-> SET = (1<<SelPin);
       break;
  }
  }
  void LabGPIO::SetLow(){
        /// Set voltage of pin to LOW 
    switch(SelPort){
    case 0:
       LPC_GPIO0-> CLR = (1<<SelPin);
       break;
    case 1:
       LPC_GPIO1-> CLR = (1<<SelPin);
       break;
    case 2:
       LPC_GPIO2-> CLR = (1<<SelPin);
       break;
    case 3:
       LPC_GPIO3-> CLR = (1<<SelPin);
       break;
    case 4:
       LPC_GPIO4-> CLR = (1<<SelPin);
        break;
    case 5:
       LPC_GPIO5-> CLR = (1<<SelPin);
       break;
  }
  
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
    switch(SelPort){
    case 0: 
        if(LPC_GPIO0->PIN & (1<<SelPin)) return State::kHigh;
        else return State::kLow;
        
    case 1: 
        if(LPC_GPIO1->PIN & (1<<SelPin)) return State::kHigh;
        else return State::kLow;
        
    
    case 2:
        if(LPC_GPIO2->PIN & (1<<SelPin)) return State::kHigh;
        else return State::kLow;
       
    case 3:
        if(LPC_GPIO3->PIN & (1<<SelPin)) return State::kHigh;
        else return State::kLow;
        
    case 4:
        if(LPC_GPIO4->PIN & (1<<SelPin)) return State::kHigh;
        else return State::kLow;
        
    case 5:
        if(LPC_GPIO5->PIN & (1<<SelPin)) return State::kHigh;
        else return State::kLow;
    }
  }
    bool LabGPIO::ReadBool(){
  /// Should return the state of the pin (input or output, doesn't matter)
  ///
  /// @return level of pin high => true, low => false
  switch(SelPort){
    case 0:
        return(LPC_GPIO0->PIN & (1<<SelPin));
    case 1:
        return(LPC_GPIO1->PIN & (1<<SelPin));
    case 2:
        return(LPC_GPIO2->PIN & (1<<SelPin));
    case 3:
        return(LPC_GPIO3->PIN & (1<<SelPin));
    case 4:
        return(LPC_GPIO4->PIN & (1<<SelPin));
    case 5:
        return(LPC_GPIO5->PIN & (1<<SelPin));  
  }
  }
    void LabGPIO::toggle(){

    switch(SelPort){
    case 0:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
    
    case 1:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
    
    case 2:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
    
    case 3:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
    
    case 4:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
    
    case 5:
        if(!ReadBool()) //reads the current state and if it is not true which means it is SetLow then SetHigh 
            SetHigh(); 
        else SetLow();
    }
  }
  
   