#pragma once

#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"

#define UART_SEND_LIQUID (1<<0)


void bsp_event_group_init();
EventGroupHandle_t bsp_get_event_group();