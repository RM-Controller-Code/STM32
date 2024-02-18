//
// Created by 小新 on 2024/2/18.
//

#ifndef STM32_FREERTOS_USB_TASK_H
#define STM32_FREERTOS_USB_TASK_H

#include <usb_device.h>
#include "cmsis_os.h"
#include "Protocol.h"

extern void usb_task(void const * argument);
extern void rm_dequeue_send_data(void* buf,uint16_t len);    // 从队列中取出数据进行序列化

#endif //STM32_FREERTOS_USB_TASK_H
