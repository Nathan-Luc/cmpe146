#pragma once
#include "pinconn.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"
#include "L0_LowLevel/interrupt.hpp"
#include "L3_Application/oled_terminal.hpp"
class LabUART{
    public:
      
        LabUART(uint8_t Uart);
        static void UartInterrupt();
        static void InitializeUart();
        static void UartSend(int data);
        static uint8_t UartReceive();
        static void setBaudrate9600();
        static int ALU(int *array);
        static void RX_Int();
        static void vReceiveByteOverUartTask(void * pvParamater);
        static void Uqueue();
     
     private:
    inline static uint8_t selUart;
     inline static pinconn pc;
     inline static OledTerminal oled_terminal;
     static QueueHandle_t Global_Queue_Handle;

};