#include "Int_led.h"

//打开led灯
void Int_led_turn_on(LED_Struct *led)
{
    //设置引脚为低电平  开灯
    HAL_GPIO_WritePin(led->port,led->pin,GPIO_PIN_RESET); 

}

//关闭led灯
void Int_led_turn_off(LED_Struct *led)
{
    //设置引脚为高电平  关灯
    HAL_GPIO_WritePin(led->port,led->pin,GPIO_PIN_SET); 
}

//翻转led灯
void Int_led_toggle(LED_Struct *led)
{
    HAL_GPIO_TogglePin(led->port,led->pin);
}

