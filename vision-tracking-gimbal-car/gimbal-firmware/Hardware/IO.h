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
#define KEY_PART						          GPIOA				
#define KEY_PART_PIN1						      GPIO_Pin_6		
#define KEY_PART_PIN2						      GPIO_Pin_7				
//Servo(和小车的pwm共用引脚)(根据情况做出取舍)
#define SERVO_PART                    GPIOA
#define SERVO_PART_PIN1               GPIO_Pin_11
#define SERVO_PART_PIN2               GPIO_Pin_8
//Servo 
#define Servo_PWM_MAX									999
#define Servo_PWM_MIN									-999
//Servo_USART1
#define Servo_USART1_PART             GPIOA
#define Servo_USART1_PIN_TX           GPIO_Pin_9
#define Servo_USART1_PIN_RX           GPIO_Pin_10


//#ifndef _IO_H
//#define _IO_H
//#include "stm32f10x.h"

///*---------------- 系统核心配置 -----------------*/
//// SWD 调试接口（必须保留）
//#define DEBUG_SWD_PORT               GPIOA
//#define SWDIO_PIN                    GPIO_Pin_13  // PA13
//#define SWCLK_PIN                    GPIO_Pin_14  // PA14

///*---------------- 电机驱动模块 -----------------*/
//#define MOTOR_PWM_PORT               GPIOA
//#define MOTOR_L_PWM_PIN              GPIO_Pin_8    // PA8 (TIM1_CH1)
//#define MOTOR_R_PWM_PIN              GPIO_Pin_11   // PA11 (TIM1_CH4)

//#define MOTOR_DIR_PORT               GPIOB
//#define MOTOR_L_IN1                  GPIO_Pin_12   // PB12
//#define MOTOR_L_IN2                  GPIO_Pin_13   // PB13
//#define MOTOR_R_IN1                  GPIO_Pin_14   // PB14
//#define MOTOR_R_IN2                  GPIO_Pin_15   // PB15

///*---------------- 编码器模块 -------------------*/
//#define ENCODER_L_PORT               GPIOA
//#define ENCODER_L_A                  GPIO_Pin_0    // PA0 (TIM2_CH1)
//#define ENCODER_L_B                  GPIO_Pin_1    // PA1 (TIM2_CH2)

//#define ENCODER_R_PORT               GPIOB
//#define ENCODER_R_A                  GPIO_Pin_6    // PB6 (TIM4_CH1)
//#define ENCODER_R_B                  GPIO_Pin_7    // PB7 (TIM4_CH2)

///*---------------- OLED显示模块 -----------------*/
//#define OLED_I2C_PORT                GPIOB
//#define OLED_SCL_PIN                 GPIO_Pin_8    // PB8 (软件 SCL)
//#define OLED_SDA_PIN                 GPIO_Pin_9    // PB9 (软件 SDA)

///*---------------- MPU6050模块 -----------------*/
//#define MPU6050_IIC_PORT             GPIOB
//#define MPU6050_SCL_PIN              GPIO_Pin_4    // PB4 (软件 SCL，需禁用 JTAG)
//#define MPU6050_SDA_PIN              GPIO_Pin_3    // PB3 (软件 SDA，需禁用 JTAG)

///*---------------- 舵机控制模块 -----------------*/
//#define SERVO_PWM_PORT               GPIOB
//#define SERVO_X_PIN                  GPIO_Pin_5    // PB5 (TIM3_CH2)
//#define SERVO_Y_PIN                  GPIO_Pin_10   // PB10 (TIM2_CH3)

///*---------------- 蓝牙模块（USART3） -----------------*/
//#define BT_UART_PORT                 GPIOB
//#define BT_TX_PIN                    GPIO_Pin_10   // PB10 (USART3_TX)
//#define BT_RX_PIN                    GPIO_Pin_11   // PB11 (USART3_RX)

///*---------------- CH340（USART2） -----------------*/
//#define CH340_UART_PORT              GPIOA
//#define CH340_TX_PIN                 GPIO_Pin_2    // PA2 (USART2_TX)
//#define CH340_RX_PIN                 GPIO_Pin_3    // PA3 (USART2_RX)

///*---------------- OpenMV（USART1） -----------------*/
//#define OPENMV_UART_PORT             GPIOA
//#define OPENMV_TX_PIN                GPIO_Pin_9    // PA9 (USART1_TX)
//#define OPENMV_RX_PIN                GPIO_Pin_10   // PA10 (USART1_RX)

///*---------------- 超声波模块 -------------------*/
//#define HCSR04_PORT                  GPIOA
//#define TRIG_PIN                     GPIO_Pin_4    // PA4
//#define ECHO_PIN                     GPIO_Pin_5    // PA5 (TIM2_CH1_ETR)

///*---------------- 用户交互模块 -----------------*/
//#define BUZZER_PORT                  GPIOC
//#define BUZZER_PIN                   GPIO_Pin_13   // PC13

//#define KEY_PORTB                    GPIOB
//#define KEY1_PIN                     GPIO_Pin_0    // PB0
//#define KEY2_PIN                     GPIO_Pin_1    // PB1

//#define KEY_PORTA                    GPIOA
//#define KEY3_PIN                     GPIO_Pin_6   
//#define KEY4_PIN                     GPIO_Pin_7   

///*---------------- 灯模块 -------------------*/
//#define LED_PORT                     GPIOC
//#define LED1_PIN                		 GPIO_Pin_14
//#define LED2_PIN                		 GPIO_Pin_15

///*---------------- 八路灰度传感器模块 -----------------*/
//#define TRACKING_PORT1               GPIOA//PA15 的启用：需在初始化代码中禁用 JTAG
//#define TRACKING_PINS1               (GPIO_Pin_12 | GPIO_Pin_15)  // PA12, PA15
//#define TRACKING_PORT2               GPIOB
//#define TRACKING_PINS2               (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5)  // PB0-PB2, PB5
//#define TRACKING_PORT3               GPIOC
//#define TRACKING_PINS3               (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)  // PC13-PC15

///*---------------- 初始化配置宏 -----------------*/
//#define HARDWARE_INIT_CONFIG() \
//    do { \
//        /* 禁用 JTAG，释放 PB3/PB4 */ \
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); \
//        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); \
//        /* 禁用 TIM1 的 PA9/PA10 复用 */ \
//        GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE); \
//        /* 启用所有 GPIO 时钟 */ \
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
//                              RCC_APB2Periph_GPIOC, ENABLE); \
//    } while(0)

//#endif	
#endif
