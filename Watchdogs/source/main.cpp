#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"
#include "event_groups.h"

#include <cstdint>
#include <cstdio>

#include "config.hpp"
#include "utility/log.hpp"
#include "utility/macros.hpp"
#include "utility/rtos.hpp"
#include "utility/time.hpp"

#include "L3_Application/commandline.hpp"
#include "L2_HAL/sensors/temperature/temperature.hpp"
#include "utility/rtos.hpp"
#include "task_command.hpp"
#include "L3_Application/commands/lpc_system_command.hpp"
//#include "L3_Application/oled_terminal.hpp"



Temperature Producer_Temperature;
xQueueHandle Global_Queue_Handle = 0;

EventGroupHandle_t PC_watchdog;
const uint32_t producer_task_id = (1<<0);
const uint32_t consumer_task_id = (1<<1);
const uint32_t tasks_all_bits = (producer_task_id | consumer_task_id);

void vProducerTask(void * pvParameters){

    int temperature = 0;;
    int  total = 0;
    int data_index = 0;
    int avgTemp = 0;
    while(1)
    { 
        temperature = Producer_Temperature.GetCelsius();
        total = total + temperature;
        data_index++;
        vTaskDelay(1);
        if(data_index == 100)
        {
        avgTemp = total / 100;
        xEventGroupSetBits(PC_watchdog, producer_task_id);
        if(!xQueueSend(Global_Queue_Handle,&avgTemp, 1000))
        {
            printf("Failed to send Sensor Queue");
        }
        total = 0;
        data_index = 0;
        }
    }
}

void vConsumerTask(void * pvParameters){
    
    int SensorQueue = 0;
    int data = 0;
    uint8_t index = 0;
    int cTotal = 0;
    int cAvgTemp = 0;
    while(1)
    {
        if(xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY))
        {
           SensorQueue = data;
           cTotal = cTotal + SensorQueue;
           index++;
           
        }
        if(index == 9)
        {
            cAvgTemp = cTotal / 9;
            index = 0;
            //send data to SD card
            xEventGroupSetBits(PC_watchdog, consumer_task_id);
        }
    } 
}

void vWatchdogs(void * pvParameters){

    while(1){
        uint32_t result = xEventGroupWaitBits(PC_watchdog, tasks_all_bits,
                                                pdTRUE, //Clear bits on exit   
                                                pdTRUE, // wait for all bits to set
                                                1000    //wait 1 sec to check the bits
                                                );
        if((result & tasks_all_bits) == tasks_all_bits){
            printf("System is fine\n");
        }
        else{
            if(!(result & producer_task_id)){
                printf("Producer not set \n");
                //output to SD "producer stuck"
            }
            if(!(result & consumer_task_id)){
               printf("Consumer not set\n");
                //output to SD "consumer stuck"
            }
        }
    }
}

int main(void){
    LOG_INFO("Start \n");
    Global_Queue_Handle = xQueueCreate(10,sizeof(10));
    Producer_Temperature.Initialize();
    PC_watchdog = xEventGroupCreate();
    xTaskCreate(vProducerTask,(const char*) "Producer_Task",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    xTaskCreate(vConsumerTask,"Consumer_Task",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    xTaskCreate(vWatchdogs,"Watchdogs",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kHigh, rtos::kNoHandle);
    vTaskStartScheduler();
}