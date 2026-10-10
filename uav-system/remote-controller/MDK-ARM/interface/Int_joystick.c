#include "Int_joystick.h"


uint16_t adc_buff[4] = {0};

//初始化adc摇杆  打开adc
void Int_joystick_init(void)
{

    //直接使用hal中的代码 开启adc
    //16位数据的地址值 其实是32位 32位数据的地址值也是32位
    //32单片机 使用的ADC转换为12位精度   范围 0-4095

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buff,4);
}


//读取adc数据 保存到结构体变量中
void Int_joystick_get(joystick_struct *joystick)
{
    //DMA不依赖cpu计算  所以读取的数据是实时保存到ADC_BUFF中
    //顺序是自定义的RAN1234    一定要对齐
    joystick->thr = adc_buff[0];
    joystick->yaw = adc_buff[1];
    joystick->pit = adc_buff[2]; 
    joystick->rol = adc_buff[3];



}
