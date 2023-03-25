
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
#include <stdio.h>
#include <stdarg.h>
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


/* 数值越小，优先级越高 */
#define  APP_CFG_TASK_START_PRIO                          2u
#define  APP_CFG_TASK_MsgPro_PRIO                         3u
#define  APP_CFG_TASK_USER_IF_PRIO                        4u
#define  APP_CFG_TASK_COM_PRIO                            5u
#define  APP_CFG_TASK_STAT_PRIO                           30u
#define  APP_CFG_TASK_IDLE_PRIO                           31u
#define  LED_CFG_TASK_PRIO                                 6u
#define  SEMP_CFG_TASK_PRIO                                5u

static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;
static int cnt = 0;

TX_SEMAPHORE Semaphore;


static TX_MUTEX AppPrintfSemp; /* 用于 printf 互斥 */
/*
*********************************************************************************************************
* 函 数 名: App_Printf
* 功能说明: 线程安全的 printf 方式 
* 形 参: 同 printf 的参数。
* 在 C 中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
* 返 回 值: 无
*********************************************************************************************************
*/
static void App_Printf(const char *fmt, ...)
{
 char buf_str[200 + 1]; /* 特别注意，如果 printf 的变量较多，注意此局部变量的大小是否够用 */
 va_list v_args;
 va_start(v_args, fmt);
 (void)vsnprintf((char *)&buf_str[0], (size_t ) sizeof(buf_str), (char const *) fmt, v_args);
 va_end(v_args);

 /* 互斥操作 */
 tx_mutex_get(&AppPrintfSemp, TX_WAIT_FOREVER);
 printf("%s", buf_str);
 tx_mutex_put(&AppPrintfSemp);
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务。
*	形    参: thread_input 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 2
*********************************************************************************************************
*/
static  void  AppTaskStart (ULONG thread_input)
{
  (void)thread_input;

  tx_mutex_create(&AppPrintfSemp,"AppPrintfSemp",TX_INHERIT);
  
  UINT i = 0;
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    
    tx_thread_sleep(200);

    //App_Printf("app task %d\r\n", ++i);
    App_Printf("app task %d\r\n", ++i);
    if(i % 5 == 0)
    {
  //    tx_semaphore_put(&Semaphore);    
    }
  }
}

static  void LedTaskStart (ULONG thread_input)
{
  (void)thread_input;

  UINT i = 0;
  while (1)
  {
//    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    tx_thread_sleep(500);

//    App_Printf("hello world %d\r\n", i++);
    App_Printf("hello world hello world hello world hello world hello world hello world hello world %d\r\n", i++);
  }
}




/*
*********************************************************************************************************
* 函 数 名: AppTaskMsgPro
* 功能说明: 消息处理，这里用作信号量获取。
* 形 参: thread_input 是在创建该任务时传递的形参
* 返 回 值: 无
优 先 级: 3
*********************************************************************************************************
*/
static void AppTaskMsgPro(ULONG thread_input)
{
  UINT status;

  printf("semaphore task start \r\n");
  
  tx_semaphore_create(&Semaphore, "Semaphore", 0); /* 创建信号量，初始值为 0，用于信号同步 */  
  
  while(1)
  {
    status = tx_semaphore_get(&Semaphore, TX_WAIT_FOREVER);

    if(status == TX_SUCCESS)
    {
      /* 接收到信号量 */
      printf("get semaphore \r\n");
    }
  }
}



static  TX_THREAD   AppTaskStartTCB;
static  uint64_t    AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 8];

static  TX_THREAD   LedTaskStartTCB;
static  uint64_t    LedTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 8];

static  TX_THREAD   SemaphoreTaskStartTCB;
static  uint64_t    SemaphoreTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 8];

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

  tx_thread_create(&AppTaskStartTCB,              /* 任务控制块地址 */
                   "App Task Start",              /* 任务名 */
                   AppTaskStart,                  /* 启动任务函数地址 */
                   0,                             /* 传递给任务的参数 */
                   &AppTaskStartStk[0],            /* 堆栈基地址 */
                   APP_CFG_TASK_START_STK_SIZE,    /* 堆栈空间大小 */
                   APP_CFG_TASK_START_PRIO,        /* 任务优先级*/
                   APP_CFG_TASK_START_PRIO,        /* 任务抢占阀值 */
                   TX_NO_TIME_SLICE,               /* 不开启时间片 */
                   TX_AUTO_START);                 /* 创建后立即启动 */

  tx_thread_create(&LedTaskStartTCB,              /* 任务控制块地址 */
                   "Led Task Start",              /* 任务名 */
                   LedTaskStart,                  /* 启动任务函数地址 */
                   0,                             /* 传递给任务的参数 */
                   &LedTaskStartStk[0],            /* 堆栈基地址 */
                   APP_CFG_TASK_START_STK_SIZE,    /* 堆栈空间大小 */
                   LED_CFG_TASK_PRIO,              /* 任务优先级*/
                   LED_CFG_TASK_PRIO,              /* 任务抢占阀值 */
                   TX_NO_TIME_SLICE,               /* 不开启时间片 */
                   TX_AUTO_START);                 /* 创建后立即启动 */

  tx_thread_create(&SemaphoreTaskStartTCB,              /* 任务控制块地址 */
                   "Semaphore Task Start",              /* 任务名 */
                   AppTaskMsgPro,                  /* 启动任务函数地址 */
                   0,                             /* 传递给任务的参数 */
                   &SemaphoreTaskStartStk[0],            /* 堆栈基地址 */
                   APP_CFG_TASK_START_STK_SIZE,    /* 堆栈空间大小 */
                   SEMP_CFG_TASK_PRIO,              /* 任务优先级*/
                   SEMP_CFG_TASK_PRIO,              /* 任务抢占阀值 */
                   TX_NO_TIME_SLICE,               /* 不开启时间片 */
                   TX_AUTO_START);                 /* 创建后立即启动 */

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
