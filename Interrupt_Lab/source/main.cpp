#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>
#include "L0_LowLevel/interrupt.hpp"
#include <cstdint>
#include <iterator>
#include "L3_Application/oled_terminal.hpp"

#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
void GpioInterruptCallback();   

int main() 
{ 
  LOG_INFO("Start");
  
  // 1) Setup a GPIO on port 2 as an input
  // 2) Configure the GPIO registers to trigger an interrupt on P2.0 rising edge. 
  // 3) Register your callback for the EINT3
  LabGPIO Int(0,29);
  Int.SetAsInput();
  LPC_GPIOINT -> IO0IntEnR = (1<<29);
  RegisterIsr(GPIO_IRQn, GpioInterruptCallback);
  
  while(true)
  {    
    
  }
  return 0;
     
}
void GpioInterruptCallback()
{
    LOG_INFO("In the interrupt");
    Delay(5);
  // 4) For the callback, do anything such as printf or blink and LED here to test your ISR
  // 5) MUST! Clear the source of the GPIO interrupt
    LPC_GPIOINT->IO0IntClr = (1<<29);
}