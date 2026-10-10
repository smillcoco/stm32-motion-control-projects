#ifndef _SEEIAL_UART1_H
#define _SEEIAL_UART1_H
#include <stdint.h>
#include <stdio.h>

void Serial_USART1_Init(void); 
void Serial_USART1_SendByte(uint8_t Byte);//发字符
void Serial_USART1_SendArray(uint8_t *Array, uint16_t Length);//发字符数组
void Serial_USART1_SendString(char *String);//发字符串
uint32_t Serial_USART1_Pow(uint32_t X, uint32_t Y);
void Serial_USART1_SendNumber(uint32_t Number, uint8_t Length);
int fputc_USART1(int ch, FILE *f);
void Serial_USART1_Printf(char *format, ...);
void USART1_IRQHandler(void);


#endif
