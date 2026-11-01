#include "bsp_queue.h"
#include "stdint.h"
#include "bsp_can_frame.h"
#include "string.h"

typedef struct {
    const char *name;
    UBaseType_t depth;
    size_t    itemSize;
    QueueHandle_t handle;
} QueueRecord_t;

static QueueRecord_t s_queue[] = {
    {.name = "can_rx", .depth = 16, .itemSize = sizeof(can_receive_frame), .handle = NULL},

};

void queue_create_all(void) {
    for(int i = 0; i < sizeof(s_queue)/sizeof(QueueRecord_t); ++i) {
        s_queue[i].handle = xQueueCreate(s_queue[i].depth, s_queue[i].itemSize);
    }
}

QueueHandle_t queue_get_handle(const char *name) {
    for(int i = 0; i < sizeof(s_queue)/sizeof(QueueRecord_t); ++i) {
        if (strcmp(name,s_queue[i].name) == 0) {
            return s_queue[i].handle;
        }
    }
    return NULL;
}
