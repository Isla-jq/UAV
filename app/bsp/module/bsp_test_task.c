#include <stdint.h>
#include "main.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "register_task.h"

void vtask1(void *parameter)
{
  StreamBufferHandle_t stream_buffer = bsp_get_stream_buffer();
  for (;;)
  {
    uint8_t ch = 2;
    if (stream_buffer != NULL)
    {
      xStreamBufferReceive(stream_buffer, &ch, 1, portMAX_DELAY);
      USART1->TDR = ch;
    }
    else
    {
      vTaskDelay(1);
    }
    vTaskDelay(1);
  }
}

// RTOS_TASK_REGISTER("task1", vtask1, 128, 1);