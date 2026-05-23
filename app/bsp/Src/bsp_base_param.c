#include "bsp_base_param.h"
#include <stdlib.h>

// sector7 0x080E0000
static ConfigParam config_temp_param;
ConfigParam * const config_param = (ConfigParam *)(CONFIG_FLASH_ADDE);

void bsp_base_param_init(void)
{
    if (config_temp_param.can_be_used != 1) {
        return -1;
    }
    config_temp_param.can_be_used = config_param->can_be_used;
    config_temp_param.liquid_level_refference = config_param->liquid_level_refference;
    return 0;
}

void bsp_set_liquid_level_refference(uint16_t liquid_level_refference)
{
    config_temp_param.can_be_used = 1;
    config_temp_param.liquid_level_refference = liquid_level_refference;
    return 0;
}

void bsp_saveconfig(void)
{
    FLASH_EraseInitTypeDef erase;
    uint32_t sector_error = 0;
    uint32_t addr = CONFIG_FLASH_ADDE;
    uint8_t *data = (uint8_t *)&config_temp_param;
    size_t data_len = sizeof(config_temp_param);

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
    HAL_FLASH_Unlock();

    erase.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase.Banks     = FLASH_BANK_1;
    erase.Sector    = FLASH_SECTOR_7;
    erase.NbSectors = 1;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&erase, &sector_error) != HAL_OK) {
        HAL_FLASH_Lock();
        return;
    }

    uint8_t flash_word_buf[32];
    size_t processed = 0;

    while (processed < data_len) {
        memset(flash_word_buf, 0xFF, 32);
        size_t remain = data_len - processed;
        size_t copy_len = (remain > 32) ? 32 : remain;
        memcpy(flash_word_buf, &data[processed], copy_len);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr, (uint32_t)flash_word_buf) != HAL_OK) {
            HAL_FLASH_Lock();
            return;
        }

        addr += 32;
        processed += 32;
    }

    HAL_FLASH_Lock();
}
ConfigParam  bsp_get_param() {
    return config_temp_param;
}