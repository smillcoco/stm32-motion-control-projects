#include "stm32f10x.h"                  // Device header
#include "IO.h"
#include "Delay.h"
#include "sys.h"
float gMsHcCount = 0;
/*
 *==============================================================================
 *函数名称：Drv_Hcsr04_Init
 *函数功能：初始化HC-SR04
 *输入参数：无
 *返回值：无
 *备  注：初始化HC-SR04引脚的同时，初始化了TIM2，用来记录高电平持续时间
          初始化完TIM2后，没有使能，当Echo收到高电平后使能
 *==============================================================================
 */
void Drv_Hcsr04_Init (void)   // Hc-sr04初始化
{    
	// 结构体定义
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   // 生成用于定时器设置的结构体  
	GPIO_InitTypeDef GPIO_InitStructure;   // GPIO结构体
	NVIC_InitTypeDef NVIC_InitStructure;   // NVIC结构体
	RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);   // 使能GPIO时钟

	// GPIO初始化  
	GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;   // 发送电平引脚  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽式输出  
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
	GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);  
		 
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;   // 返回电平引脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // 浮空输入  
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);    
	GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);      
		 
	// 定时器初始化 使用基本定时器TIM3  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   // 使能对应RCC时钟  
	// 配置定时器基础结构体  
	TIM_DeInit(TIM3);  
	TIM_TimeBaseStructure.TIM_Period = (1000-1);   // 设置在下一个更新事件装入活动的自动重装载寄存器周期的值(计数到1000为1ms ) 
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1);   // 设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   // 不分频  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   // TIM向上计数模式  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);   // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位         
				
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);   // 清除更新中断，免得一打开中断立即产生中断  
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);   // 打开定时器更新中断  

	// NVIC配置	     
		
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;   // 选择定时器2中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   // 抢占式中断优先级设置为0  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   // 响应式中断优先级设置为0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // 使能中断  
	NVIC_Init(&NVIC_InitStructure);  
	
	TIM_Cmd(TIM3,DISABLE);       
}

/*
 *==============================================================================
 *函数名称：Drv_Hcsr04_OpenTimerForHc
 *函数功能：打开定时器
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void Drv_Hcsr04_OpenTimerForHc (void)   // 打开定时器  
{  
	TIM_SetCounter(TIM3,0);   // 清除计数  
	gMsHcCount = 0;  
	TIM_Cmd(TIM3, ENABLE);   // 使能TIMx外设  
}

/*
 *==============================================================================
 *函数名称：Drv_Hcsr04_CloseTimerForHc
 *函数功能：关闭定时器
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void Drv_Hcsr04_CloseTimerForHc (void)   // 关闭定时器  
{  
	TIM_Cmd(TIM3, DISABLE);   // 使能TIMx外设  
}  

/*
 *==============================================================================
 *函数名称：Drv_Hcsr04_GetEchoTimer
 *函数功能：获取定时器定时时间 
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
unsigned int Drv_Hcsr04_GetEchoTimer (void)  //获取定时时间
{  
	u32 t = 0;  
	t = gMsHcCount * 1000;   // 得到MS  
	t += TIM_GetCounter(TIM3);   // 得到US  
	TIM2 -> CNT = 0;   // 将TIM2计数寄存器的计数值清零  
	delay_ms(50);
	return t;  
}

/*
 *==============================================================================
 *函数名称：Med_Hcsr04_GetLength
 *函数功能：获取测量距离
 *输入参数：无
 *返回值：无
 *备  注：一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号
					为了消除余震的影响，取五次数据的平均值进行加权滤波
 *==============================================================================
 */
float Med_Hcsr04_GetLength (void )  //计算测量距离
{  
	u32 t = 0;  
	int i = 0;  
	float lengthTemp = 0;  
	float sum = 0;  
	while(i!=5)  
	{  
		TRIG_Send = 1;   // 发送口高电平输出  
		delay_us(20);
		TRIG_Send = 0;  
		while(ECHO_Reci == 0);   // 等待接收口高电平输出  
		Drv_Hcsr04_OpenTimerForHc();   //打开定时器  
		i = i + 1;  
		while(ECHO_Reci == 1);  
		Drv_Hcsr04_CloseTimerForHc();   // 关闭定时器  
		t = Drv_Hcsr04_GetEchoTimer();   // 获取时间,分辨率为1us
		lengthTemp = ((float)t/58.0);   // cm  
		sum = lengthTemp + sum ;        
		}  
	lengthTemp = sum/5.0;  
	return lengthTemp;  
}

/*
 *==============================================================================
 *函数名称：TIM2_IRQHandler
 *函数功能：定时器2中断服务程序 
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */

void TIM3_IRQHandler (void)   // TIM3中断  用于超声波
{  
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   // 检查TIM2更新中断发生与否  
	{  
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // 清除TIMx更新中断标志   
		gMsHcCount++;  
	}  
}

