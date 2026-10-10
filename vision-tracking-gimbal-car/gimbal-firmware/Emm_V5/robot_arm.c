#include "stm32f10x.h"                  // Device header
#include <math.h>

//初始机械臂放置角度
#define Start_Base_corner           //底座角初始角度
#define Start_Big_angle             //大臂角初始角度
#define Start_Small_angle           //小臂角初始角度

//系统内部参数
#define Arm_compensation            //臂向补偿
#define Base_height                 //底座高度
#define Small_Arm_length            //小臂长度
#define Big_Arm_length              //大臂长度 

//计算过程参数
float Short_side;            //短边
float hypotenuse;            //斜边
float Vertical_projection;   //垂直投影

//结果调用参数
float Base_corner;           //底座角
float Big_angle;             //大臂角
float Big_angle1;            //大臂角1
float Big_angle2;            //大臂角2
float Small_angle;           //小臂角

void robot_arm_driver(int x, int y, int z)
{
    // 1. 计算底座角
    Base_corner = atan2f(x, y);  // 使用atan2处理所有象限情况
    
    // 2. 计算大臂角
    // 计算垂直投影
    Vertical_projection = sqrtf(x*x + y*y) - Arm_compensation;
    
    // 计算短边
    Short_side = fabsf(Base_height - z);
    
    // 计算斜边
    hypotenuse = sqrtf(Short_side*Short_side + Vertical_projection*Vertical_projection);
    
    // 计算大臂角1（余弦定理部分）
    Big_angle1 = acosf((Big_Arm_length*Big_Arm_length + hypotenuse*hypotenuse - Small_Arm_length*Small_Arm_length) 
                        / (2 * Big_Arm_length * hypotenuse));
    
    // 计算大臂角2（高度情况判断）
    if (Base_height == z) 
		{
        Big_angle2 = 90.0;
    } 
    else if (Base_height > z) 
		{
        Big_angle2 = atan2f(Vertical_projection, Short_side);
    } 
    else 
		{ // Base_height < z
        Big_angle2 = atan2f(Short_side, Vertical_projection) + 90.0;
    }
    
    Big_angle = Big_angle1 + Big_angle2;
    
    // 3. 计算小臂角
    Small_angle = acosf((Big_Arm_length*Big_Arm_length + Small_Arm_length*Small_Arm_length - hypotenuse*hypotenuse)
                         / (2 * Big_Arm_length * Small_Arm_length))
                  + Big_angle;
    
    // 最后调用机械臂控制函数
    // 机械臂控制(Base_corner, Big_angle, Small_angle);
}
