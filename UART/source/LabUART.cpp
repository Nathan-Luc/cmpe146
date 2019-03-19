#include "LabUART.hpp"
QueueHandle_t LabUART::Global_Queue_Handle;
inline static LPC_UART_TypeDef * UART[2] = {LPC_UART2, LPC_UART3};

LabUART::LabUART(uint8_t Uart){
    selUart = Uart-2;
}
void LabUART::RX_Int(){
    char rx_data;
    rx_data = LPC_UART2->RBR;
    if(xQueueSendFromISR(Global_Queue_Handle, &rx_data, NULL))
    {
        
    }
    
}
void LabUART::InitializeUart(){
    switch(selUart)
   {
       case 0:
       LPC_SC->PCONP |= (1<<24); 
        pc.uart2_txd(2,8);
        pc.uart2_rxd(2,9);
        RegisterIsr(UART2_IRQn,RX_Int);
       case 1:
       LPC_SC->PCONP |= (1<<25); 
        pc.uart3_txd(2,8);
        pc.uart3_rxd(2,9);
        RegisterIsr(UART3_IRQn, RX_Int);
   }
    UART[selUart]->LCR |= (1<<7);
    UART[selUart]->FCR |= (1<<0);
    setBaudrate9600();
    UART[selUart]->LCR &= ~(0b11 <<2);
    UART[selUart]->LCR |= (0b11<<0);
    UART[selUart]->LCR &= ~(1<<7);
    UART[selUart]->FCR |= (0b11 << 1);
    UART[selUart]->TER |= (0b1<<7);
    UART[selUart]->IER |= (1 << 0);
   
  }
void LabUART::setBaudrate9600(){
     const uint32_t baudrate=9600;
     uint16_t divisor = 48000000 / (16*baudrate);
       //SetBaud Rate
    UART[selUart]->DLL |= divisor;
    UART[selUart]->DLM |= (divisor>>8);
}
void LabUART::UartSend(char value){
        

    while(!(UART[selUart]->LSR & (1 << 5) ) );
        char char_val = 0;
        char char_val2 = 0;
        char_val2 = value;
        char_val = value + '0';
        if(char_val >= '0' && char_val <= '9') 
        UART[selUart]->THR = char_val;
        else 
        UART[selUart]->THR = char_val2;
       
 //       oled_terminal.printf("Letter sent\n");
}
void LabUART::UartReceive(){
     OledTerminal oled;
    char array[3];
    char data;
    uint8_t alu_result=0;
    uint8_t index=0;
    while(1)
    {
        if(xQueueReceive(Global_Queue_Handle, &data, portMAX_DELAY)) 
        {
            array[index++] = data;
            vTaskDelay(500);
            
        }
        if(index == 3)
        {
            alu_result = ALU(array);
            printf("Result = %x\n", alu_result);
        }
    }
    oled.printf("Data: %s\n",array);
    
}
int LabUART::ALU(char *array){
    int num1, num2;
    int result = 0;
    num1 = array[0] - '0';
    num2 = array[1] - '0';
    switch(array[3])
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
    
    //send(
}
