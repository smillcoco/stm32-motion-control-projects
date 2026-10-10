#ifndef _HCSR04_H
#define _HCSR04_H

void Drv_Hcsr04_Init (void);   // Hc-sr04初始化
void Drv_Hcsr04_OpenTimerForHc (void);   // 打开定时器  
void Drv_Hcsr04_CloseTimerForHc (void);   // 关闭定时器  
unsigned int Drv_Hcsr04_GetEchoTimer (void);  //获取定时时间
float Med_Hcsr04_GetLength (void );  //计算测量距离


#endif
