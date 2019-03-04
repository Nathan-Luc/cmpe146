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
xSemaphoreHandle binary_sem;
void GpioInterruptCallback();   
void isr_toggle();
void sem_task(void *p);
LabGPIO Button3(0,29);
LabGPIO led3(1, 18);
int main() 
{ 
  LOG_INFO("Start");
  
  // 1) Setup a GPIO on port 2 as an input
  // 2) Configure the GPIO registers to trigger an interrupt on P2.0 rising edge. 
  // 3) Register your callback for the EINT3
  
  /*LabGPIO Int(0,29);
  Int.SetAsInput();
  LPC_GPIOINT -> IO0IntEnR = (1<<29);
  RegisterIsr(GPIO_IRQn, GpioInterruptCallback);
  */
  vSemaphoreCreateBinary(binary_sem);
  
  //gpio.SetAsInput();
  Button3.AttachInterruptHandler(&isr_toggle,LabGPIO::Edge::kFalling);
  Button3.EnableInterrupts();
  xTaskCreate(sem_task, (const char*)"t",1024, NULL, 1, NULL);
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
void isr_toggle(){
    LOG_INFO("In the interrupt");
    Delay(50);
    xSemaphoreGiveFromISR(binary_sem,NULL);
     LPC_GPIOINT->IO0IntClr = (1<<29);
    
}
void sem_task(void *p){
      while(1){
      if(xSemaphoreTake(binary_sem,99999999))
        {   
            LOG_INFO("Semaphore Recieved\n");
            led3.toggle();
        }
        else LOG_INFO("NOT TICK\n");
        }
}