#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>
#include "L0_LowLevel/interrupt.hpp"
#include <cstdint>
#include <iterator>
#include "L3_Application/oled_terminal.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
//Declare a Semaphore
xSemaphoreHandle binary_sem;
xSemaphoreHandle binary_sema;

//Functions
void GpioInterruptCallback();   
void isr_toggle();
void isr_toggle2();
void led_task(void *p);

//Buttons
LabGPIO Button3(0,29);
LabGPIO led3(1, 18);
LabGPIO Button2(0,30);
LabGPIO led2(1, 24);

int main() 
{ 
  LOG_INFO("Start");
  /*Part 0
  // 1) Setup a GPIO on port 2 as an input
  // 2) Configure the GPIO registers to trigger an interrupt on P2.0 rising edge. 
  // 3) Register your callback for the EINT3
  LabGPIO Int(0,29);
  Int.SetAsInput();
  LPC_GPIOINT -> IO0IntEnR = (1<<29);
  RegisterIsr(GPIO_IRQn, GpioInterruptCallback);
  */
  
   //Creates Semaphore
  vSemaphoreCreateBinary(binary_sem);
  vSemaphoreCreateBinary(binary_sema)
  
  //Creates Button to Isr
  Button2.AttachInterruptHandler(&isr_toggle,LabGPIO::Edge::kFalling);
  Button2.EnableInterrupts();
  Button3.AttachInterruptHandler(&isr_toggle2,LabGPIO::Edge::kRising);
  Button3.EnableInterrupts();
  
  //Create and start Tast
  xTaskCreate(led_task, (const char*)"t",1024, NULL, 1, NULL);
  vTaskStartScheduler();
  
  while(true)
  {    
    
  }
  return 0;
     
}
void GpioInterruptCallback()
{
    LOG_INFO("In the interrupt");
    Delay(500);
  // 4) For the callback, do anything such as printf or blink and LED here to test your ISR
  // 5) MUST! Clear the source of the GPIO interrupt
    LPC_GPIOINT->IO0IntClr = (1<<29);
}
//Sends Semaphore and clears the Interrupt
void isr_toggle(){
    LOG_INFO("In the interrupt");
    Delay(50);
    xSemaphoreGiveFromISR(binary_sem,NULL);
    Button2.ClrEdge(LabGPIO::Edge::kFalling);
    
}
void isr_toggle2(){
    LOG_INFO("In the second interrupt\n");
    Delay(50);
    xSemaphoreGiveFromISR(binary_sema,NULL);
    Button3.ClrEdge(LabGPIO::Edge::kRising);
}

//Task that takes interrupt and provides a function
void led_task(void *p){
      while(1){
      if(xSemaphoreTake(binary_sem,1000)) //waits 1 sec for a semaphore to come
        {   
            LOG_INFO("Semaphore Recieved\n");
            led2.toggle();
        }
        else LOG_INFO("Interrupt 1 Not Recieved\n");
      if(xSemaphoreTake(binary_sema, 1000))
      {
          LOG_INFO("Semaphore 2 Recieved\n");
          led3.toggle();
      }
        else LOG_INFO("Interrupt 2 Not Recieved\n");
        }
}