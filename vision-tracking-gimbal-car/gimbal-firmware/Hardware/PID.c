#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "Motor.h"
//float Vertial_Kp=-180,Vertial_Kd=-1.2;      //直立环  数量值（kp：0-1000  kd：0-10）      //负  //临界值*0.6
//float Velocity_Kp=0,Velocity_Ki=0;      //速度环  数量级（kp：0-1）（ki=两百分之一kp）  //正

//float Vertial_Kp=-180,Vertial_Kd=-1.185;      //直立环  数量值（kp：0-1000  kd：0-10）      //负  //临界值*0.6
//float Velocity_Kp=0.55,Velocity_Ki=0.00275;      //速度环  数量级（kp：0-1）（ki=两百分之一kp）  //正



uint8_t stop;
//传感器数据变量
extern float Pitch,Roll,Yaw;//角度
extern short ax,ay,az;//角速度
extern short gx,gy,gz;//加速度
extern int16_t Encoder_actully_L;//左边编码器脉冲
extern int16_t Encoder_actully_R;//右边编码器脉冲
//闭环控制中间变量
int Vertial_out,Velocity_out,Turn_out,Target_Speed,Target_turn,MOTO1,MOTO2;
float Med_Angle=0;//平衡角度偏移量（机械中值）//每次开机都不一样，相加除以二，保留正负号

//角度环//mpu6050来调
//输入：期望角度，真实角度，角速度
int Angle_PID(float Med,float Angle,float gyro_Y)
{
	
	float Vertial_Kp=150,Vertial_Kd=0.8;      //直立环  数量值（kp：0-1000  kd：0-10）      //负  //临界值*0.6
	int pwm;
	pwm=Vertial_Kp*(Angle-Med)+Vertial_Kd*gyro_Y;
	
	return pwm;	
}
//速度环//增量式控制
//输入：左边电机实际脉冲，目标脉冲
int Speed_left_PID(float Encoder, float Target)
{
    float Kp =-5, Ki = 0;    //0.3m/s        // 
    static float error, pwm, err_last;             // 误差  输出  上一次误差
    error = Encoder - Target;                    // 求出速度偏差，由测量值减去目标值。
    pwm += Kp * (error - err_last) + Ki * error; // 使用增量 PI 控制器求出电机 PWM。
    err_last = error;                            // 保存上一次偏差
    return pwm;                                  // 增量输出
}
//速度环//增量式控制
//输入：右边电机实际脉冲，目标脉冲
int Speed_right_PID(float Encoder, float Target)
{
    float Kp =-5, Ki = 0;    //0.3m/s        //
    static float error, pwm, err_last;             // 误差  输出  上一次误差
    error = Encoder - Target;                    // 求出速度偏差，由测量值减去目标值。
    pwm += Kp * (error - err_last) + Ki * error; // 使用增量 PI 控制器求出电机 PWM。
    err_last = error;                            // 保存上一次偏差
    return pwm;                                  // 增量输出
}

//距离  位置式控制//也可用作速度环  
//输入：实际值，目标值
int Position_PID(int Encoder,int Target)
{ 	
	
	 float Position_KP=0,Position_KI=0,Position_KD=0;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出

}

/*平衡车调参

//直立环PD控制器//也是角度环//mpu6050来调
//输入：期望角度，真实角度，角速度
int Vertial(float Med,float Angle,float gyro_Y)
{
	
	float Vertial_Kp=-180,Vertial_Kd=-1.16;      //直立环  数量值（kp：0-1000  kd：0-10）      //负  //临界值*0.6
	int temp;
	temp=Vertial_Kp*(Angle-Med)+Vertial_Kd*gyro_Y;
	return temp;	
}

//速度环PI控制器//位置式pid
//输入：期望速度，左编码器，右编码器
int Velocity(int Target,int encoder_L,int encoder_R)
{
	
	float Velocity_Kp=0.55,Velocity_Ki=0.00275;  
	static int Err_LowOut_last,Encoder_S;
	static float a=0.7;
	int Err,Err_LowOut,temp;
  //1.计算偏差值
	Err=(encoder_L+encoder_R)-Target;
	//2.低通滤波	
	Err_LowOut=(1-a)*Err+a*Err_LowOut_last;
	Err_LowOut_last=Err_LowOut;
	//3.积分
	Encoder_S+=Err_LowOut;
	//4.积分限幅(-20000 20000)
	Encoder_S=Encoder_S>20000?20000:(Encoder_S<(-20000)?(-20000):Encoder_S);
	if(stop==1)Encoder_S=0,stop=0;
	//5.速度环计算
	temp=Velocity_Kp*Err_LowOut+Velocity_Ki*Encoder_S;
	return temp;
	
}

//转向环PI控制器
//输入：角速度，期望角度值(目标偏转角)
int Turn(float gyro_Z,int Target_turn)
{
	
	float Turn_Kp=0,Turn_Kd=0;            //转向环   //Turn_Kd=Velocity_Kp
	int temp;
	temp=Turn_Kp*Target_turn+Turn_Kd*gyro_Z;
	return temp;
	
}


void Control(void)//平衡车直立代码
{
	int PWM_out;
	//1.读取陀螺仪的数据和编码器脉冲
	Encoder_actully_L=Get_Encoder_L();
	Encoder_actully_R=Get_Encoder_R();
	MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);   //读取姿态角（角度），偏航角有偏移
  MPU_Get_Gyroscope(&gx,&gy,&gz);            //读取角速度 
	MPU_Get_Accelerometer(&ax,&ay,&az);        //读取角加速度
	//2.将数据传入到PID控制器中，计算出输出结果，左右电机转速
	Velocity_out=Velocity(Target_Speed,Encoder_actully_L,Encoder_actully_R);//速度环
	Vertial_out=Vertial(Velocity_out+Med_Angle,Roll,gx
	);//直立环
	Turn_out=Turn(gz,Target_turn);//转向环
	PWM_out=Vertial_out;
	//差速转向模型
	MOTO1=PWM_out-Turn_out;
	MOTO2=PWM_out+Turn_out;
	Limit(&MOTO1,&MOTO2);
	Motor(MOTO1,MOTO2);

}
*/

