#include "App_receive_data.h"

extern Remote_Data remote_data;

//重试次数
uint8_t retry_count = 0;
uint8_t RX_data[TX_PLOAD_WIDTH] = {0};
extern Remote_State remote_state; 
extern Flight_State flight_state;
extern uint16_t fix_height; 
extern uint8_t back_buff[TX_PLOAD_WIDTH];

//油门解锁状态值
Thr_state thr_state = FREE;
//max状态的进入时间
uint32_t max_enter_time = 0;
//min状态的进入时间
uint32_t min_enter_time = 0;

//接受遥控器发送的数据,解析为结构体
//0:校验通过,是正常的数据     1:没收到数据,或者校验失败
uint8_t App_receive_data(void)
{
    memset(RX_data,0,TX_PLOAD_WIDTH);


    //回传也要严格按照时序=>从接收成功遥控数据开始发送一直到发送完成
    uint8_t res = Int_SI24R1_RxPacket(RX_data);
    if(res == 0)
    {
        //2.4G同步发送逻辑 =>必须严格按照双端发送的时序才能数据
        Int_SI24R1_TX_Mode(); // 切换回发射模式，准备发送数据

        uint16_t count = 500;
        while(Int_SI24R1_TxPacket(back_buff) == 1 && count--)
        {
        }
        Int_SI24R1_RX_Mode(); // 切换回接收模式，准备接收下一次数据
    }

    // 判断：把缓冲区强转为char字符串，用strlen判断长度是否等于0
    if (strlen((char *)RX_data) == 0)
    {
        // 如果缓冲区是空字符串，直接return 1退出当前函数
        return 1;
    }
    //1.帧头校验
    if(RX_data[0] != FRAME_HEAD_CHECK_1 || RX_data[1] != FRAME_HEAD_CHECK_2 || RX_data[2] != FRAME_HEAD_CHECK_3)
    {
        return 1;
    }
    //2.帧尾校验
    uint32_t sum = 0;
    uint32_t sum_receive = 0;

    for(uint8_t i = 0; i < 13; i++)
    {
        sum += RX_data[i];
    }
    //高位在前
    sum_receive = (RX_data[13] << 24) | (RX_data[14] << 16) | (RX_data[15] << 8) | RX_data[16];

    if(sum != sum_receive)
    {
        return 1;
    }

    //3.保存数据
    remote_data.thr = (RX_data[3] << 8) | RX_data[4];
    remote_data.yaw = (RX_data[5] << 8) | RX_data[6];
    remote_data.pit = (RX_data[7] << 8) | RX_data[8];
    remote_data.rol = (RX_data[9] << 8) | RX_data[10];
    remote_data.shutdown = RX_data[11];
    remote_data.fix_height = RX_data[12];

    //printf("%d,%d,%d,%d,%d,%d\n",remote_data.thr,remote_data.yaw,remote_data.pit,remote_data.rol,remote_data.shutdown,remote_data.fix_height);

    return 0;



}


void App_process_connect_state(uint8_t res)
{   
    if(res == 0)
    {
        //接收数据成功一次 即为连接成功
        //此处使用的全局变量 只有当前一个地方会修改 LED灯控任务当中是读取使用
        remote_state = REMOTE_CONNECTED;
        retry_count = 0;
    }
    else if(res == 1)
    {
     
        retry_count++;
        if(retry_count >= MAX_RETRY_COUNT)
        {
            
            remote_state = REMOTE_DISCONNECTED;
            retry_count = 0;

        }
    }



}

//处理解锁逻辑
//0:解锁成功 1:解锁失败
static uint8_t App_process_unlock(void)
{
    switch(thr_state)
    {
    case FREE:
        if(remote_data.thr >= 900)
        {
            thr_state = MAX;
            //freertos中以mc为单位计数的时间
            max_enter_time = xTaskGetTickCount();
        }
        break;
    case MAX:
        if(remote_data.thr < 900)
        {
            if(xTaskGetTickCount() - max_enter_time >= 1000)
            {
                //油门保持最高状态超过1s=>进入leave_max状态
                thr_state = LEAVE_MAX;
            }
            else
            {
                //油门保持最高状态时间小于1s=>退回到free 重新解锁
                thr_state = FREE;
            }
        }
        break;
    case LEAVE_MAX:
        if(remote_data.thr <= 100)
        {
            //油门回到0 进入min状态
            thr_state = MIN;
            min_enter_time = xTaskGetTickCount();
        }
        break;
    case MIN:
        //判断保持多久时间
        if(xTaskGetTickCount() - min_enter_time <= 1000)
        {
            //还不够一秒
            if(remote_data.thr > 100)
            {
                thr_state = FREE;
            }
        }
        else 
        {
            //已经超过一秒
            thr_state = UNLOCK;
        }
        break;
    case UNLOCK:

        break;
    default:
        break;
    }

    if(thr_state == UNLOCK)
    {
        // 解锁状态处理
        return 0;
    }

    return 1;
}

void App_process_flight_state(void)
{
    //使用状态机逻辑实现
    //1.轮询调用判断当前所处的状态
    switch (flight_state)
    {
    //2.只需要编写指向其他状态的代码即可
    case IDLE:
        //解锁成功,空闲状态才能进行解锁
        if(App_process_unlock() == 0)
        {
            flight_state = NORMAL;
            //每一次解锁成功,需要将解锁状态重置
            thr_state = FREE; 
        }
        
        break;
    case NORMAL:
        //判断进入定高模式
        if(remote_data.fix_height == 1)
        {
            flight_state = FIX_HEIGHT;
            remote_data.fix_height = 0;

            // 记录当前目标的高度
            fix_height = Int_VL53L1X_GetDistance(); 
        }
        //判断进入故障模式
        if(remote_state == REMOTE_DISCONNECTED)
        {
            flight_state = FAIL;

        } 
        break;
    case FIX_HEIGHT:
        //取消定高
        if(remote_data.fix_height == 1)
        {
            flight_state = NORMAL;
            remote_data.fix_height = 0;
        }
        //判断进入故障模式
        if(remote_state == REMOTE_DISCONNECTED)
        {
            flight_state = FAIL;
        }
        break;
    case FAIL:
        //处理失联故障  缓慢停止电机
        //等待故障处理完成=>T直等处理完成不会
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        flight_state = IDLE;
        break;
    default:
        break;
    }

}
