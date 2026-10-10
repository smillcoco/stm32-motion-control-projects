#include "stm32f10x.h"                  // Device header
#include "IO.h"


void Tacking_Init(void)
{

	RCC_APB2PeriphClockCmd(TACKING_CLK,ENABLE);//使能GPIOA时钟
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;//配置GPIOA为输入模式
	GPIO_Initstructure.GPIO_Pin = L1 | M | R1;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置GPIOA为输出模式
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TACKING_PORT,&GPIO_Initstructure);
	
}
