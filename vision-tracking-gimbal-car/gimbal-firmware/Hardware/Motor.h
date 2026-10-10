#ifndef _MOTOR_H
#define _MOTOR_H
#include <stdint.h>

void Motor_Init(void);
void Motor(int16_t Motor_L,int16_t Motor_R);//Ð¡³µÇý¶¯
void Limit(int *motoA,int *motoB);

#endif
