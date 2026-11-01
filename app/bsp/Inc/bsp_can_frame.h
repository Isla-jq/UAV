#pragma once 
#include "main.h"

typedef struct 
{
    FDCAN_RxHeaderTypeDef can_head;
    uint8_t data[8];
} can_receive_frame;


