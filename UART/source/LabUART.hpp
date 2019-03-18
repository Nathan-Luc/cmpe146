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
        void UartInterrupt();
        void InitializeUart();
        void UartSend(char value);
        static void UartReceive();
        void setBaudrate9600();
        int ALU(char *array);
        static void RX_Int();
     private:
     uint8_t selUart;
     pinconn pc;
     OledTerminal oled_terminal;
     static QueueHandle_t Global_Queue_Handle;

};