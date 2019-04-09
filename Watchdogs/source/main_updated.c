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
#include "utility/rtos.hpp"

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
#include <FreeRTOS.h>
#include <task.h>
#include "L3_Application/commands/common.hpp"

//#include "task_command.hpp"

SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();

FATFS fat_fs;
xQueueHandle Global_Queue_Handle = 0;

EventGroupHandle_t PC_watchdog;
const uint32_t producer_task_id = (1<<0);
const uint32_t consumer_task_id = (1<<1);
const uint32_t tasks_all_bits = (producer_task_id | consumer_task_id);

uint64_t time = 0;
int counter=0;

void vProducerTask(void * pvParameters){

    uint16_t temperature = 0;;
    uint32_t  total = 0;
    uint8_t data_index = 0;
    uint16_t avgTemp = 0;
    Temperature Producer_Temperature;
    Producer_Temperature.Initialize();
    while(1)
    { 
        temperature = static_cast<uint16_t>( Producer_Temperature.GetCelsius());
        total = total + temperature;
        data_index++;
        vTaskDelay(1);
        if(data_index == 100)
        {
        avgTemp = total / 100;
        xQueueSend(Global_Queue_Handle,&avgTemp,100);
        total = 0;
        data_index = 0;
        counter++;
        xEventGroupSetBits(PC_watchdog, producer_task_id);
        }
        
    }
}

void vConsumerTask(void * pvParameters){
    
   uint16_t data = 0;
    FIL fil;
    f_open(&fil, "SENSOR.txt", FA_OPEN_APPEND | FA_WRITE);
    //f_sync(&fil);
    
   // f_sync(&fil);
    while(1)
    {  
        
           
        if(xQueueReceive(Global_Queue_Handle,&data, 100))
        {    time = Milliseconds(); 
                printf("Time: " "%"PRIu64 " Temp: %d\n", time, (unsigned int) data);
            if(counter %7 ==0){
                printf("Writing to SD\n");
            if(xSemaphoreTake(xSemaphore,1000)){

            //sprintf(buffer, "Temp: %d Time: %d\r\n", data, time);
           // strcat(buffer2,buffer);
           //f_puts("TEMP: ",&fil);
      
          // f_puts("TIME: ",&fil);
           f_printf(&fil, "%d", time);
            f_printf(&fil, " %d\r\n", data);
            //f_printf(&fil, "%s", buffer2);
            f_sync(&fil);
            //f_close(&fil);
           }
             xSemaphoreGive(xSemaphore);
            }
         xEventGroupSetBits(PC_watchdog, consumer_task_id);
       
        }
         vTaskDelay(1);
    } 
           
}
        
     


void vWatchdogs(void * pvParameters){
    FIL fil;
    f_open(&fil, "STUCK.txt", FA_OPEN_APPEND | FA_WRITE);
    vTaskDelay(2000);
    while(1)
    {  
        vTaskDelay(1000);
        uint32_t result = xEventGroupWaitBits(PC_watchdog, tasks_all_bits,
                                                pdTRUE, //Clear bits on exit   
                                                pdTRUE, // wait for all bits to set
                                                1000    //wait 1 sec to check the bits
                                                );
                        
        if((result & tasks_all_bits) == tasks_all_bits)
        {
            printf("System is fine\n");
           // LOG_INFO("System is fine\n");
            //f_sync(&fil1);
        }
        else
        {
            if(!(result & producer_task_id)){
                printf("Producer not set \n");
                
                if(xSemaphoreTake(xSemaphore,1000))
                {
                    //LOG_INFO("in here");
                   
                    f_puts("Producer is stuck\r\n", &fil);
                    f_sync(&fil);
                    xSemaphoreGive(xSemaphore);    
                    //f_close(&fil1);
                }
                
                //output to SD "producer stuck"
            }
            if(!(result & consumer_task_id))
            {
                printf("Consumer not set\n");
                //output to SD "consumer stuck"
                if(xSemaphoreTake(xSemaphore,1000))
                {
                   // f_open(&fil1, "STUCK.txt", FA_READ | FA_OPEN_APPEND | FA_WRITE);
                   // f_sync(&fil1);
                    f_puts("Consumer is stuck\r\n", &fil);
                    f_sync(&fil);
                    xSemaphoreGive(xSemaphore);
                   // f_close(&fil1);
                }
            }
        }
       }
    
}

CommandList_t<32> command_list;
CommandLine<command_list> ci;

void vTerminalTask([[maybe_unused]] void * ptr)
{
  LOG_INFO("Press Enter to Start Command Line!");

  ci.WaitForInput();

  LOG_WARNING("\nUser has quit from terminal!");
  vTaskDelete(nullptr);
}

TaskHandle_t xProducerTask;
TaskHandle_t xConsumerTask;
TaskHandle_t xWatchdogTask;
TaskHandle_t xTerminalTask;

// TaskHandle_t xProducer;
// TaskHandle_t xConsumer;
// TaskHandle_t xWatchdog;
// TaskHandle_t xTerminal;



inline Command task("task", "Suspends and resumes tasks", [](int argc, const char * const argv[]) -> int {

    if      (argv[1] == "suspend-producer-task"){   vTaskSuspend(xProducerTask);    }
    else if (argv[1] == "resume-producer-task"){    vTaskResume(xProducerTask);     }

    else if (argv[1] == "suspend-consumer-task"){   vTaskSuspend(xConsumerTask);    }
    else if (argv[1] == "resume-consumer-task"){    vTaskResume(xConsumerTask);     }

    else if (argv[1] == "suspend-watchdog-task"){   vTaskSuspend(xWatchdogTask);    }
    else if (argv[1] == "resume-watchdog-task"){    vTaskResume(xWatchdogTask);     }

    else if (argv[1] == "suspend-terminal-task"){   vTaskSuspend(xTerminalTask);    }
    else if (argv[1] == "resume-terminal-task"){    vTaskResume(xTerminalTask);     }

    return 0;
});



int main(void){
   // LOG_INFO("Start \n");
   // LOG_INFO("Mounting filesystem...");
    Global_Queue_Handle = xQueueCreate(10,sizeof(uint16_t));
    PC_watchdog = xEventGroupCreate();
    

    AddCommonCommands(ci);
    ci.AddCommand(&task);  //


    xTaskCreate(vProducerTask, "Producer_Task", 1024, NULL, 3, &xProducerTask);
    xTaskCreate(vConsumerTask, "Consumer_Task", 1024, NULL, 3, &xConsumerTask);
    xTaskCreate(vWatchdogs, "Watchdog_Task", 1024, NULL, 3, &xWatchdogTask);
    xTaskCreate(vTerminalTask, "Terminal_Task", 1024, NULL, 3, &xTerminalTask);

    f_mount(&fat_fs, "", 0);
    Global_Queue_Handle = xQueueCreate(10,sizeof(uint16_t));
    Producer_Temperature.Initialize();
    PC_watchdog = xEventGroupCreate();
    //xTaskCreate(vProducerTask,"Producer_Task",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    //xTaskCreate(vConsumerTask,"Consumer_Task",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    //xTaskCreate(vWatchdogs,"Watchdogs",rtos::StackSize(4096), rtos::kNoParameter,  rtos::Priority::kHigh, rtos::kNoHandle);

    LOG_INFO("Initializing CommandLine object...\n");
    LOG_INFO("Part 2\n");
    ci.Initialize();
    /*
    xTaskCreate(vProducerTask,"Producer_Task",rtos::StackSize(1024), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    xTaskCreate(vConsumerTask,"Consumer_Task",rtos::StackSize(1024), rtos::kNoParameter,  rtos::Priority::kMedium, rtos::kNoHandle);
    xTaskCreate(vWatchdogs,"Watchdogs",rtos::StackSize(1024), rtos::kNoParameter,  rtos::Priority::kHigh, rtos::kNoHandle);
    */
    //xTaskCreate(vProducerTask,"Producer_Task",1024, NULL, 2, NULL);
    //xTaskCreate(vConsumerTask,"Consumer_Task",1024, NULL,  3, NULL);
    //xTaskCreate(vWatchdogs,"Watchdogs",1024, NULL, 4, NULL);
    
    vTaskStartScheduler();
}