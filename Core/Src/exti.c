
#include "main.h"
#include "app_azure_rtos.h"
#include <stdio.h>


extern TX_SEMAPHORE Semaphore;
extern TX_EVENT_FLAGS_GROUP EventGroup;
extern TX_QUEUE MessageQueues1; 

uint8_t SendMessage[20] = {0x07, 0xff, 0x22, 0x21, 0x80, 0x04, 0x2f, 0x2c, 0x51, 0x10, 0x80, 0x04, 0x11, 0x44, 0x55, 0x66, 0x77};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
    case GPIO_PIN_3:
    //  HAL_Delay(100);
    //  tx_semaphore_put(&Semaphore);
    //  tx_event_flags_set(&EventGroup, EVENT_KEY_PRESS, TX_OR);
     tx_queue_send(&MessageQueues1, SendMessage, TX_NO_WAIT);
    break;
  }
}

