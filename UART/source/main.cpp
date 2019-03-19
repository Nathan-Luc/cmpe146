#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>
#include "L0_LowLevel/interrupt.hpp"
#include <cstdint>
#include <iterator>
#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
#include "pinconn.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"
#include "L3_Application/oled_terminal.hpp"
//#include "LabUART.hpp"

/*Part 0*/
QueueHandle_t Global_Queue_Handle = 0;
pinconn pc;
OledTerminal oled_terminal;
uint8_t index =0;
int val = 0;
void vReceiveByteOverUartTask(void * pvParamater);
void Uart2Receive();
void Uart2Interrupt();
void InitializeUart2();
void Uart2Send(char num);
void Uart_queue();


//LabUART uart_rx(2);

int main() 
{   
  char hold = '+';
  LOG_INFO("Start\n");
  Global_Queue_Handle = xQueueCreate(10,sizeof(10));
  xTaskCreate(vReceiveByteOverUartTask,(const char*) "receive", 4096, NULL, 1 , NULL);
  oled_terminal.Initialize();
  InitializeUart2();
  Uart2Send(hold);
  Uart2Send(7);
  Uart2Send(5);
  vTaskStartScheduler();
 
  
}
void InitializeUart2()
{   //Power On
    LPC_SC->PCONP |= (1<<24); 
    LPC_UART2->LCR |= (1<<7);
    //Enable FIFO
    LPC_UART2->FCR |= (1<<0);
    //SetBaud Rate
    LPC_UART2->DLL |= 0x38;
    LPC_UART2->DLM |= 0x1;
    //Set Pins
    pc.uart2_txd(2,8);
    pc.uart2_rxd(2,9);
    LPC_UART2->LCR &= ~(0b11 <<2);
    LPC_UART2->LCR |= (0b11<<0);
    // Disable for interrupt
    LPC_UART2->LCR &= ~(1<<7);
    //Reset FIFOs
    LPC_UART2->FCR |= (0b11 << 1);
    //Set TER
    LPC_UART2->TER |= (0b1<<7);
    //Set Receive Data
    LPC_UART2->IER |= (1 << 0);
  // Init PINSEL, baud rate, frame size, etc. 
    //NVIC_EnableIRQ(UART2_IRQn);
    RegisterIsr(UART2_IRQn, Uart2Interrupt);
}

void Uart2Send(char num)
{   
    
        while(!(LPC_UART2->LSR & (1 << 5) ) );
        char char_val = 0;
        char char_val2 = 0;
        char_val2 = num;
        char_val = num + '0';
        if(char_val >= '0' && char_val <= '9') 
        LPC_UART2->THR = char_val;
        else 
        LPC_UART2->THR = char_val2;
        oled_terminal.printf("Letter sent\n");
       
   
}
void Uart2Receive()
{   
    char array[1];
    array[0]= LPC_UART2->RBR;
    oled_terminal.printf("Data: %c\n",array[0]);
    
}
void Uart2Interrupt()
{
  char x=0;
  oled_terminal.printf("In Here\n");
  x = LPC_UART2->RBR;
  
  xQueueSendFromISR(Global_Queue_Handle, &x, NULL);
}
void vReceiveByteOverUartTask(void * pvParamater)
{
  char data = 0;
  char Rx_x[]={};
  uint8_t index = 0;
  while (true) 
  {
    if (xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY)) 
    {
      //printf("Here in Task\n");
     // printf("Data : %c\n" , Rx_x);
      Rx_x[index] = data;
      index++;
    }
    if(index == 3)
    {
        for(int i = 0; i<3; i++)
        {
            printf("Recieve Data: %c\n",Rx_x[i]);
        }
    }
  }
}