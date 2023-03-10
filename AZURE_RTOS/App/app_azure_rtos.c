
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_azure_rtos.c
  * @author  MCD Application Team
  * @brief   azure_rtos application implementation file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "app_azure_rtos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN TX_Pool_Buffer */
/* USER CODE END TX_Pool_Buffer */
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;

/* USER CODE BEGIN PV */

#define  APP_CFG_TASK_START_STK_SIZE                    4096u
#define  APP_CFG_TASK_MsgPro_STK_SIZE                   4096u
#define  APP_CFG_TASK_COM_STK_SIZE                      4096u
#define  APP_CFG_TASK_USER_IF_STK_SIZE                  4096u
#define  APP_CFG_TASK_IDLE_STK_SIZE                     1024u
#define  APP_CFG_TASK_STAT_STK_SIZE                     1024u

#define  APP_CFG_TASK_START_PRIO                          2u
#define  APP_CFG_TASK_MsgPro_PRIO                         3u
#define  APP_CFG_TASK_USER_IF_PRIO                        4u
#define  APP_CFG_TASK_COM_PRIO                            5u
#define  APP_CFG_TASK_STAT_PRIO                           30u
#define  APP_CFG_TASK_IDLE_PRIO                           31u
#define LED_CFG_TASK_PRIO                                 6u

static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;
static int cnt = 0;
/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ��������
*	��    ��: thread_input ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 2
*********************************************************************************************************
*/
static  void  AppTaskStart (ULONG thread_input)
{
  (void)thread_input;

  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    tx_thread_sleep(1000);
  }
}

static  void LedTaskStart (ULONG thread_input)
{
  (void)thread_input;


  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    tx_thread_sleep(200);
    cnt++;
  }
}

static  TX_THREAD   AppTaskStartTCB;
static  uint64_t    AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 8];

static  TX_THREAD   LedTaskStartTCB;
static  uint64_t    LedTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 8];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Define the initial system.
  * @param  first_unused_memory : Pointer to the first unused memory
  * @retval None
  */
VOID tx_application_define(VOID *first_unused_memory)
{
  /* USER CODE BEGIN  tx_application_define */

  tx_thread_create(&AppTaskStartTCB,              /* ������ƿ��ַ */
                   "App Task Start",              /* ������ */
                   AppTaskStart,                  /* ������������ַ */
                   0,                             /* ���ݸ�����Ĳ��� */
                   &AppTaskStartStk[0],            /* ��ջ����ַ */
                   APP_CFG_TASK_START_STK_SIZE,    /* ��ջ�ռ��С */
                   APP_CFG_TASK_START_PRIO,        /* �������ȼ�*/
                   APP_CFG_TASK_START_PRIO,        /* ������ռ��ֵ */
                   TX_NO_TIME_SLICE,               /* ������ʱ��Ƭ */
                   TX_AUTO_START);                 /* �������������� */

  tx_thread_create(&LedTaskStartTCB,              /* ������ƿ��ַ */
                   "Led Task Start",              /* ������ */
                   LedTaskStart,                  /* ������������ַ */
                   0,                             /* ���ݸ�����Ĳ��� */
                   &LedTaskStartStk[0],            /* ��ջ����ַ */
                   APP_CFG_TASK_START_STK_SIZE,    /* ��ջ�ռ��С */
                   LED_CFG_TASK_PRIO,              /* �������ȼ�*/
                   LED_CFG_TASK_PRIO,              /* ������ռ��ֵ */
                   TX_NO_TIME_SLICE,               /* ������ʱ��Ƭ */
                   TX_AUTO_START);                 /* �������������� */

  /* USER CODE END  tx_application_define */

  VOID *memory_ptr;

  if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS)
  {
    /* USER CODE BEGIN TX_Byte_Pool_Error */

    /* USER CODE END TX_Byte_Pool_Error */
  }
  else
  {
    /* USER CODE BEGIN TX_Byte_Pool_Success */

    /* USER CODE END TX_Byte_Pool_Success */

    memory_ptr = (VOID *)&tx_app_byte_pool;

    if (App_ThreadX_Init(memory_ptr) != TX_SUCCESS)
    {
      /* USER CODE BEGIN  App_ThreadX_Init_Error */

      /* USER CODE END  App_ThreadX_Init_Error */
    }

    /* USER CODE BEGIN  App_ThreadX_Init_Success */

    /* USER CODE END  App_ThreadX_Init_Success */

  }

}

/* USER CODE BEGIN  0 */

/* USER CODE END  0 */
