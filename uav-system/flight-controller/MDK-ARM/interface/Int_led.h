#ifndef _INT_LED_H_
#define _INT_LED_H_

#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} LED_Struct;

//打开led灯
void Int_led_turn_on(LED_Struct *led);

//关闭led灯
void Int_led_turn_off(LED_Struct *led);

//翻转led灯
void Int_led_toggle(LED_Struct *led);

#endif // _INT_LED_H_
