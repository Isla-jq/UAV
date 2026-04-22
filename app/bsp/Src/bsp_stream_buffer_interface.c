#include "main.h"
#include "bsp_stream_buffer_interface.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"

static StreamBufferHandle_t stream_buffer = NULL;