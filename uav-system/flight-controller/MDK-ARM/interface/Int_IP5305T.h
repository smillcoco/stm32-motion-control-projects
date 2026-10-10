#ifndef __INT_IP5305T__
#define __INT_IP5305T__

#include "main.h"
#include "freeRTOS.h"
#include "task.h"


//启动IP5305T电源 避免自动关机
void INT_IP5305T_start(void);

//关闭IP5305T电源
void INT_IP5305T_shutdown(void);

#endif

