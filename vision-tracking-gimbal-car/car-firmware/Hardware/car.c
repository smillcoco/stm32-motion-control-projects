#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "PID.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "Motor.h"

extern float Pitch,Roll,Yaw;//角度
extern short ax,ay,az;//角速度
extern short gx,gy,gz;//加速度

int output_L=0;
int output_R=0;
int PWM_speed_L=0;
int PWM_speed_R=0;
int Angle_output=0;
int PWM_PID_L=0; 
int PWM_PID_R=0; 


extern int16_t Encoder_actully_L;//左边编码器脉冲
extern int16_t Encoder_actully_R;//右边编码器脉冲


void Car_PID_control_speed(void)//速度环
{
		//目标脉冲值//可改变
		int Encoder_Target_L=0;
		int Encoder_Target_R=60;
		//角度环
		//输入：期望角度，真实角度，角速度
		Angle_output=Angle_PID(0,Yaw,gz);
		//速度环
		output_L=Encoder_Target_L+Angle_output;
		output_R=Encoder_Target_R-Angle_output;
		PWM_speed_L=Speed_left_PID(Encoder_actully_L, output_L);
		PWM_speed_R=Speed_right_PID(Encoder_actully_R,output_R);
		//积分限幅
		Limit(&PWM_speed_L,&PWM_speed_R);
		Motor(PWM_speed_L,PWM_speed_R);
	
}
void Car_PID_control_angle(void)//角度环
{
		//目标脉冲值//可改变
		int Encoder_Target_L=0;
		int Encoder_Target_R=0;
		//角度环
		//输入：期望角度，真实角度，角速度
		Angle_output=Angle_PID(-30,Yaw,gz);
		//速度环
		output_L=Encoder_Target_L+Angle_output;
		output_R=Encoder_Target_R-Angle_output;
		//PWM_speed_L=Speed_left_PID(Encoder_actully_L, output_L);
		//PWM_speed_R=Speed_right_PID(Encoder_actully_R,output_R);
		//积分限幅
		Limit(&output_L,&output_R);
		Motor(output_L,output_R);
	
}

void Car_PID_control(void)//双环//速度环与角度环//基本用不到
{
		//目标脉冲值//可改变
		int Encoder_Target_L=20;
		int Encoder_Target_R=20;
		//角度环
		//输入：期望角度，真实角度，角速度
		Angle_output=Angle_PID(-30,Yaw,gz);
		//速度环
		output_L=Encoder_Target_L+Angle_output;
		output_R=Encoder_Target_R-Angle_output;
		PWM_speed_L=Speed_left_PID(Encoder_actully_L, output_L);
		PWM_speed_R=Speed_right_PID(Encoder_actully_R,output_R);
		//积分限幅
		Limit(&PWM_speed_L,&PWM_speed_R);
		Motor(PWM_speed_L,PWM_speed_R);
	
}
