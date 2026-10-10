#ifndef __APP_FLIGHT__
#define __APP_FLIGHT__

#include "Int_mpu6050.h"
#include "debug_printf.h"
#include "Com_filter.h"
#include "math.h"
#include "Com_imu.h"
#include "Com_pid.h"
#include "Int_motor.h"
#include "Int_VL53L1X.h"

/**
 * @brief 根据陀螺仪测量的数据 计算出欧拉角
 * 
 */
void App_flight_get_euler_angle(void);

//根据欧拉角   计算出PID的目标值
void App_flight_pid_process(void);

//根据pid的输出值 控制电机
void App_flight_control_process(void); 

//定高pid处理
void App_flight_fix_height_pid_process(void);


//飞控任务初始化(包括mpu6050的初始化,启动电机)
void App_flight_init(void);




#endif // __APP_FLIGHT__
