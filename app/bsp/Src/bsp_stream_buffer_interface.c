#include "bsp_stream_buffer_interface.h"

static StreamBufferHandle_t stream_buffer = NULL;

StreamBufferHandle_t bsp_get_stream_buffer(void)
{
    return stream_buffer;
}

void bsp_stream_buffer_init(void)
{
    stream_buffer = xStreamBufferCreate(128, 1);
}