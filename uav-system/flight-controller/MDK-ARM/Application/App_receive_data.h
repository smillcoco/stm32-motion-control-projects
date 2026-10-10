#ifndef APP_RECEIVE_DATA_H
#define APP_RECEIVE_DATA_H

#include <string.h>
#include "Int_SI24R1.h"
#include "Com_config.h"
#include "Int_VL53L1X.h"

//定义帧头校验值
#define FRAME_HEAD_CHECK_1 'a'
#define FRAME_HEAD_CHECK_2 'b'
#define FRAME_HEAD_CHECK_3 'c'

//最大重试次数
#define MAX_RETRY_COUNT 10

//接受遥控器发送的数据,解析为结构体
//0:校验通过,是正常的数据     1:没收到数据,或者校验失败
uint8_t App_receive_data(void);

//处理连接状态
void App_process_connect_state(uint8_t res);

//处理飞机飞行状态
void App_process_flight_state(void);

//处理解锁逻辑
//0:解锁成功 1:解锁失败
static uint8_t App_process_unlock(void);

#endif // APP_RECEIVE_DATA_H    
