#pragma once
#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 必须在启动调度器之前（或任何任务使用 uORB 之前）调用一次 */
void bsp_uorb_init(void);

void vUorbTask1(void *parameter);
void vUorbTask2(void *parameter);

#ifdef __cplusplus
}
#endif