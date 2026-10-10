#include "App_freeRTOS_Task.h"

//STM32F103C8T6 => SRAM 20k  => 分配12K给操作系统

// 内存管理 => C 语言中的结构体通常保存在堆中 不会自动垃圾回收 => 始终使用同一个结构体 不断循环使用



//led结构体
LED_Struct left_top_led = {.port = LED1_GPIO_Port, .pin = LED1_Pin};
LED_Struct left_bottom_led = {.port = LED4_GPIO_Port, .pin = LED4_Pin};
LED_Struct right_top_led = {.port = LED2_GPIO_Port, .pin = LED2_Pin};
LED_Struct right_bottom_led = {.port = LED3_GPIO_Port, .pin = LED3_Pin};


//表示当前连接状态(REMOTE_CONNECTED:已连接,REMOTE_DISCONNECTED:未连接)
Remote_State remote_state = REMOTE_DISCONNECTED;

//表示当前的飞行状态(IDLE:空闲,NORMAL:正常,FIX_HEIGHT:定高,FAIL:故障)
Flight_State flight_state = IDLE;

//扩展获取接受的遥控数据
Remote_Data remote_data = {.thr = 0, .yaw = 500, .pit = 500, .rol = 500, 
.fix_height = 0, .shutdown = 0};

//按下定高飞行的高度
uint16_t fix_height = 0;

//电池电压
uint8_t back_buff[TX_PLOAD_WIDTH] = {0};

//电源管理任务
void power_task(void *args);
//在 32 位 MCU（比如 STM32）上，1 个字 = 4 字节（32 位）
// 最小推荐填写128 => 128*4 = 512B
#define POWER_TASK_STACK_SIZE 128
// 任务优先级 => 数值越小 优先级越小  => 最大4  => 不推荐使用最小优先级0
#define POWER_TASK_PRIORITY 4
TaskHandle_t power_task_handle;
//定义任务的周期
#define POWER_TASK_PERIOD 10000

//飞行控制任务
void flight_task(void *args);
#define FLIGHT_TASK_STACK_SIZE 128
#define FLIGHT_TASK_PRIORITY 3
TaskHandle_t flight_task_handle;
#define FLIGHT_TASK_PERIOD 6

//led控制任务
void led_task(void *args);
#define LED_TASK_STACK_SIZE 128
#define LED_TASK_PRIORITY 1
TaskHandle_t led_task_handle;
#define LED_TASK_PERIOD 100


//发送通讯任务
void TX_task(void *args);
#define TX_TASK_STACK_SIZE 128
#define TX_TASK_PRIORITY 2
TaskHandle_t TX_task_handle;
#define TX_TASK_PERIOD 6

//接收通讯任务
void RX_task(void *args);
#define RX_TASK_STACK_SIZE 128
#define RX_TASK_PRIORITY 4
TaskHandle_t RX_task_handle;
#define RX_TASK_PERIOD 10


/**
 * @brief 启动freeRTOS操作系统
 *
 */
void App_freeRTOS_start(void)
{
    // 0. printf互斥锁初始化（必须在创建任务之前）
    debug_printf_mutex_init();

    //1.创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
    
    //2.创建飞行控制任务
    xTaskCreate(flight_task, "flight_task", FLIGHT_TASK_STACK_SIZE, NULL, FLIGHT_TASK_PRIORITY, &flight_task_handle);

    //3.创建led控制任务
    xTaskCreate(led_task, "led_task", LED_TASK_STACK_SIZE, NULL, LED_TASK_PRIORITY, &led_task_handle);

    //4.创建接受通讯任务
    xTaskCreate(RX_task, "RX_task", RX_TASK_STACK_SIZE, NULL, RX_TASK_PRIORITY, &RX_task_handle);

    //5. 启动调度器
    vTaskStartScheduler();
}

void power_task(void *args)
{   
    //获取当前的基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {


        //每十秒启动一次 => 启动电源 避免自动关机
        //vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(POWER_TASK_PERIOD));

        //启动电源
        //INT_IP5305T_start();


        // 使用直接任务通知的接收方法实现10s处理一次
        // 一直等任务通知  直到收到通知res=1  或者  超时res=0
        uint32_t res = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(POWER_TASK_PERIOD));
        if (res != 0)
        {
            // 收到关机通知
            INT_IP5305T_shutdown();
        }
        else
        {
            // 不需要关机 => 正常执行启动
            INT_IP5305T_start();
        }

    }

}

void flight_task(void *args)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t count = 0;
    //一定要先对MPU6050执行初始化操作 =>之后才能读取数据(包含零偏校准)
    App_flight_init();

    while (1)
    {
        //1.获取三轴加速度和三轴角速度
        App_flight_get_euler_angle();

        //2.根据当前的欧拉角,进行pid控制
        App_flight_pid_process();

        //3.判断定高
        if(flight_state == FIX_HEIGHT)
        {
            //才需要计算PID=>激光测距仪的数据采集 20ms一次
            count++;
            if (count>= 4)
            {

                App_flight_fix_height_pid_process();
                count = 0;

            }

        }

        //4.根据pid计算的结果,进行电机的控制
        App_flight_control_process();

        //5.打印激光测距仪得到的距离值
        //uint16_t distance = Int_VL53L1X_GetDistance();
        //printf("Distance:%dmm\n", distance);



        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(FLIGHT_TASK_PERIOD));



    }

}

void led_task(void *args)
{

    //获取当前的基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    //计数器 用于控制led的闪烁频率
    uint8_t count = 0;
    while (1)
    {   
        count++;
        //前两个灯表示连接状态
        //1.判断当前的连接状态
        if(remote_state == REMOTE_CONNECTED)
        {
            //2.点亮前两个灯(已连接)
            Int_led_turn_on(&left_top_led); 
            Int_led_turn_on(&right_top_led);
            
        }
        else if(remote_state == REMOTE_DISCONNECTED)
        {
            //3.熄灭前两个灯(未连接)
            Int_led_turn_off(&left_top_led);
            Int_led_turn_off(&right_top_led);
        }

        //后两个灯表示飞行状态
        //1.判断当前的飞行状态
        if(flight_state == IDLE)
        {
            //2.两个灯慢闪烁(空闲) => 500ms闪烁一次
            if(count % 5 == 0)
            {
                //循环五次 一次100ms 五次就是500ms
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);

            }
            

        }
        else if(flight_state == NORMAL)
        {
            //3.两个灯快闪烁(正常) => 200ms闪烁一次
            if(count % 2 == 0)
            {
                //循环两次 一次100ms 两次就是200ms
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if(flight_state == FIX_HEIGHT)
        {
            //4.点亮后两个灯(定高)
            Int_led_turn_on(&left_bottom_led);
            Int_led_turn_on(&right_bottom_led);
        }
        else if(flight_state == FAIL)
        {
            //5.熄灭后两个灯(故障)
            Int_led_turn_off(&left_bottom_led);
            Int_led_turn_off(&right_bottom_led);
        }

        //6.计数器重置(10是2和5的最小公倍数)
        if(count == 10)
        {
            count = 0;
        }
        


        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LED_TASK_PERIOD));
    }
}





void RX_task(void *args)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    //初始化电池电压采样
    Int_bat_ADC_Init();

    while (1)
    {
        //1.接收数据
        uint8_t res = App_receive_data();
        //2.根据接受数据的返回值 处理当前飞机的连接状态
        App_process_connect_state(res); 
        //3.处理关机命令
        if(remote_data.shutdown == 1)
        {
            //使用INT_IP5305T_shutdown 关机功能可以实现,但是项目结构比较奇怪
            //INT_IP5305T_shutdown();

            //使用freertos直接任务通知 => 通知电源任务 => 执行关机
            xTaskNotifyGive(power_task_handle);


        }

        //4.处理飞机的飞行状态  遇到故障状态(失联)=>会一直等待任务通知
        App_process_flight_state();

        //5.准备回传电池电压值
        float voltage = Int_bat_ADC_Read();
        sprintf(back_buff, "%.2f", voltage);
        //printf("Voltage: %.2f\n", voltage);

        vTaskDelay(pdMS_TO_TICKS(TX_TASK_PERIOD));
    }
    
}

