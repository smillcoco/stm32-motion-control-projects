#ifndef _SERVO_PID_H
#define _SERVO_PID_H


float Position_X_PID(float Encoder,float Target);
float Position_Y_PID(float Encoder,float Target);
void Servo_Limit(float *motoA,float *motoB);


#endif

