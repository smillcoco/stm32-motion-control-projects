#include "App_flight.h"

Gyro_Accel_Struct gyro_accel_data = {0};
Euler_struct euler_angle = {0};
Gyro_struct last_gyro = {0};
float gyro_z_sum = 0;
extern Remote_Data remote_data;
extern Flight_State flight_state;
extern TaskHandle_t RX_task_handle;
//PID调参是先调节内环再调解外环  

//俯仰角pid结构体 => 后续需要进行专业的PID调参 
PID_Struct pitch_pid = {.kp = -7.0, .ki = 0.000, .kd = 0.000};
//Y轴角速度pid结构体 => 对应俯仰角的内环 
PID_Struct gyro_y_pid = {.kp = 3.0, .ki = 0.000, .kd = 0.500};

//横滚角pid结构体 => 后续需要进行专业的PID调参 
PID_Struct roll_pid = {.kp = -7.0, .ki = 0.000, .kd = 0.000};
//X轴角速度pid结构体 => 对应横滚角的内环 
PID_Struct gyro_x_pid = {.kp = 3.0, .ki = 0.000, .kd = 0.500}; 

//偏航角属于辅助功能=>并不在平稳飞行的范围 只需要保证飞机不要在空中旋转即可=>明显的返回就行
//偏航角pid结构体 => 只通过比例部分来实现
PID_Struct yaw_pid = {.kp = -3.0, .ki = 0.000, .kd = 0.000};
//Z轴角速度pid结构体 => 对应偏航角的内环 
PID_Struct gyro_z_pid = {.kp = -5.0, .ki = 0.000, .kd = 0.000};

//定高pid结构体
PID_Struct height_pid = {.kp = -0.6, .ki = 0.000, .kd = -0.200};

//记录下的定高飞行高度
extern uint16_t fix_height;

//电机结构体
Motor_Struct left_top_motor = {.tim = &htim3, .channel = TIM_CHANNEL_1, .speed = 0};
Motor_Struct left_bottom_motor = {.tim = &htim4, .channel = TIM_CHANNEL_4, .speed = 0};
Motor_Struct right_top_motor = {.tim = &htim2, .channel = TIM_CHANNEL_2, .speed = 0};
Motor_Struct right_bottom_motor = {.tim = &htim1, .channel = TIM_CHANNEL_3, .speed = 0};


//飞控任务初始化(包括mpu6050的初始化,启动电机)
void App_flight_init(void)
{
    Int_MPU6050_Init();
    //启动电机
    Int_motor_start(&left_top_motor);
    Int_motor_start(&left_bottom_motor);
    Int_motor_start(&right_top_motor);
    Int_motor_start(&right_bottom_motor);
    //初始化激光测距仪
    Int_VL53L1X_Init();
}


/**
 * @brief 根据陀螺仪测量的数据 计算出欧拉角
 *
 */
void App_flight_get_euler_angle(void)
{
    //1.使用MPU6050的硬件接口  得到六轴数据 
    Int_MPU6050_Get_Data(&gyro_accel_data);

    //2.对角速度进行低通滤波 => 后续对采集数据的使用是及时性比较高的
    //目前系统对准确性要求没这么高,但一定要计算迅速
    //output= 加权系数*last_output+(1- 加权系数)*本次的测量值;
    gyro_accel_data.gyro.gyro_x = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_x, last_gyro.gyro_x);
    gyro_accel_data.gyro.gyro_y = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_y, last_gyro.gyro_y);
    gyro_accel_data.gyro.gyro_z = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_z, last_gyro.gyro_z);
    
    last_gyro.gyro_x = gyro_accel_data.gyro.gyro_x;
    last_gyro.gyro_y = gyro_accel_data.gyro.gyro_y;
    last_gyro.gyro_z = gyro_accel_data.gyro.gyro_z;

    // 先打印角速度
    //printf(":%d,%d,%d\n", gyro_accel_data.gyro.gyro_x, gyro_accel_data.gyro.gyro_y, gyro_accel_data.gyro.gyro_z);

    //3.对波动变化比较大的加速度 使用更高级的滤波方式=>卡尔曼滤波
    gyro_accel_data.accel.accel_x = Common_Filter_KalmanFilter(&kfs[0], gyro_accel_data.accel.accel_x);
    gyro_accel_data.accel.accel_y = Common_Filter_KalmanFilter(&kfs[1], gyro_accel_data.accel.accel_y);
    gyro_accel_data.accel.accel_z = Common_Filter_KalmanFilter(&kfs[2], gyro_accel_data.accel.accel_z);


    // 打印加速度
    //printf(":%d,%d,%d\n",gyro_accel_data.accel.accel_x, gyro_accel_data.accel.accel_y, gyro_accel_data.accel.accel_z);

    //4.根据加速度和角速度 计算出欧拉角
    //使用互补解算计算欧拉角 =>优先使用加速度解算=>俯仰角和横滚角
    //能够使用偏航角 =>只能使用角速度积分
    //euler_angle.pitch = atan2(gyro_accel_data.accel.accel_x * 1.0, gyro_accel_data.accel.accel_z)/3.14159*180;
    //euler_angle.roll = atan2(gyro_accel_data.accel.accel_y * 1.0, gyro_accel_data.accel.accel_z)/3.14159*180;
    //16位ADC的值转换为°/s =>量程是±2000°/s
    //gyro_z_sum += (gyro_accel_data.gyro.gyro_z *2000.0 / 32768.0)*0.006;
    //euler_angle.yaw = gyro_z_sum;

    //也可以使用移植的四元数姿态解算
    Common_IMU_GetEulerAngle(&gyro_accel_data, &euler_angle, 0.006);

    //打印角度
    //printf(":%.2f,%.2f,%.2f\n",euler_angle.pitch, euler_angle.roll, euler_angle.yaw);

}

//根据欧拉角   计算出PID的目标值
void App_flight_pid_process(void)
{
    //俯仰角
    //1.需要赋值目标值和测量值
    //外环的目标角度 =>如果是平稳飞行=>值为0=>如果需要遥控飞行=>目标角度就是遥控器的值
    //数值转换 => remote_data.pit(0-1000,500为中间点) 控制范围在±10°
    pitch_pid.desire = (remote_data.pit - 500) / 50.0;  
    //外环的测量值 => 俯仰角的角度
    pitch_pid.measure = euler_angle.pitch;  
    //内环的测量值 => 俯仰角的角速度 单位要保持一致
    //gyro_accel_data.gyro.gyro_y单位是adc,需要转换为°/s
    gyro_y_pid.measure = gyro_accel_data.gyro.gyro_y * 2000.0 / 32768.0;

    //2.进行pid运算
    Com_PID_Calc_Chain(&pitch_pid, &gyro_y_pid);

    //printf("%.2f,%.2f,%.2f\n",pitch_pid.err,pitch_pid.output,gyro_y_pid.output);

    //横滚角
    //1.需要赋值目标值和测量值
    //外环的目标角度 =>如果是平稳飞行=>值为0=>如果需要遥控飞行=>目标角度就是遥控器的值
    //数值转换 => remote_data.rol(0-1000,500为中间点) 控制范围在±10°
    roll_pid.desire = (remote_data.rol - 500) / 50.0;  
    //外环的测量值 => 俯仰角的角度
    roll_pid.measure = euler_angle.roll;  
    //内环的测量值 => 俯仰角的角速度 单位要保持一致
    //gyro_accel_data.gyro.gyro_x单位是adc,需要转换为°/s
    gyro_x_pid.measure = gyro_accel_data.gyro.gyro_x * 2000.0 / 32768.0;
    
    //2.进行pid运算
    Com_PID_Calc_Chain(&roll_pid, &gyro_x_pid);

    //偏航角
    //1.需要赋值目标值和测量值
    //外环的目标角度 =>如果是平稳飞行=>值为0=>如果需要遥控飞行=>目标角度就是遥控器的值
    //数值转换 => remote_data.yaw(0-1000,500为中间点) 控制范围在±10°
    yaw_pid.desire = (remote_data.yaw - 500) / 50.0;  
    //外环的测量值 => 俯仰角的角度
    yaw_pid.measure = euler_angle.yaw;  
    //内环的测量值 => 俯仰角的角速度 单位要保持一致
    //gyro_accel_data.gyro.gyro_z单位是adc,需要转换为°/s
    gyro_z_pid.measure = gyro_accel_data.gyro.gyro_z * 2000.0 / 32768.0;
    
    //2.进行pid运算
    Com_PID_Calc_Chain(&yaw_pid, &gyro_z_pid);

}
//根据pid的输出值 控制电机
void App_flight_control_process(void)
{
    //1.判断当前飞机的飞行状态
    switch (flight_state)
    {
    case IDLE:
        //一旦进入枷锁状态 => 需要将电机速度设置为0
        left_top_motor.speed = 0;
        left_bottom_motor.speed = 0;
        right_top_motor.speed = 0;
        right_bottom_motor.speed = 0;
        break;   
    case NORMAL:
        //俯仰角=>向前飞有角速度=>正误差=>需要一个向后飞的反馈效果=>前两个电机转的快后两个转的慢
        left_top_motor.speed = remote_data.thr + gyro_y_pid.output - gyro_x_pid.output +
        Com_limit(gyro_z_pid.output, 100, -100);
        left_bottom_motor.speed = remote_data.thr - gyro_y_pid.output - gyro_x_pid.output -
        Com_limit(gyro_z_pid.output, 100, -100);
        right_top_motor.speed = remote_data.thr + gyro_y_pid.output + gyro_x_pid.output - 
        Com_limit(gyro_z_pid.output, 100, -100);
        right_bottom_motor.speed = remote_data.thr - gyro_y_pid.output + gyro_x_pid.output +
        Com_limit(gyro_z_pid.output, 100, -100);
        break;   
    case FIX_HEIGHT:
        //只有定高状态才需要进行定高的PID计算 => 定高同时也需要平稳飞行
        left_top_motor.speed = remote_data.thr + gyro_y_pid.output - gyro_x_pid.output +
        Com_limit(gyro_z_pid.output, 100, -100) + height_pid.output;
        left_bottom_motor.speed = remote_data.thr - gyro_y_pid.output - gyro_x_pid.output -
        Com_limit(gyro_z_pid.output, 100, -100) + height_pid.output;
        right_top_motor.speed = remote_data.thr + gyro_y_pid.output + gyro_x_pid.output - 
        Com_limit(gyro_z_pid.output, 100, -100) + height_pid.output;
        right_bottom_motor.speed = remote_data.thr - gyro_y_pid.output + gyro_x_pid.output +
        Com_limit(gyro_z_pid.output, 100, -100) + height_pid.output;

        break;   
    case FAIL:
        // 进行故障处理 => 一直处理  到满足条件 修改状态为IDLE
        // 6ms 周期执行一次 => 每次所有电机速度降低3个单位
        left_top_motor.speed -= 2;
        left_bottom_motor.speed -= 2;
        right_top_motor.speed -= 2;
        right_bottom_motor.speed -= 2;

        // 判断：四个电机转速全部小于等于0时
        if (left_top_motor.speed <= 0 && left_bottom_motor.speed <= 0 &&
            right_top_motor.speed <= 0 && right_bottom_motor.speed <= 0)
        {
            xTaskNotifyGive(RX_task_handle); 
        }
        break;   
    default:
        break;
    }

    //限制电机速度上限值
    left_top_motor.speed = Com_limit(left_top_motor.speed, 800, 0);
    left_bottom_motor.speed = Com_limit(left_bottom_motor.speed, 800, 0);
    right_top_motor.speed = Com_limit(right_top_motor.speed, 800, 0);
    right_bottom_motor.speed = Com_limit(right_bottom_motor.speed, 800, 0);

    //安全限制 => 当油门设置为<50时=>强制将速度设置为0
    if(remote_data.thr < 50)
    {
        left_top_motor.speed = 0;
        left_bottom_motor.speed = 0;
        right_top_motor.speed = 0;
        right_bottom_motor.speed = 0;
    }

    //2.设置电机速度
    Int_motor_set_speed(&left_top_motor);
    Int_motor_set_speed(&left_bottom_motor);
    Int_motor_set_speed(&right_top_motor);
    Int_motor_set_speed(&right_bottom_motor);


}

//定高pid处理
void App_flight_fix_height_pid_process(void)
{
    //24ms一次
    height_pid.desire = fix_height;
    height_pid.measure = Int_VL53L1X_GetDistance();
    Com_PID_Calc(&height_pid);
    
}
