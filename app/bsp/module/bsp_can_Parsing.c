#include "main.h"
#include <stdint.h>
#include "register_task.h"
#include "FreeRTOS.h"
#include "fdcan.h"
#include "printf.h"
#include "bsp_queue.h"
#include "stdint.h"
#include "bsp_can_frame.h"
#include "string.h"
#include "ugv_can_decode.h"

void vCanParsingTask(void *parameter)
{
    QueueHandle_t can_queue = queue_get_handle("can_rx");
    can_receive_frame frame;
    struct ugv_ccu_t dst_p;
    FDCAN_TxHeaderTypeDef fd_head = {
        .IdType = FDCAN_STANDARD_ID,
        .FDFormat = FDCAN_CLASSIC_CAN,
        .Identifier = 0x120,
        .TxFrameType = FDCAN_DATA_FRAME,
        .TxEventFifoControl = FDCAN_NO_TX_EVENTS,
        .DataLength = FDCAN_DLC_BYTES_8,
    };
    for (;;)
    {
        // uint8_t data[]={0x00,0x01,2,3,4,5,6,7};
        // HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &fd_head, data);
        // printf("send end\r\n");
        // uint8_t rev[]={0x00,0x0,0,0,0,0,0,0};
        if (xQueueReceive(can_queue, &frame, pdMS_TO_TICKS(100)) == pdPASS)
        {
            // printf("%x\n\n\n",frame.can_head.Identifier);
            // for(int i=0;i<8;i++) {
            //     printf("%d ",frame.data[i]);
            // }
            if (frame.can_head.Identifier == 0x701)
            {
                ugv_scu_unpack(&dst_p, frame.data, sizeof(frame.data));
                printf("ccu_drive_mode:%d\r\n", dst_p.ccu_drive_mode);
                printf("ccu_ignition_sts:%d\r\n", dst_p.ccu_ignition_sts);
                printf("ccu_p_sts:%d\r\n", dst_p.ccu_p_sts);
                printf("ccu_steering_wheel_angle:%d\r\n", dst_p.ccu_steering_wheel_angle);
                printf("steering_wheel_direction:%d\r\n", dst_p.steering_wheel_direction);
            }
            // else
            // {
            //     // printf("timeout, no frame\r\n");
            // }
            // vTaskDelay(100);
        }
    }
}
RTOS_TASK_REGISTER("CanParsing", vCanParsingTask, 512, 3);