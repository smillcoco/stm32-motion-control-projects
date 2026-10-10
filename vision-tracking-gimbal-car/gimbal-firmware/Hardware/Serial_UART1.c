#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "IO.h"



char Serial_USART1_RxPacket[100];				//"@MSG\r\n"
uint8_t Serial_USART1_RxFlag = 0;

void Serial_USART1_Init(void) 
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = Servo_USART1_PIN_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Servo_USART1_PART, &GPIO_InitStructure);					//将引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Servo_USART1_PIN_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Servo_USART1_PART, &GPIO_InitStructure);					//将引脚初始化为上拉输入
	
	/*USART初始化*/
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = 115200;				//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(USART1, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
	/*中断输出配置*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*USART使能*/
	USART_Cmd(USART1, ENABLE);								//使能USART1，串口开始运行
}


void Serial_USART1_SendByte(uint8_t Byte)//发字符
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_USART1_SendArray(uint8_t *Array, uint16_t Length)//发字符数组
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_USART1_SendByte(Array[i]);
	}
}

void Serial_USART1_SendString(char *String)//发字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_USART1_SendByte(String[i]);
	}
}

uint32_t Serial_USART1_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_USART1_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_USART1_SendByte(Number / Serial_USART1_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc_USART1(int ch, FILE *f)
{
	Serial_USART1_SendByte(ch);
	return ch;
}

void Serial_USART1_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_USART1_SendString(String);
}


//void USART1_IRQHandler(void)
//{
//	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
//	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
//		
//		//使用状态机的思路，依次处理数据包的不同部分
//		
//		//当前状态为0，接收数据包包头
//		if (RxState == 0)
//		{
//			if (RxData == '@')			//如果数据确实是包头
//			{
//				RxState = 1;			//置下一个状态
//				pRxPacket = 0;			//数据包的位置归零
//			}
//		}
//		//当前状态为1，接收数据包数据
//		else if (RxState == 1)
//		{
//			if(RxData == '\r')
//			{
//				RxState = 2;//第一个停止位
//			}
//			else
//			{
//				Serial_USART1_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置  //RxData为接受到的数据
//				pRxPacket ++;				//数据包的位置自增

//			}

//		}
//		//当前状态为2，接收数据包包尾
//		else if (RxState == 2)
//		{
//			if (RxData == '\n')			//如果数据确实是包尾部
//			{
//				RxState = 0;			//状态归0
//				Serial_USART1_RxPacket[pRxPacket] = '\0';// 字符串终止
//				Serial_USART1_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
//			}
//		}
//		
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位
//	}
//}



