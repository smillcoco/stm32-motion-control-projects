#ifndef _PWM_H
#define _PWM_H
#include <stdint.h>

//void PWM_Init(void);
void Servo_PWM_Init(void);

void PWM_L_SetCompare(uint16_t Compare); //◊Û
void PWM_R_SetCompare(uint16_t Compare);

void Servo_PWM_X_SetCompare(uint16_t Compare);  //x÷·
void Servo_PWM_Y_SetCompare(uint16_t Compare);  //y÷·


#endif
