#ifndef __DEBUG_PRINTF_H__
#define __DEBUG_PRINTF_H__

#include "usart.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"

/* ===================== 调试打印开关 ===================== */

/**
  * 编译期总开关：注释掉下面这行 → 完全禁用打印，零 CPU 开销
  * 保留下面这行   → 由运行时变量 debug_printf_enable 控制
  */
#define DEBUG_PRINTF_ENABLE

/**
  * 运行时打印开关（仅在 DEBUG_PRINTF_ENABLE 定义时有效）
  *   1 = 打印开启（地面调试）
  *   0 = 打印关闭（飞行模式，仅剩一条 if 判断的开销）
  */
extern uint8_t debug_printf_enable;

/**
  * FreeRTOS 互斥锁句柄 —— 保护 USART2 不被多个任务同时访问
  * 在 App_freeRTOS_start() 中调用 debug_printf_mutex_init() 初始化
  */
extern SemaphoreHandle_t printf_mutex;

void debug_printf_mutex_init(void);

/* ======================================================== */

#endif
