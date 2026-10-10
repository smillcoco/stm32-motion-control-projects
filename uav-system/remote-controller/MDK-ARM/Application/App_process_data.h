#ifndef APP_PROCESS_DATA_H
#define APP_PROCESS_DATA_H

#include "Int_key.h"
#include "Int_joystick.h"
#include "Com_tool.h"

//遥控数据结构体
typedef struct 
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;
    uint8_t shutdown;     //0: 开机 1: 关机
    uint8_t fix_height;   //0: 不定高 1: 定高


} Remote_Data;


//校准摇杆函数
void App_calibrate_joystick(void);

//处理按键数据 => 如果有按键按下 记录对应的数据
void App_process_key_data(void);

//处理摇杆数据 => 修正极性相位和标准值
void App_process_joystick_data(void);


#endif // APP_PROCESS_DATA_H    

