#include "LabUART.hpp"
QueueHandle_t LabUART::Global_Queue_Handle;

inline static LPC_UART_TypeDef * UART[2] = {LPC_UART2, LPC_UART3};

LabUART::LabUART(uint8_t Uart){
    selUart = Uart-2;
}

void LabUART::InitializeUart(){
    switch(selUart)
   {
       case 0:
       LPC_SC->PCONP |= (1<<24); 
        pc.uart2_txd(2,8);
        pc.uart2_rxd(2,9);
        RegisterIsr(UART2_IRQn,RX_Int);
        break;
       case 1:
       LPC_SC->PCONP |= (1<<25); 
        pc.uart3_txd(2,8);
        pc.uart3_rxd(2,9);
        RegisterIsr(UART3_IRQn, RX_Int);
        break;
   }
    oled_terminal.Initialize();
    UART[selUart]->LCR |= (1<<7);
    UART[selUart]->FCR |= (1<<0);
    setBaudrate9600();
    UART[selUart]->LCR &= ~(0b11 <<2);
    UART[selUart]->LCR |= (0b11<<0);
    UART[selUart]->LCR &= ~(1<<7);
    UART[selUart]->FCR |= (0b11 << 1);
    UART[selUart]->TER |= (0b1<<7);
    UART[selUart]->IER |= (1 << 0);
    Uqueue();
   
  }
void LabUART::setBaudrate9600(){
     const uint32_t baudrate=9600;
     uint16_t divisor = 48000000 / (16*baudrate);
       //SetBaud Rate
    UART[selUart]->DLL |= divisor;
    UART[selUart]->DLM |= (divisor>>8);
}
void LabUART::UartSend(int data){
        while(!(UART[selUart]->LSR & (1 << 5) ) );
        UART[selUart]->THR = data;
}
uint8_t LabUART::UartReceive(){
    uint8_t Rdata =0;
    if(UART[selUart]->LSR & (1<<0))
    {
        Rdata= UART[selUart]->RBR;
    }
    return Rdata;
    
}
void LabUART::vReceiveByteOverUartTask(void * pvParamater){
     oled_terminal.Initialize();
uint8_t data = 0;
  int Rx_x[3];
  int res=0;
  int index = 0;
  while (true) 
  {
    if (xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY)) 
    {
      Rx_x[index] = data;
      oled_terminal.printf("Data: %i\n", Rx_x[index]);
      index++; 
    }
    if(index == 3)
    {
       res = ALU(Rx_x);
       oled_terminal.printf("Result: %i\n", res);
       index = 0;
    }
  }
}
void LabUART::RX_Int(){
  
  uint8_t idata = 0;
  idata = UartReceive();
  xQueueSendFromISR(Global_Queue_Handle, &idata, NULL);
  
}
int LabUART::ALU(int *array){
    int num1, num2;
    int result = 0;
    num1 = array[0];
    num2 = array[1];
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
  
    UartSend(result);
    return result;
}
void LabUART::Uqueue(){
      Global_Queue_Handle = xQueueCreate(10,sizeof(10));
}