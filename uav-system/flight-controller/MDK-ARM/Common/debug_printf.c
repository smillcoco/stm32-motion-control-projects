#include "debug_printf.h"

// 重定向fputc函数，使printf输出到串口2
// 在 main.c 里 #include "debug_printf.h" 后调用 printf("Hello\r\n") 即可。

#ifdef DEBUG_PRINTF_ENABLE

// 运行时打印开关：1=开启（地面调试），0=关闭（飞行模式）
uint8_t debug_printf_enable = 1;

// (创建串口2打印互斥量)FreeRTOS 互斥锁 —— 保护 USART2 不被多个任务同时访问
SemaphoreHandle_t printf_mutex = NULL;

void debug_printf_mutex_init(void)
{
    //创建互斥锁
    printf_mutex = xSemaphoreCreateMutex();
}

int fputc(int ch, FILE *f)
{
    (void)f;  // 未使用

    // 飞行模式：跳过所有打印
    if (!debug_printf_enable)
    {
        return ch;
    }

    // 获取互斥锁，防止多任务同时打印导致数据交织
    // 若调度器未启动（printf_mutex == NULL），则跳过加锁
    if (printf_mutex != NULL)
    {
        //获取互斥锁，超时时间为50ms
        if (xSemaphoreTake(printf_mutex, pdMS_TO_TICKS(50)) != pdTRUE)
        {
            return ch;  // 超时，放弃本次打印
        }
    }

    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 10);

    // 释放互斥锁
    if (printf_mutex != NULL)
    {
        //释放互斥锁
        xSemaphoreGive(printf_mutex);
    }

    return ch;
}

#else

// 编译期关闭打印，fputc 为空函数，零开销
/**
 * @brief 重定向标准输出字符函数（桩函数）
 *
 * 重写标准库 fputc 函数，用于将 printf 等标准输出重定向至串口。
 * 当前为空实现，直接丢弃输出字符。
 *
 * @param ch 待输出的字符
 * @param f  文件指针（未使用）
 * @return 原样返回输入的字符
 */
int fputc(int ch, FILE *f)
{
    (void)ch;
    (void)f;
    return ch;
}

#endif
