#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "IO.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR_PART_PIN_L_1|MOTOR_PART_PIN_L_2|MOTOR_PART_PIN_R_1|MOTOR_PART_PIN_R_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_PART,&GPIO_InitStructure);
	
//	PWM_Init();
}

void Motor(int16_t Motor_L,int16_t Motor_R)//小车驱动
{
	if (Motor_L>0)
	{
		GPIO_SetBits(MOTOR_PART,MOTOR_PART_PIN_L_1);//方向控制
		GPIO_ResetBits(MOTOR_PART,MOTOR_PART_PIN_L_2);
//		PWM_L_SetCompare(Motor_L);//速度大小
	}
	else if (Motor_L < 0)
	{
		GPIO_SetBits(MOTOR_PART,MOTOR_PART_PIN_L_2);//方向控制
		GPIO_ResetBits(MOTOR_PART,MOTOR_PART_PIN_L_1);
//		PWM_L_SetCompare(-Motor_L);//速度大小
	}
	else if (Motor_L == 0)
	{
			GPIO_ResetBits(MOTOR_PART,MOTOR_PART_PIN_L_1|MOTOR_PART_PIN_L_2);//停止
//		  PWM_L_SetCompare(0);
	}

	if (Motor_R>0)
	{
		GPIO_SetBits(MOTOR_PART,MOTOR_PART_PIN_R_1);//方向控制
		GPIO_ResetBits(MOTOR_PART,MOTOR_PART_PIN_R_2);
//		PWM_R_SetCompare(Motor_R);//速度大小
	}
	else if (Motor_R<0)
	{
		GPIO_SetBits(MOTOR_PART,MOTOR_PART_PIN_R_2);//方向控制
		GPIO_ResetBits(MOTOR_PART,MOTOR_PART_PIN_R_1);
//		PWM_R_SetCompare(-Motor_R);//速度大小
	}
	else if (Motor_R == 0)
	{
			GPIO_ResetBits(MOTOR_PART,MOTOR_PART_PIN_R_1|MOTOR_PART_PIN_R_2);//停止
//		  PWM_R_SetCompare(0);
	}
}

void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;

}
