#include "LabGPIO.hpp"

    inline static LPC_GPIO_TypeDef * GPIO[6] = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4, LPC_GPIO5};
    inline static volatile uint32_t  * interrupts[2][5] =
     {{
        &LPC_GPIOINT->IO0IntStatR,
        &LPC_GPIOINT->IO0IntStatF,
        &LPC_GPIOINT->IO0IntClr,
        &LPC_GPIOINT->IO0IntEnR,
        &LPC_GPIOINT->IO0IntEnF,
    },
    {
        &LPC_GPIOINT->IO2IntStatR,
        &LPC_GPIOINT->IO2IntStatF,
        &LPC_GPIOINT->IO2IntClr,
        &LPC_GPIOINT->IO2IntEnR,
        &LPC_GPIOINT->IO2IntEnF,
     }};
    //inline static volatile uint32_t *stat= &(LPC_GPIOINT->IntStatus);
    
    IsrPointer LabGPIO::pin_isr_map[kPorts][kPins]={ nullptr};
    
 LabGPIO::LabGPIO(uint8_t port, uint8_t pin){
    SelPort = port;
    SelPin = pin;
    
    interrupt_port = (port == 2) ? 1:0;
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
void LabGPIO::setPulldown(){ 
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
 void LabGPIO::AttachInterruptHandler(IsrPointer isr, Edge edge){
    pin_isr_map[interrupt_port][SelPin]=isr;
    //printf("%i %i\n",interrupt_port,SelPin);
    IntEdge(edge);
  
 }
 void LabGPIO::GpioInterruptHandler(){
    uint32_t selPin_local;

  
    if((LPC_GPIOINT->IO0IntStatR) | (LPC_GPIOINT->IO0IntStatF)){
    selPin_local = __builtin_ctz(LPC_GPIOINT->IO0IntStatR | LPC_GPIOINT->IO0IntStatF);
    //printf("%i\n",selPin_local);
    pin_isr_map[0][selPin_local]();
    LPC_GPIOINT -> IO0IntClr &= ~(1<< selPin_local);
    }
    
   
  
    if((LPC_GPIOINT->IO2IntStatR) | (LPC_GPIOINT->IO2IntStatF)) {
    selPin_local = __builtin_ctz((LPC_GPIOINT->IO2IntStatR)|(LPC_GPIOINT->IO2IntStatF));
    pin_isr_map[1][selPin_local]();
    LPC_GPIOINT -> IO2IntClr &= ~(1<< selPin_local);
    }
 }
 
void LabGPIO::EnableInterrupts(){
  // Register GPIO_IRQn here
        RegisterIsr(GPIO_IRQn, GpioInterruptHandler );
 }
 
 void LabGPIO::IntEdge(Edge edge){
    switch(edge)
    {   case Edge::kNone:
                LOG_INFO("No edge selected");
                break;
        case Edge::kRising:
            setRisingEdge();
            break;
        case Edge::kFalling:
            setFallingEdge();
            break;
        case Edge::kBoth:
            setRisingEdge();
            setFallingEdge();
            break;   
    }
 }
 void LabGPIO::setFallingEdge(){
    *interrupts[interrupt_port][4] = (1<<SelPin);
 }
 void LabGPIO::setRisingEdge(){
    *interrupts[interrupt_port][3] = (1<<SelPin);
 }
 void LabGPIO::ClrFallingEdge(){
    *interrupts[interrupt_port][4] = (1<<SelPin);
 }
 void LabGPIO::ClrRisingEdge(){
    *interrupts[interrupt_port][3] = (1<<SelPin);
 }
 