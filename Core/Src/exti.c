
#include "main.h"
#include "app_azure_rtos.h"
#include <stdio.h>


extern TX_SEMAPHORE Semaphore;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
    case GPIO_PIN_3:
      HAL_Delay(100);
      tx_semaphore_put(&Semaphore);
      break;
  }
}

