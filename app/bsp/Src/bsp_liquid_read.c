#include "adc.h"
#include "bsp_liquid_read.h"
#include "bsp_base_param.h"

static uint16_t liquid_value = 0;

void vReadLiquidTask(void *parameter) {
    for (;;)
    {      
      HAL_ADC_Start(&hadc1);
      if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
          liquid_value = HAL_ADC_GetValue(&hadc1);
      }

      HAL_Delay(1000);
    }
    
}

uint16_t get_liquid_value() {
    return liquid_value;
}