#ifndef _PID_H
#define _PID_H

int Vertial(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_L,int encoder_R);
int Turn(float gyro_Z,int Target_turn);
void Control(void);
int Angle_PID(float Med,float Angle,float gyro_Y);
int Speed_left_PID(float Encoder, float Target);
int Speed_right_PID(float Encoder, float Target);
int Position_PID(int Encoder,int Target);



#endif
