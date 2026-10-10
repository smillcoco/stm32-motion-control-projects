#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Motor.h"
#include "MPU6050.h"
#include "Delay.h"
#include "inv_mpu.h"
#include "MPU6050_EXTI.h"
#include "PID.h"
#include "Serial.h"
#include "Hcsr04.h"
#include "car.h"
#include "Mode.h"
#include "Serial_UART1.h"
#include "Key.h"
#include "IO.h"
#include "Servo.h"
#include "Servo_pid.h"


volatile uint8_t mpu6050_int_flag = 0;
int a=0;
int task = 0;
extern uint8_t Serial_TxPacket[];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
extern uint8_t Serial_RxPacket[];				//定义接收数据包数组
extern uint8_t Serial_RxFlag;					//定义接收数据包标志位

float Pitch,Roll,Yaw;
int16_t ax,ay,az,gx,gy,gz;

extern float gMsHcCount;
float gDistance = 0;//超声波返回距离的返回值

extern int16_t Encoder_actully_L;//左边编码器脉冲
extern int16_t Encoder_actully_R;//右边编码器脉冲
extern int output_L;
extern int output_R;
extern int PWM_speed_L;
extern int PWM_speed_R;

extern char Serial_USART1_RxPacket[];				//"@MSG\r\n"
extern uint8_t Serial_USART1_RxFlag;
extern int set_out;
extern uint8_t allow_key;

extern float x_pwm;
extern float y_pwm;

// 定义结构体类型
typedef struct {
    int x;  // x坐标
    int y;  // y坐标
} Point;
//upper_left.x可以直接得出x坐标
//upper_left.y可以直接得出y坐标
extern Point upper_left;   //左上
extern Point upper_right;  //右上
extern Point lower_right;  //右下
extern Point lower_left;   //左下
extern Point center;       //中心
extern Point actual;      //当前
extern char Serial_USART1_RxPacket[];				//"@MSG\r\n"
extern uint8_t Serial_USART1_RxFlag;
extern uint8_t recenter_flag; // 复位标志位

extern int actual_x, actual_y;

uint8_t KeyNum;			//定义用于接收键码的变量
float Angle;			//定义角度变量 	

int main(void)
{
	/*模块初始化*/
	mode_Init();
	Key_Init();
	Serial_USART1_Init();
	Serial_Init();
	OLED_Init();		//OLED初始化
	MPU6050_Init();		//MPU6050初始化
	MPU6050_DMP_Init();
	OLED_ShowString(1,1,"Init sucess");
	MPU6050_EXTI_Init();//陀螺仪初始化后再开启外部中断，避免初始化问题
	Motor_Init();
 	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
//	Motor(0,0);
//	Drv_Hcsr04_Init();
	OLED_Clear();
	
	
//	OLED_ShowString(2, 1, "Yaw:");					//OLED显示数据
//	OLED_ShowString(3, 1, "gz:");
//	OLED_ShowString(4, 1, "pwm:");
	
//	OLED_ShowString(2, 1, "real:");					//OLED显示数据
//	OLED_ShowString(3, 1, "target:");
//	OLED_ShowString(4, 1, "pwm:");

		OLED_ShowString(1, 1, "set_out:");
		OLED_ShowString(2, 1, "x_pwm:");
		OLED_ShowString(3, 1, "y_pwm:");
/*	
	while (1)
	{
		KeyNum = Key_GetNum();			//获取按键键码
		if (KeyNum == 1)				//按键1按下
		{
			Angle += 30;				//角度变量自增30
			if (Angle > 180)			//角度变量超过180后
			{
				Angle = 0;				//角度变量归零
			}
		}
		
		Servo_Y_SetAngle(Angle);			//设置舵机的角度为角度变量
		OLED_ShowNum(1, 7, Angle, 3);	//OLED显示角度变量
	}
*/
		while (1)
{
	

		OLED_ShowNum(1,8,set_out,5);
		OLED_ShowNum(2,8,x_pwm,5);
		OLED_ShowNum(3,8,y_pwm,5);
	
		if(Key_GetNum() == 1)
		{
			a=1;
			mode_start();
		}
		else if(Key_GetNum() == 2)
		{
			a=2;			
		}
		
		
		if(a==2)
		{
			mode_1();
		}
	
	
	
	
	
/*	
//	if (mpu6050_int_flag == 1)
//{
//	
//	//Control();//控制平衡车的代码
//	//读取陀螺仪的数据和编码器脉冲
//	Encoder_actully_L=Get_Encoder_L();
//	Encoder_actully_R=Get_Encoder_R();
//	MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);   //读取姿态角（角度），偏航角有偏移
//	MPU_Get_Gyroscope(&gx,&gy,&gz);            //读取角速度 
//	MPU_Get_Accelerometer(&ax,&ay,&az);        //读取角加速度
//	
//	mpu6050_int_flag = 0;

//	
//}
//		

//		Car_PID_control_angle();

//if((Yaw>-36)&&(Yaw<-24))
//{
//	  Motor(1000,1000);
//}
//		
//		
//		//Car_PID_control();
//		
//		printf("%d,%d,%d\n", Encoder_actully_R,output_R,PWM_speed_R);//实际脉冲，目标脉冲，pid输出的pwm
//		OLED_ShowSignedNum(2, 8, Yaw, 5);  
////		OLED_ShowSignedNum(3, 8, gz, 5);
////		OLED_ShowSignedNum(4, 8, output_R, 5);
//		
////		OLED_ShowSignedNum(2, 8, Encoder_actully_R, 5);  
//		OLED_ShowSignedNum(3, 8, output_R, 5);
//		OLED_ShowSignedNum(4, 8, PWM_speed_R, 5);


		gDistance = Med_Hcsr04_GetLength();  //获取超声波返回距离

//		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);   //读取姿态角（角度），偏航角有偏移
//    MPU_Get_Gyroscope(&gx,&gy,&gz);            //读取角速度 
//		MPU_Get_Accelerometer(&ax,&ay,&az);        //读取角加速度

		OLED_ShowSignedNum(2, 8, gDistance, 5);  
		OLED_ShowSignedNum(3, 8, Encoder_actully_R, 5);
		OLED_ShowSignedNum(4, 8, Roll, 5);
		//Motor(7200,7200);
*/
	
}
/*
void TIM3_IRQHandler(void)//定时器中断，得出一段时间内脉冲，1ms中断一次  //用于编码器  //用时要调整定时器频率
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		
		Encoder_actully_L = Get_Encoder_L();								//每隔固定时间段读取一次编码器计数增量值，即为速度值		
		Encoder_actully_R = Get_Encoder_R();								//每隔固定时间段读取一次编码器计数增量值，即为速度值		
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
*/
/*
void TIM3_IRQHandler (void)   // TIM3中断  用于超声波
{  
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   // 检查TIM2更新中断发生与否  
	{  
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // 清除TIMx更新中断标志   
		gMsHcCount++;  
	}  
}
*/

/*void EXTI9_5_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断5号线触发的中断
//	{
//		mpu6050_int_flag = 1;

//		EXTI_ClearITPendingBit(EXTI_Line5);		//清除外部中断5号线的中断标志位
//													//中断标志位必须清除
//													//否则中断将连续不断地触发，导致主程序卡死
//	}
//}


//void USART3_IRQHandler(void)
//{
//	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
//	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
//	{
//		uint8_t RxData = USART_ReceiveData(USART3);				//读取数据寄存器，存放在接收的数据变量
//		
//		//使用状态机的思路，依次处理数据包的不同部分
//		
//		//当前状态为0，接收数据包包头
//		if (RxState == 0)
//		{
//			if (RxData == 0xFF)			//如果数据确实是包头
//			{
//				RxState = 1;			//置下一个状态
//				pRxPacket = 0;			//数据包的位置归零
//			}
//		}
//		//当前状态为1，接收数据包数据
//		else if (RxState == 1)
//		{
//			Serial_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
//			pRxPacket ++;				//数据包的位置自增
//			if (pRxPacket >= 4)			//如果收够4个数据
//			{
//				RxState = 2;			//置下一个状态
//			}
//		}
//		//当前状态为2，接收数据包包尾
//		else if (RxState == 2)
//		{
//			if (RxData == 0xFE)			//如果数据确实是包尾部
//			{
//				RxState = 0;			//状态归0
//				Serial_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
//			}
//		}
//		
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		//清除标志位
//	}
//}
*/
}



void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{
		uint8_t RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
		
		//使用状态机的思路，依次处理数据包的不同部分
		
		//当前状态为0，接收数据包包头
		if (RxState == 0)
		{
			if ((RxData == '@')|| (RxData == '$'))		//如果数据确实是包头
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		//当前状态为1，接收数据包数据
		else if (RxState == 1)
		{
			if(RxData == '\r')
			{
				RxState = 2;//第一个停止位
			}
			else
			{
				Serial_USART1_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置  //RxData为接受到的数据
				pRxPacket ++;				//数据包的位置自增

			}

		}
		//当前状态为2，接收数据包包尾
		else if (RxState == 2)
		{
			if (RxData == '\n')			//如果数据确实是包尾部
			{
				RxState = 0;			//状态归0
				Serial_USART1_RxPacket[pRxPacket] = '\0';// 字符串终止
				Serial_USART1_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位
	}
}

