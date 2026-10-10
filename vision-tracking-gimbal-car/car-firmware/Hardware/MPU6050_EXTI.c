#include "stm32f10x.h"                  // Device header
#include "PID.h"
/**
  * @brief  MPU6050外部中断引脚
  * @param  
  * @retval 
  * @attention  RCC_APB2Periph_AFIO开启复用时钟，复用为外部中断
  */
void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;//PB5配置为上拉输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource5);//将GPIO口配置成外部中断 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
	
	/**
  * @brief  中断优先级
  * @param  
  * @retval 
  * @attention  配置MPU6050外部中断为最高，因为要时刻采集数据维持平衡车
  */

	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4级抢占,4级响应。
	
	//配置MPU6050外部中断
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能该通道中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//主优先级0，最高
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//子优先级，最高
	NVIC_Init(&NVIC_InitStruct);//初始化
/*	//使用串口时再用
	//配置串口中断
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//串口中断
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能该通道中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//主优先级2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//子优先级2
	NVIC_Init(&NVIC_InitStruct);	//初始化
*/
}

/**
  * 函    数：EXTI15_10外部中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
/*
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断14号线触发的中断
	{
		Control();
		EXTI_ClearITPendingBit(EXTI_Line5);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
*/

