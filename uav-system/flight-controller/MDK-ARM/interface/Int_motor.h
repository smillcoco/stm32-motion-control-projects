#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__

#include "tim.h"
#include "debug_printf.h"

typedef struct
{
    TIM_HandleTypeDef *tim;
    int16_t channel;
    int16_t speed;
 
}Motor_Struct;
//传入的参数其实是比较值，最大为1000，默认为200
void Int_motor_set_speed(Motor_Struct *motor);
//启动电机,传入具体电机的结构体
void Int_motor_start(Motor_Struct *motor);


#endif

