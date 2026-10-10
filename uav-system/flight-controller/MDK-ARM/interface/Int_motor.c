#include "Int_motor.h"

//传入的参数其实是比较值，最大为1000，默认为200
void Int_motor_set_speed(Motor_Struct *motor)
{
    if(motor->speed > 1000)
    {
        printf("speed_ is too large\n");
        return;
    }
    __HAL_TIM_SET_COMPARE(motor->tim,motor->channel,motor->speed);
}

//启动电机,传入具体电机的结构体
void Int_motor_start(Motor_Struct *motor)
{
    __HAL_TIM_SET_COMPARE(motor->tim,motor->channel,0);
    HAL_TIM_PWM_Start(motor->tim,motor->channel);
}

