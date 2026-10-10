#include "Int_IP5305T.h"


//启动IP5305T电源 避免自动关机
void INT_IP5305T_start(void)
{
    //飞机引脚为pb2，遥控器引脚为pb15
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin,GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(100));
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin,GPIO_PIN_RESET);

}

void INT_IP5305T_shutdown(void)
{
    //一秒钟连按两次按钮就是关机
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin,GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(100));
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin,GPIO_PIN_RESET);

    vTaskDelay(pdMS_TO_TICKS(200));

    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin,GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(100));
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port,POWER_KEY_Pin,GPIO_PIN_RESET);


}
