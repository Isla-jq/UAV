#include "adc.h"
#include "bsp_liquid_read.h"
#include "bsp_base_param.h"
#include "bsp_event_group_interface.h"

static uint16_t liquid_value = 0;

void vReadLiquidTask(void *parameter)
{
    EventGroupHandle_t xEventGroup = bsp_get_event_group();
    EventBits_t uxBits = 0;
    for (;;)
    {

        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
        {
            liquid_value = HAL_ADC_GetValue(&hadc1);
        }
        xEventGroupSetBits(xEventGroup, UART_SEND_LIQUID);
        HAL_Delay(1000);
    }
}

uint16_t get_liquid_value()
{
    return liquid_value;
}