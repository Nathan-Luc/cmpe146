#include "third_party/FreeRTOS/Source/include/FreeRTOS.h"
#include "third_party/FreeRTOS/Source/include/task.h"
#include "third_party/FreeRTOS/Source/include/queue.h"
#include "third_party/FreeRTOS/Source/include/event_groups.h"
#include "third_party/FreeRTOS/Source/include/semphr.h"

#include "third_party/fatfs/source/ff.h"
#include "utility/log.hpp"

#include "L2_HAL/sensors/temperature/temperature.hpp"

#include <cstdint>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include "utility/time.hpp"

#include "L3_Application/commandline.hpp"

#include "utility/rtos.hpp"
//#include "task_command.hpp"

SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();

FATFS fat_fs;
 
Temperature Producer_Temperature;
xQueueHandle Global_Queue_Handle = 0;

EventGroupHandle_t PC_watchdog;
const uint32_t producer_task_id = (1<<0);
const uint32_t consumer_task_id = (1<<1);
const uint32_t tasks_all_bits = (producer_task_id | consumer_task_id);
  
void vProducerTask(void * pvParameters){

    uint16_t temperature = 0;;
    uint32_t  total = 0;
    uint8_t data_index = 0;
    uint16_t avgTemp = 0;
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
        xQueueSend(Global_Queue_Handle,&avgTemp, 100);
        total = 0;
        data_index = 0;
        }
    }
}

void vConsumerTask(void * pvParameters){
    
   uint16_t data = 0;
    FIL fil;
    f_open(&fil, "TEMP.txt", FA_READ | FA_OPEN_APPEND | FA_WRITE);
    while(1)
    {   if(xSemaphoreTake(xSemaphore,portMAX_DELAY))
        {
            printf("S open\n");
            
        if(xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY))
        {   
            f_puts("Temp : ", &fil);
            f_printf(&fil, "%d\r\n", data );
            f_sync(&fil);
        }
            xEventGroupSetBits(PC_watchdog, consumer_task_id);
            xSemaphoreGive(xSemaphore);
        } 
           
    }
        
     
}

void vWatchdogs(void * pvParameters){
    vTaskDelay(1000);
    FIL fil;

    while(1)
    {
       vTaskDelay(1500);
        uint32_t result = xEventGroupWaitBits(PC_watchdog, tasks_all_bits,
                                                pdTRUE, //Clear bits on exit   
                                                pdTRUE, // wait for all bits to set
                                                1000    //wait 1 sec to check the bits
                                                );
        if((result & tasks_all_bits) == tasks_all_bits)
        {
            printf("System is fine\n");
           // f_sync(&fil);
        }
        else
        {
            if(!(result & producer_task_id)){
                printf("Producer not set \n");
                if(xSemaphoreTake(xSemaphore,portMAX_DELAY))
                {
                    f_open(&fil, "STUCK.txt", FA_READ | FA_OPEN_APPEND | FA_WRITE);
                    f_puts("Producer is stuck\r\n", &fil);
                    f_sync(&fil);
                    xSemaphoreGive(xSemaphore);    
                }
                
                //output to SD "producer stuck"
            }
            if(!(result & consumer_task_id))
            {
                printf("Consumer not set\n");
                //output to SD "consumer stuck"
                if(xSemaphoreTake(xSemaphore,portMAX_DELAY))
                {
                    f_open(&fil, "STUCK.txt", FA_READ | FA_OPEN_APPEND | FA_WRITE);
                    f_puts("Consumer is stuck\r\n", &fil);
                    f_sync(&fil);
                    xSemaphoreGive(xSemaphore);
                }
            }
        }
    }
}
int main(void){
   // LOG_INFO("Start \n");
   // LOG_INFO("Mounting filesystem...");
    f_mount(&fat_fs, "", 0);
    Global_Queue_Handle = xQueueCreate(10,sizeof(uint16_t));
    Producer_Temperature.Initialize();
    PC_watchdog = xEventGroupCreate();
    xTaskCreate(vProducerTask,"Producer_Task",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    xTaskCreate(vConsumerTask,"Consumer_Task",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    xTaskCreate(vWatchdogs,"Watchdogs",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kHigh, rtos::kNoHandle);
    vTaskStartScheduler();
}