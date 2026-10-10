#ifndef __INT_KEY_H_
#define __INT_KEY_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


typedef enum
{
    KEY_NONE = 0,
    KEY_UP = 1,
    KEY_DOWN = 2,
    KEY_LEFT = 3,
    KEY_RIGHT = 4,
    KEY_LEFT_X = 5,
    KEY_RIGHT_X = 6,
    KEY_RIGHT_X_LONG =7,
}Key_type;

//获取当前按键是否被按下
Key_type Int_key_get(void);

#endif 
