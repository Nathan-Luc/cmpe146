#include "LabUART.hpp"
QueueHandle_t LabUART::Global_Queue_Handle;

inline static LPC_UART_TypeDef * UART[2] = {LPC_UART2, LPC_UART3};

LabUART::LabUART(uint8_t Uart){
    selUart = Uart-2;
    //MINUS 2 DONE TO USE CASE 0 AND 1. NOTHING ELSE
}

void LabUART::InitializeUart(){
    switch(selUart){
       case 0:
        LPC_SC->PCONP |= (1<<24);       //POWER: UM10562- Page 30. Pin 24 for UART2
        pc.uart2_txd(2,8);              //Pin 2.8 SET for TX
        pc.uart2_rxd(2,9);              //Pin 2.9 SET for RX
        RegisterIsr(UART2_IRQn,RX_Int);
        break;

       case 1:
        LPC_SC->PCONP |= (1<<25);       //POWER: UM10562- Page 30. Pin 24 for UART2
        pc.uart3_txd(4,28);              //Pin 2.8 SET for TX
        pc.uart3_rxd(4,29);              //Pin 2.9 SET for RX
        RegisterIsr(UART3_IRQn, RX_Int);
        break;
   }

    oled_terminal.Initialize();
    UART[selUart]->LCR |= (1<<7);       //SET DLAB (DIVISOR LATCH ACCESS BIT)
    UART[selUart]->FCR |= (1<<0);       //BIT 0: FIFO ENABLE. UM 10562 Page 501 (UARTn FIFO Control Register)
    setBaudrate9600();                  //SET DLL & DLM

    UART[selUart]->LCR &= ~(0b11 <<2);  //RESET UP STOP BIT SELECT(2) & PARITY BIT (3)
    UART[selUart]->LCR |= (0b11<<0);    //SET UP STIOP BIT SELECT(2) & PARITY BIT (3)
    UART[selUart]->LCR &= ~(1<<7);      //RESET DLAB (DIVISOR LATCH ACCESS BIT)
    UART[selUart]->FCR |= (0b11 << 1);  //RXFIFO-RESET & TXFIFORESET. UM 10562 Page 511 (UARTn FIFO Control Register)
    UART[selUart]->TER |= (0b1<<7);     //TER (Transmit Enable Register) UM 10562 Page 501 (UARTn Transmit Enable Register)
                                        /*When this bit is 1, as it is after a Reset,
                                        data written to the THR is output on the TXD pin as soon as any preceding data has been sent.*/
    UART[selUart]->IER |= (1 << 0);     //DISABLE IER (INTERRUPT ENABLE REGISTER) UM 10562 Page 498 (UARTn Interrupt Enable Register)
    Uqueue();                           //SET UP UART QUEUE
   
  }

//DLL: Divisor Latch LSB register
//DLM: Divisor Latch MSB register
/*SETS UP DLL AND DLM BY CALCULATION*/
void LabUART::setBaudrate9600(){
    //DivAddVal = 0 & MulVal = 1
    //Lets say Div = 16 x (256 x UnDLM + UnDLL)
    //(Pclk) / (16 x UART) = Div
    //Pclk = 48 MHz
    const uint32_t baudrate=9600;
    uint16_t divisor = 48000000 / (16*baudrate);
    //SetBaud Rate
    //DLL: 16 bits, DLL: LOWER 8-bits, DLM: UPPER 8-bits
    UART[selUart]->DLL |= divisor;
    UART[selUart]->DLM |= (divisor>>8);
}

//SENDING DATA
void LabUART::UartSend(int data){
    //Wait while the THR (Transmit Holding Register) is not empty.
    while(!(UART[selUart]->LSR & (1 << 5)));    //UM 10562 Page 503 (UARTn Line Status Register)
                                                //BIT 5: THRE (Transmit Holding Register Empty)
    //Send data to THR
    UART[selUart]->THR = data;
}

//RECEIVING DATA
//@returns  received data
uint8_t LabUART::UartReceive(){
    uint8_t Rdata =0;
    //IF FIFO not Empty
    if(UART[selUart]->LSR & (1<<0)){          //UM 10562 Page 503 (UARTn Line Status Register)
                                              //BIT 1: RDR (Receive Data Ready) If 1, FIFO is NOT EMPTY!
        Rdata= UART[selUart]->RBR;
    }
    return Rdata;
}


//TASK FOR RECEIVING BYTE OVER UART
void LabUART::vReceiveByteOverUartTask(void * pvParamater){
    oled_terminal.Initialize();
    uint8_t data = 0;
    int Rx_x[3];      //INTEGER ARRAY TO HOLD THREE VALUES
    int res=0;        //RESULT TO BE CALCULATED FROM ALU
    int index = 0;    //INDEX POINTER TO ARRAY Rx_x
    while (true){
        if (xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY)){
            Rx_x[index] = data;
            oled_terminal.printf("Data: %i\n", Rx_x[index]);
            index++; 
        }

        //Once both values and operands have been received, send data to ALU for Calculation
        //Index 0, 1 & 2: Data1, Data2, Operand
        if(index == 3){
            res = ALU(Rx_x);
            oled_terminal.printf("Result: %i\n", res);
            index = 0;  //RESET INDEX
        }
    }
}

//RECEIVER INTERRUPT
void LabUART::RX_Int(){
    uint8_t idata = 0;
    idata = UartReceive();
    xQueueSendFromISR(Global_Queue_Handle, &idata, NULL);
}

//PERFORM ARITHMETIC OPERATION
//@param    array is array passed to the function that has two numbers and one operator
//@returns  added, subtracted or multipled result of two integers in array
int LabUART::ALU(int *array){
    int num1, num2;   //initialize two input numbers
    int result = 0;   //initialize output variable result
    num1 = array[0];  //First number is first element in the array.
    num2 = array[1];  //Second number is second element in the array.

    switch(static_cast<char>(array[2])){
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
