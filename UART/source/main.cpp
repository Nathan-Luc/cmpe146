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
//#include "L3_Application/oled_terminal.hpp"
#include "LabUART.hpp"
#include "L2_HAL/switches/button.hpp"
/*Part 0*/
/*QueueHandle_t Global_Queue_Handle = 0;
pinconn pc;
OledTerminal oled_terminal;
uint8_t index =0;
int val = 0;
void vReceiveByteOverUartTask(void * pvParamater);
uint8_t Uart2Receive();
void Uart2Interrupt();
void InitializeUart2();
void Uart2Send(uint8_t data);
void Uart_queue();
int ALU(int *array);
*/
 int op = 0;
 int num1 =0;
 int num2 =0;
 char plus = '+';
 char sub = '-';
 char mult = '*';
 char operate = 0;
 void EC();
 OledTerminal oled;
  LabGPIO button0(1, 19);
  LabGPIO button1(1, 15);
  LabGPIO button2(0, 30);
  LabGPIO button3(0, 29);
    LabUART uart_rx(2);

int main() 
{   
  
  LOG_INFO("Start\n");
  //Global_Queue_Handle = xQueueCreate(10,sizeof(10));
  xTaskCreate(LabUART::vReceiveByteOverUartTask,(const char*) "receive", 4096, NULL, 1 , NULL);
  oled.Initialize();
  //InitializeUart2();
  EC();
  uart_rx.InitializeUart();
  uart_rx.UartSend(num1);
  uart_rx.UartSend(num2);
  uart_rx.UartSend(operate);
  vTaskStartScheduler();
  //Uart2Receive();
  
}
void EC(){
    
    bool loop = true;
    while(loop)
    {
      if(button0.ReadBool())
    { 
        num1++;
        oled.SetCursor(0,0);
        oled.printf("%i",num1);
        Delay(500);
    }
  
   if(button1.ReadBool())
   {     
   
      op++;
      oled.SetCursor(0,2);
      switch(op){
          
      case 1:
        operate = plus;
        oled.printf("%c",operate);
        break;
      case 2:
        operate = sub;
        oled.printf("%c",operate);
        break;
      case 3:
        operate = mult;
        oled.printf("%c",operate);
        break;
      }
      
       Delay(500);
       op = op%4;
   }
       
   if(button2.ReadBool())
   {        
        num2++;
        oled.SetCursor(0,5);
        oled.printf("%i",num2);
        Delay(500);
   }
   if(button3.ReadBool())
   {     
      
           loop = false;
   }
    }
    
}

/*
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

void Uart2Send(uint8_t data)
{   
    
        while(!(LPC_UART2->LSR & (1 << 5) ) );
        LPC_UART2->THR = data;
        //oled_terminal.printf("udata: %u\n",data);
      
   
}
uint8_t Uart2Receive()
{   
    uint8_t Rdata =0;
    if(LPC_UART2->LSR & (1<<0))
    {
        Rdata= LPC_UART2->RBR;
    }
   // oled_terminal.printf("Rdata: %u\n", Rdata);
    return Rdata;
}
void Uart2Interrupt()
{
  uint8_t idata = 0;
  idata = Uart2Receive();
  xQueueSendFromISR(Global_Queue_Handle, &idata, NULL);
}
void vReceiveByteOverUartTask(void * pvParamater)
{
  uint8_t data = 0;
  int Rx_x[3];
  //int convert = 0;
  int res=0;
  int index = 0;
  while (true) 
  {
    if (xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY)) 
    {
      //printf("Here in Task\n");
      
      //oled_terminal.printf("Data : %u\n" , data);
      Rx_x[index] = data;
      index++;
      
    }
     // ++index;
    //oled_terminal.printf("index: %i\n",index);
    if(index == 3)
    {
       res = ALU(Rx_x);
       oled_terminal.printf("Result: %i\n", res);
       index = 0;
    }
  }
}
int ALU(int *array){
    int num1, num2;
    char convert=0;
   
    int result = 0;
    num1 = array[0];
    num2 = array[1];
    
    //oled_terminal.printf("in here\n");
    switch(static_cast<char>(array[2]))
    {
        case '+':
        result = num1 + num2;
        break;
        
        case '-':
        result = num2 - num1;
        break;
        
        case '*':
        result = num1 * num2;
        break;
    }
  
    Uart2Send(result);
    return result;
}*/
