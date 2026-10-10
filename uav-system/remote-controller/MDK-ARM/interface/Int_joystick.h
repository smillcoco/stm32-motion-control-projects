#ifndef _INT_JOYSTICK_H_
#define _INT_JOYSTICK_H_


#include "adc.h"

typedef struct
{
    //油门
    int16_t thr;
    //偏航
    int16_t yaw;
    //俯仰
    int16_t pit;
    //滚转
    int16_t rol;
}joystick_struct;

//初始化adc摇杆  打开adc
void Int_joystick_init(void);


//读取adc数据 保存到结构体变量中
void Int_joystick_get(joystick_struct *joystick);

#endif // _INT_JOYSTICK_H_  

