/**
  ******************************************************************************
  * @file           : retarget.c
  * @brief          : 输出重定向
  * @author         : yz.ouyang
  * @date           : 2023/3/16
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */
#include "main.h"
#include <stdio.h>
#include "usart.h"


extern UART_HandleTypeDef huart2;

#define DEBUG_UART  huart2

#if 1

__ASM (".global __use_no_semihosting");

#else

#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
  int handle;
};

#endif

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
  x = x;
}

void _ttywrch(int ch)
{
  ch = ch;
}

//重定义fputc函数
int fputc(int ch, FILE *f)
{

  while(( DEBUG_UART.Instance->SR & 0X40 ) == 0); /* 等待发送结束 */

  DEBUG_UART.Instance->DR = (uint8_t)ch;

  return ch;
}

/*************************************END OF FILE**********************/
