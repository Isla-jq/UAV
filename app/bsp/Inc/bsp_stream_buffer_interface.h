#pragma once

#include "FreeRTOS.h"
#include "stream_buffer.h"
StreamBufferHandle_t bsp_get_stream_buffer(void);
void bsp_stream_buffer_init(void);