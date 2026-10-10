#ifndef _PWM_H
#define _PWM_H
#include <stdint.h>

void PWM_Init(void);
void PWM_L_SetCompare(uint16_t Compare); //×ó
void PWM_R_SetCompare(uint16_t Compare);

#endif
