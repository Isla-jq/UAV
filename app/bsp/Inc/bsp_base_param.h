#pragma once
#include "main.h"

#define CONFIG_FLASH_ADDE 0x080E0000

typedef struct 
{
    uint8_t can_be_used;
    uint16_t liquid_level_base;
    uint16_t liquid_level_max;
} __attribute__((aligned(4))) ConfigParam;

extern ConfigParam * const config_param;

void bsp_base_param_init(void);

void bsp_set_liquid_level_base(uint16_t liquid_level_base);
void bsp_set_liquid_level_max(uint16_t liquid_level_max);

void bsp_saveconfig();

ConfigParam bsp_get_param();