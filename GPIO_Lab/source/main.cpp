#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>

#include <cstdint>
#include <iterator>


#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
int main() 
{
  /*Lab 0
  // 1) Find and choose an onboard LED to manipluate.
  // 2) Use the schematic to figure out which pin it is connected to
  // 3) Use FIODIR to set that pin as an output
   LPC_GPIO2->PIN |= (1 << 3);
   LPC_GPIO2->DIR |= (1 << 3);
  while (true)
  {
    // 4) use FIOCLR to set the pin LOW, turning ON the LED
    LPC_GPIO2->CLR = (1 << 3);
    LOG_INFO("Turning LED ON!");
    Delay(500);  // Delay in milliseconds
    // 5) use FIOSET to set the pin HIGH, turning OFF the LED
    LPC_GPIO2->SET = (1 << 3);
    LOG_INFO("Turning LED OFF!");
    Delay(500);
  }
  return 0;
  */
  
  /*Lab3*/
  LabGPIO button3(0, 29);
  LabGPIO led3(1, 18);
  
  LabGPIO button2(0, 30);
  LabGPIO led2(1, 24);
  
  LabGPIO button1(1, 15);
  LabGPIO led1(1, 26);
  
  LabGPIO button0(1, 19);
  LabGPIO led0(2, 3);
  int count=0;
  // Initialize button and led here

  while(true)
  {
       if(button3.ReadBool()){
        
         if(button3.ReadBool() != true)
       {
            led3.toggle();
       }           
       }
     
    // Logic to read if button has been RELEASED and if so, TOGGLE LED state;
  }
  return 0;
     
}
