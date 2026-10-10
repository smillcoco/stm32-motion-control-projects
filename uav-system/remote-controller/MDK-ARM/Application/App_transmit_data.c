#include "App_transmit_data.h"

extern Remote_Data remote_data;

uint8_t TX_data[TX_PLOAD_WIDTH] = {0};
uint8_t post_buff[TX_PLOAD_WIDTH] = {0};

//自动切换SI24R1的模式 =>将采集完成的遥控数据打包发送到飞机
void App_transmit_data(void)
{


    // 2. 发送数据 => 唯一性和可靠性
    // (1) => 帧头校验指定发送给对应的设备 唯一性
    // (2) => 发送数据的结尾 => 添加检验和(将发送的数据累加得到的值添加到数据的末尾) 可靠性

    uint32_t sum = 0;
    //1 字节 (Byte) = 8 个二进制位 (bit)
    // 帧头校验设置为3字节 => 数据本身是10字节 => 校验和设置为4字节
    // 帧头校验 3字节 
    TX_data[0] = FRAME_HEAD_CHECK_1;
    TX_data[1] = FRAME_HEAD_CHECK_2;
    TX_data[2] = FRAME_HEAD_CHECK_3;

    // 高8位在前
    // thr
    TX_data[3]  = (remote_data.thr >> 8) & 0xFF;
    TX_data[4]  = remote_data.thr & 0xFF;
    // yaw
    TX_data[5]  = (remote_data.yaw >> 8) & 0xFF;
    TX_data[6]  = remote_data.yaw & 0xFF;
    // pit
    TX_data[7]  = (remote_data.pit >> 8) & 0xFF;
    TX_data[8]  = remote_data.pit & 0xFF;
    // rol
    TX_data[9]  = (remote_data.rol >> 8) & 0xFF;
    TX_data[10] = remote_data.rol & 0xFF;

    //开启临界区
    taskENTER_CRITICAL();

    // shutdown/fix_height 仅0/1，单字节直接赋值
    TX_data[11] = remote_data.shutdown;
    remote_data.shutdown = 0;
    TX_data[12] = remote_data.fix_height;
    remote_data.fix_height = 0;

    //退出临界区
    taskEXIT_CRITICAL();

    for(uint8_t i = 0; i < 13; i++)
    {
        sum += TX_data[i];

    }
    
    // 校验和 4字节(高位在前)
    TX_data[13] = (sum >> 24) & 0xFF;
    TX_data[14] = (sum >> 16) & 0xFF;
    TX_data[15] = (sum >> 8) & 0xFF;
    TX_data[16] = sum & 0xFF;

    printf("%d,%d,%d,%d,%d,%d\n",remote_data.thr, remote_data.yaw, remote_data.pit, 
    remote_data.rol, remote_data.shutdown, remote_data.fix_height);
    // 1. 切换到发送模式
    Int_SI24R1_TX_Mode();
    uint8_t res = Int_SI24R1_TxPacket(TX_data); 
    // 3. 切换回接收模式
    Int_SI24R1_RX_Mode();

    
    if(res == 0)
    {
        
        //2.4G同步发送逻辑 =>必须严格按照双端发送的时序才能数据
        while(Int_SI24R1_RxPacket(post_buff) == 1)
        {

        }

        //printf("post_buff:%s\n", post_buff);

    }
      

        






}

