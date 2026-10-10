#include "App_freeRTOS_Task.h"

// STM32F103C8T6 => SRAM 20k  => 分配12K给操作系统




// 电源管理任务
void power_task(void *args);
// 最小推荐填写128 => 128*4 = 512B
#define POWER_TASK_STACK_SIZE 128
// 任务优先级 => 数值越小 优先级越小  => 最大4  => 不推荐使用最小优先级0
#define POWER_TASK_PRIORITY 4
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000

//发送通讯任务
void TX_task(void *args);
#define TX_TASK_STACK_SIZE 128
#define TX_TASK_PRIORITY 3
TaskHandle_t TX_task_handle;
#define TX_TASK_PERIOD 10 

//按键任务
void key_task(void *args);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
TaskHandle_t key_task_handle;
#define KEY_TASK_PERIOD 20

//摇杆任务
void joy_task(void *args);
#define JOY_TASK_STACK_SIZE 128
#define JOY_TASK_PRIORITY 2
TaskHandle_t joy_task_handle;
#define JOY_TASK_PERIOD 20

//屏幕任务
void oled_task(void *args);
#define OLED_TASK_STACK_SIZE 128
#define OLED_TASK_PRIORITY 1
TaskHandle_t oled_task_handle;
#define OLED_TASK_PERIOD 100


/**

 * @brief 启动freeRTOS操作系统

 *

 */

void App_freeRTOS_start(void)

{
    //0. printf互斥锁初始化（必须在创建任务之前）
    debug_printf_mutex_init();

    //1.创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
    //2.创建发送通讯任务
    xTaskCreate(TX_task, "TX_task", TX_TASK_STACK_SIZE, NULL, TX_TASK_PRIORITY, &TX_task_handle);
    //3.创建按键任务
    xTaskCreate(key_task, "key_task", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &key_task_handle);
    //4.创建摇杆任务
    xTaskCreate(joy_task, "joy_task", JOY_TASK_STACK_SIZE, NULL, JOY_TASK_PRIORITY, &joy_task_handle);
    //5.创建屏幕任务
    xTaskCreate(oled_task, "oled_task", OLED_TASK_STACK_SIZE, NULL, OLED_TASK_PRIORITY, &oled_task_handle);  

    //启动调度器
    vTaskStartScheduler();
}



void power_task(void *args)

{

    // 获取当前的基准时间

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)

    {



        // 每10s执行一次  =>  启动电源  避免自动关机

        vTaskDelayUntil(&xLastWakeTime, POWER_TASK_PERIOD);



        // 启动电源

        Int_IP5305T_start();

    }

}

void joy_task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    //1.初始化摇杆adc
    Int_joystick_init();

    while (1)
    {  
        //统一的处理方式
        App_process_joystick_data();

        vTaskDelayUntil(&xLastWakeTime, JOY_TASK_PERIOD);


    }
    
}


void key_task(void *args)
{
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        //统一的处理方式       
        App_process_key_data();

        vTaskDelayUntil(&xLastWakeTime, KEY_TASK_PERIOD);

    }

}



void TX_task(void *args)

{



    while (1)
    {

        App_transmit_data();

        vTaskDelay(pdMS_TO_TICKS(TX_TASK_PERIOD));

    }

    

}

void oled_task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    App_display_init();
    while (1)
    {
        //统一的处理方式
        App_display_show();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(OLED_TASK_PERIOD));

    }
    
}
