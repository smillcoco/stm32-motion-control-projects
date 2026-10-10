#include "stm32f10x.h"                  // Device header
#include "IO.h"

//x轴pid
//距离  位置式控制//也可用作速度环  
//输入：实际值，目标值
//舵机云台输入：实际坐标，目标坐标
float Position_X_PID(float Encoder,float Target)
{ 	
	
	 float Position_KP=0,Position_KI=0,Position_KD=0;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出

}

//y轴pid
//距离  位置式控制//也可用作速度环  
//输入：实际值，目标值
//舵机云台输入：实际坐标，目标坐标
float Position_Y_PID(float Encoder,float Target)
{ 	
	
	 float Position_KP=-1,Position_KI=0,Position_KD=0;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出

}


void Servo_Limit(float *motoA,float *motoB)
{
	if(*motoA>Servo_PWM_MAX)*motoA=Servo_PWM_MAX;
	if(*motoA<Servo_PWM_MIN)*motoA=Servo_PWM_MIN;
	if(*motoB>Servo_PWM_MAX)*motoB=Servo_PWM_MAX;
	if(*motoB<Servo_PWM_MIN)*motoB=Servo_PWM_MIN;

}
