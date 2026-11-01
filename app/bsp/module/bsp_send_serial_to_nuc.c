#include "main.h"
#include "FreeRTOS.h"
#include "register_task.h"
#include "bsp_base_param.h"
#include "bsp_event_group_interface.h"

void vSendSerialToNucTask(void *parameter)
{
  EventGroupHandle_t xEventGroup = bsp_get_event_group();
  EventBits_t uxBits;
  for (;;)
  {
    uxBits = xEventGroupWaitBits(xEventGroup, UART_SEND_LIQUID, pdTRUE, pdFALSE, portMAX_DELAY);
    if ((uxBits & UART_SEND_LIQUID) != 0)
    {
      ConfigParam param = bsp_get_param();
      if (param.can_be_used == 1)
      {
        uint16_t current_val = get_liquid_value();
        uint16_t base = param.liquid_level_base;
        uint16_t max = param.liquid_level_max;
        
        int16_t value = (int16_t)current_val - (int16_t)base;
        uint8_t percentage = 0;
        
        if (max > base) {
            if (current_val <= base) percentage = 0;
            else if (current_val >= max) percentage = 100;
            else percentage = (uint8_t)(((uint32_t)(current_val - base) * 100) / (max - base));
        }

        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = 0x55;
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = 0x01;
        
        // Base value
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = (base >> 8) & 0xff;
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = base & 0xff;
        
        // Max value
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = (max >> 8) & 0xff;
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = max & 0xff;
        
        // Current value (relative to base)
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = (value >> 8) & 0xff;
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = value & 0xff;
        
        // Percentage
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = percentage;
        
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = '\r';
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF));
        USART1->TDR = '\n';
        while (!(USART1->ISR & USART_ISR_TC));
      }
    }
  }
}

// RTOS_TASK_REGISTER("SendSerialToNucTask",vSendSerialToNucTask, 156, 2);