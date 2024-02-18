//
// Created by 小新 on 2024/2/18.
//

#include "USB_Task.h"

extern QueueHandle_t CDC_send_queue;    // 数据队列
static uint8_t usb_buf[128];            // USB缓存数据

_Noreturn void usb_task(void const * argument){
    // USB初始化
    MX_USB_DEVICE_Init();
    while (1){
        /**
         * BaseType_t xQueueReceive(QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait);
         * xQueue: 是要接收消息的队列的句柄(handle)
         * pvBuffer: 是一个指向用于存储接收到的消息数据的缓冲区的指针
         * xTicksToWait: 是等待消息的最大时间,如果队列为空，任务将在等待指定的时间内阻塞，直到有消息可用
         * 函数返回值是 pdPASS 表示成功接收消息，而 errQUEUE_EMPTY 表示在超时前未接收到消息(队列为空且超时)
         */
        //函数返回值是 pdPASS 表示成功接收消息，而 errQUEUE_EMPTY 表示在超时前未接收到消息（队列为空且超时）。
        if(xQueueReceive(CDC_send_queue, usb_buf, 10 ) == pdTRUE){
            // 发送数据
            htim5.Instance->CCR2 = 100;
            rm_dequeue_send_data(usb_buf,128);
        }
        osDelay(2);
    }
}

// Test
extern void rm_queue_data(uint16_t cmd_id,void* buf,uint16_t len);                     // 添加数据进入队列
void green_led_task(void const * argument){
    while (1){
        vision_t vision_data;
        vision_data.id = 7;
        vision_data.mode = 0x21;
        vision_data.yaw = 10;
        vision_data.pitch = 15;
        vision_data.roll = 0;
        vision_data.shoot_speed = 0;
        for (int i = 0; i < 4; ++i) {
            vision_data.quaternion[i] = 1;
        }
        rm_queue_data(VISION_ID,&vision_data, sizeof(vision_t));
    }
}