#include <stdint.h>
#include"register_task.h"

void vGetAllStacksTask(void *parameter)
{
  char task_buf[1024];
  for (;;)
  {
    memset(task_buf, 0, sizeof(task_buf));
    vTaskList(task_buf);
    // vTaskGetRunTimeStatistics(task_buf, sizeof(task_buf));
    CDC_Transmit_FS((uint8_t *)task_buf, strlen(task_buf));
    vTaskDelay(1000);
    
  }
}

// RTOS_TASK_REGISTER(vGetAllStacksTask, "GetAllStacks", 512, 3);