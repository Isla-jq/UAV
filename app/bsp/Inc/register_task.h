#pragma once
#include <stddef.h> 

typedef void (*task_func_t)(void *param);

typedef struct register_task
{
    const char *name_;
    task_func_t func_;
    size_t stack_size_;
    uint8_t priority_;
} task_item_t;



#define RTOS_TASK_REGISTER(name, func, stack_size, priority) \
    __attribute__((used, section(".rtos_section"))) \
    static const task_item_t _task_##func = { \
        .name_ = name, \
        .func_ = func, \
        .stack_size_ = stack_size, \
        .priority_ = priority, \
    }