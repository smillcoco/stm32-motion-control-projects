#ifndef _IO_H
#define _IO_H
#include <stdint.h>

//OLED
#define OLED_PART                     GPIOB
#define OLED_PART_SCL                 GPIO_Pin_8  
#define OLED_PART_SDA                 GPIO_Pin_9  
//PWM
#define PWM_PART											GPIOA
#define PWM_PART_PIN_L                GPIO_Pin_11  
#define PWM_PART_PIN_R                GPIO_Pin_8  
//MOTOR
#define MOTOR_PART										GPIOB
#define MOTOR_PART_PIN_L_1						GPIO_Pin_13				
#define MOTOR_PART_PIN_L_2						GPIO_Pin_12		
#define MOTOR_PART_PIN_R_1						GPIO_Pin_14				
#define MOTOR_PART_PIN_R_2						GPIO_Pin_15	
#define PWM_MAX												7200
#define PWM_MIN												-7200
//ENCODER
#define ENCODER_PARTA									GPIOA
#define ENCODER_PARTB									GPIOB
#define ENCODER_PARTA_PIN_L_1					GPIO_Pin_0				
#define ENCODER_PARTA_PIN_L_2					GPIO_Pin_1		
#define ENCODER_PARTB_PIN_R_1					GPIO_Pin_6			
#define ENCODER_PARTB_PIN_R_2					GPIO_Pin_7			
//MPU6050
#define MPU6050_IIC_GPIO              GPIOB
#define MPU6050_IIC_SCL_Pin           GPIO_Pin_4	         
#define MPU6050_IIC_SDA_Pin           GPIO_Pin_3	         

#define	MPU6050_IIC_SCL               PBout(4)
#define	MPU6050_IIC_SDA               PBout(3)  
#define	MPU6050_IIC_SDA_IN            PBin(3)              //读数据端口
#define MPU6050_IIC_delay_4us()       delay_us(4)
//SERIAL   //蓝牙数据包接受
#define SERIAL_PARTB								  GPIOB
#define SERIAL_PART_PIN_TX					  GPIO_Pin_10				
#define SERIAL_PART_PIN_RX					  GPIO_Pin_11
/*发送
Serial_TxPacket[0] = 0x01;
Serial_TxPacket[1] = 0x02;
Serial_TxPacket[2] = 0x03;
Serial_TxPacket[3] = 0x04;
Serial_SendPacket();		//串口发送数据包Serial_TxPacket
//串口重定向打印数据
printf("%d,%d,%d\n", Encoder_actully_R,output_R,PWM_speed_R);//实际脉冲，目标脉冲，pid输出的pwm
*/

/*接受
if (Serial_GetRxFlag() == 1)	//如果接收到串口中断数据包
{

	if(Serial_TxPacket[0] == 0x01)
	{
	//执行任务
	}//这个要结合接受使用，常用于蓝牙模块，要配置中断
	//下面这个串口发送什么就接受什么，不用配置中断
	RxData = Serial_GetRxData();		  //获取串口接收的数据
	Serial_SendByte(RxData);			    //串口将收到的数据回传回去，用于测试
	OLED_ShowHexNum(1, 8, RxData, 2);	//显示串口接收的数据
	
}
*/
//HCSR04
#define HCSR04_PORT                   GPIOA   // 定义IO口
#define HCSR04_CLK                    RCC_APB2Periph_GPIOA  // 开启GPIO时钟  
#define HCSR04_TRIG                   GPIO_Pin_3   // 定义Trig对应引脚
#define HCSR04_ECHO                   GPIO_Pin_2   // 定义Echo对应引脚
#define TRIG_Send                     PAout(3)   // 将TRIG_Send映射到
#define ECHO_Reci                     PAin(2)   // 将ECHO_Reci映射到
//TACKING
#define TACKING_CLK                   RCC_APB2Periph_GPIOA  // 开启GPIO时钟
#define TACKING_PORT                  GPIOA   // 定义IO口
#define L1                            GPIO_Pin_3
#define M                             GPIO_Pin_4
#define R1                            GPIO_Pin_5
//LED
#define LED_PART						          GPIOA				
#define LED_PART_PIN						      GPIO_Pin_14				
//BUZZER
#define BUZZER_PART						        GPIOA				
#define BUZZER_PART_PIN						    GPIO_Pin_14				
//KEY
#define KEY_PART						          GPIOB				
#define KEY_PART_PIN1						      GPIO_Pin_14		
#define KEY_PART_PIN2						      GPIO_Pin_13				





#endif
