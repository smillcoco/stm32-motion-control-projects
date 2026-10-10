#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Serial_UART1.h"
#include "Servo_pid.h"
#include "Servo.h"
#include "PWM.h"

int set_out = 0;      //记录铅笔框以及中心点位的标志位  1-4分别为左上角顺时针的位置，5为中心
uint8_t allow_key = 1; // 按键使能标志（关键同步控制）
// 定义结构体类型
typedef struct {
    int x;  // x坐标
    int y;  // y坐标
} Point;
//upper_left.x可以直接得出x坐标
//upper_left.y可以直接得出y坐标
Point upper_left = {0, 0};   //左上
Point upper_right = {0, 0};  //右上
Point lower_right = {0, 0};  //右下
Point lower_left = {0, 0};   //左下
Point center = {0, 0};       //中心
Point actual = {0, 0};      //当前
extern char Serial_USART1_RxPacket[];				//"@MSG\r\n"
extern uint8_t Serial_USART1_RxFlag;
uint8_t recenter_flag = 0; // 复位标志位
int actual_x, actual_y;
float x_pwm = 0;
float y_pwm = 0;

void mode_Init(void)
{
		Key_Init();
		Serial_USART1_Init();
		Servo_Init();
		Servo_PWM_Init();	
}


void mode_start(void)
{
    if (allow_key == 1) 
	{		
			
        switch (set_out) 
			{
            case 0: // 初始状态
                
                Serial_USART1_SendString("a"); // 开始设置左上角
                break;
            case 1: // 左上角已设置

                Serial_USART1_SendString("b"); // 开始设置右上角
                break;
            case 2: // 右上角已设置

                Serial_USART1_SendString("c"); // 开始设置右下角
                break;
            case 3: // 右下角已设置

                Serial_USART1_SendString("d"); // 开始设置左下角
                break;
            case 4: // 左下角已设置

                Serial_USART1_SendString("e"); // 开始设置中心点
                break;
					
      }
			allow_key = 0;  // 发送指令后立即锁定按键
  }

    // 串口数据接收逻辑
    if (Serial_USART1_RxFlag == 1) 
	{
        int x, y;
				//sscanf 仅解析当前缓冲区中的最新数据
        if (sscanf(Serial_USART1_RxPacket, "%d,%d", &x, &y) == 2) 
		{
					if (x >= 0 && y >= 0)
				{
            switch (set_out) 
					{
                case 0:
                    upper_left.x = x;
                    upper_left.y = y;
										set_out = 1;
                    break;
                case 1:
                    upper_right.x = x;
                    upper_right.y = y;
										set_out = 2;
                    break;
                case 2:
                    lower_right.x = x;
                    lower_right.y = y;
										set_out = 3;
                    break;
                case 3:
                    lower_left.x = x;
                    lower_left.y = y;
										set_out = 4;
                    break;
                case 4:
                    center.x = x;
                    center.y = y;
                    set_out = 0; // 全部设置完成
                    break;
            }
						allow_key = 1; //解锁按键
				}
			
		}
        Serial_USART1_RxFlag = 0;
	}
}

void mode_1(void)
{	

    // 1. 设置目标坐标
    int target_x = center.x;
    int target_y = center.y;
		// 2. 发送复位指令
    Serial_USART1_SendString("z"); 
    
		if (Serial_USART1_RxFlag == 1) 
  {
       // 3. 解析坐标
       if (sscanf(Serial_USART1_RxPacket, "%d,%d", &actual_x, &actual_y) == 2) 
      {
         // 4. 单次PID调整
         x_pwm = Position_X_PID(actual_x, target_x);
         y_pwm = Position_Y_PID(actual_y, target_y);
         Servo_Limit(&x_pwm, &y_pwm);
				 //舵机的中立位置(1500),对应 1.5ms 的脉冲宽度（占空比）
         Servo_PWM_X_SetCompare(x_pwm + 1500);
         Servo_PWM_Y_SetCompare(y_pwm + 1500);
				 
      }
         Serial_USART1_RxFlag = 0;
  }
                    
}

void mode_2(void)
{
	int mode2_flag = 0;
	if(mode2_flag == 0)//左上到右上
	{
		
		
		Servo_PWM_X_SetCompare(x_pwm + 1500);
    Servo_PWM_Y_SetCompare(y_pwm + 1500);
	}
	
		
	
	
}

		
		
			
		
	



