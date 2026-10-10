#include "stm32f10x.h"                  // Device header
#include "Servo_pid.h"
#include "Serial.h"

float red_actually_X_coord=0;
float red_target_X_coord=0;
float red_actually_Y_coord=0;
float red_target_Y_coord=0;

extern float red_openmv_X_actually_coord;
extern float red_openmv_X_target_coord;
extern float red_openmv_Y_actually_coord;
extern float red_openmv_Y_target_coord;

void Servo_X_motor(void)
{
		red_actually_X_coord = red_openmv_X_actually_coord;
		red_target_X_coord = red_openmv_X_target_coord;
		Position_X_PID(red_actually_X_coord,red_target_X_coord);
}

void Servo_Y_motor(void)
{
		red_actually_Y_coord = red_openmv_Y_actually_coord;
		red_target_Y_coord = red_openmv_Y_target_coord;
		Position_Y_PID(red_actually_Y_coord,red_target_Y_coord);
}

