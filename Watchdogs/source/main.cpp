#include <cstdint> 
#include <project_config.hpp>
#include <iterator>
#include "utility/log.hpp"
#include "utility/time.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"
#include "event_groups.h"
#inlcude "semphr.h"
#include "L2_HAL/sensors/temperature/temperature.hpp"
//#include "L3_Application/oled_terminal.hpp"

Temperature Producer_Temperature;
xQueueHandle Global_Queue_Handle = 0;

EventGroupHandle_t PC_watchdog;
const uint32_t producer_task_id = (1<<0);
const uint32_t consumer_task_id = (1<<1);
const uint32_t tasks_all_bits = (producer_task_id | consumer_task_id);

void vProducerTask(){

    int data[100]=0;
    uint32_t  total = 0;
    int data_index = 0;
    uint32_t avgTemp = 0;
    int sensor_queue[10]=0;
    int sensor_index=0;
    while(1)
    { 
        data[index] = Producer_Temperature.GetCelsius();
        total = total + data[index];
        index++;
        vTaskDelay(1);
        if(index == 100)
        {
        avgTemp = total / 100;
        sensor_queue[sensor_index] = avgTemp;
        sensor_index++;
        xEventGroupSetBits(PC_watchdog, producer_task_id);
        if(!xQueueSend(Global_Queue_Handle,&avgtemp, 1000)
        {
            printf("Failed to send Sensor Queue");
        }
        index == 0;
        }
    }
}

void vConsumerTask(){
    
    int SensorQueue[9] = 0;
    uint16_t data = 0;
    uint8_t index = 0;
    while(1)
    {
        if(xQueueReceive(Global_Queue_Handle,&data, portMAX_DELAY))
        {
           SensorQueue[index] = data;
           index++;
        }
        if(index == 9)
        {
            //send data to SD card
            xEventGroupSetBits(PC_watchdog, consumer_task_id);
        }
    } 
}

void vWatchdogs(){

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
                //output to SD "producer stuck"
            }
            if(!(result & consumer_task_id)){
                //output to SD "consumer stuck"
            }
        }
    }
}

int main(){
    Producer_Temperature.Initialize();
    PC_watchdog = xEventGroupCreate();
    vSemaphoreCreateBinary(consumer_signal);
}