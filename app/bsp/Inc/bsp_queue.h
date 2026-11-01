#pragma once
#include "FreeRTOS.h"
#include "queue.h"

// {.name = "can_rx", .depth = 16, .itemSize = sizeof(can_receive_frame), .handle = NULL}
QueueHandle_t queue_get_handle(const char *name);
void queue_create_all(void);
