#include "stm32f10x.h"                  // Device header
#include "IO.h"

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BUZZER_PART_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZER_PART, &GPIO_InitStructure);
	
	GPIO_SetBits(BUZZER_PART, BUZZER_PART_PIN);
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(BUZZER_PART, BUZZER_PART_PIN);
}

void Buzzer_OFF(void)
{
	GPIO_SetBits(BUZZER_PART, BUZZER_PART_PIN);
}

void Buzzer_Turn(void)
{
	if (GPIO_ReadOutputDataBit(BUZZER_PART, BUZZER_PART_PIN) == 0)
	{
		GPIO_SetBits(BUZZER_PART, BUZZER_PART_PIN);
	}
	else
	{
		GPIO_ResetBits(BUZZER_PART, BUZZER_PART_PIN);
	}
}
