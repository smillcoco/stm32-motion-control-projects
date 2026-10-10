#ifndef _ENCODER_H
#define _ENCODER_H
#include <stdint.h>
void Encoder_TIM2_Init(void);
void Encoder_TIM4_Init(void);
int16_t Get_Encoder_L(void);//¶ÁÈ¡Âö³å
int16_t Get_Encoder_R(void);//¶ÁÈ¡Âö³å
void Motor_Speed(void);//±àÂëÆ÷²âËÙ

#endif
