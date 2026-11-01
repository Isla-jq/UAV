#include "bsp_uorb_task.h"
#include "uORB/uORB.h"
#include "uORB/topics/actuator_armed.h"
#include "uORB/topics/adc_report.h"
#include "uORBHelper.h"
#include "drv_hrt.h"
#include "printf.h"

/*
 * 最简单的 uORB 收发 demo：
 *   vUorbTask1: orb_advertise() + orb_publish()      -> actuator_armed
 *   vUorbTask2: orb_subscribe() + orb_check() + orb_copy()
 * 两边都用 printf() 打印（printf -> printf_ -> _putchar -> USART1）。
 */

static orb_advert_t actuator_armed_pub = nullptr;
static orb_advert_t adc_report_pub = nullptr;

void bsp_uorb_init(void)
{
    orb_init();      /* 分配 orb_data / 创建信号量，必须在任务使用 uORB 之前调用 */
    orb_set_in_os(); /* 允许 uORB 内部使用临界区和信号量 */
}

/* 发布者：每秒翻转一次 armed 并发布 */
extern "C" void vUorbTask1(void *parameter)
{
    actuator_armed_s armed = {};
    armed.timestamp = hrt_absolute_time();

    actuator_armed_pub = orb_advertise(ORB_ID(actuator_armed), &armed);
    adc_report_pub = orb_advertise(ORB_ID(adc_report), &armed); 
    printf("[PUB] advertise actuator_armed %s\r\n",
           (actuator_armed_pub != nullptr) ? "ok" : "failed");
    printf("[PUB] advertise adc_report %s\r\n",
           (adc_report_pub != nullptr) ? "ok" : "failed");


    for (;;)
    {
        if (actuator_armed_pub != nullptr)
        {
            armed.armed = !armed.armed;
            armed.timestamp = hrt_absolute_time();
            adc_report_s adc_report = {};
            adc_report.timestamp = hrt_absolute_time();
            adc_report.channel_value[0] = (float)armed.armed * 100.0f;

            int ret = orb_publish(ORB_ID(actuator_armed), actuator_armed_pub, &armed);
            // printf("[PUB] armed=%d ts=%llu us ret=%d\r\n",
                //    (int)armed.armed, (unsigned long long)armed.timestamp, ret);

            ret = orb_publish(ORB_ID(adc_report), adc_report_pub, &adc_report);
            // printf("[PUB] adc_report=%d ts=%llu us ret=%d\r\n",
                //    (int)adc_report.channel_value[0], (unsigned long long)adc_report.timestamp, ret);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* 订阅者：收到新数据就打印一次 */
extern "C" void vUorbTask2(void *parameter)
{
    actuator_armed_s armed = {};
    adc_report_s adc_report = {};
    unsigned int rx_count = 0;

    int sub_fd = orb_subscribe(ORB_ID(actuator_armed));
    // printf("[SUB] subscribe actuator_armed fd=%d\r\n", sub_fd);
    int adc_sub_fd = orb_subscribe(ORB_ID(adc_report));
    // printf("[SUB] subscribe adc_report fd=%d\r\n", adc_sub_fd);

    for (;;)
    {
        bool updated = false;

        if (orb_check(sub_fd, &updated) == 0 && updated)
        {
            if (orb_copy(ORB_ID(actuator_armed), sub_fd, &armed) == 0)
            {
                ++rx_count;

                printf("[SUB] #%u armed=%d ts=%llu us\r\n",
                       rx_count, (int)armed.armed, (unsigned long long)armed.timestamp);
            }
        }

        if (orb_check(adc_sub_fd, &updated) == 0 && updated)
        {
            if (orb_copy(ORB_ID(adc_report), adc_sub_fd, &adc_report) == 0)
            {
                ++rx_count;

                printf("[SUB] #%u adc_report=%d ts=%llu us\r\n",
                       rx_count, (int)adc_report.channel_value[0], (unsigned long long)adc_report.timestamp);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
