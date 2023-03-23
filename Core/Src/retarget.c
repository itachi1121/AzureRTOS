/**
  ******************************************************************************
  * @file           : retarget.c
  * @brief          : ����ض���
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
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
  int handle;
};

#endif

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
  x = x;
}

void _ttywrch(int ch)
{
  ch = ch;
}

//�ض���fputc����
int fputc(int ch, FILE *f)
{

  while(( DEBUG_UART.Instance->SR & 0X40 ) == 0); /* �ȴ����ͽ��� */

  DEBUG_UART.Instance->DR = (uint8_t)ch;

  return ch;
}

/*************************************END OF FILE**********************/
