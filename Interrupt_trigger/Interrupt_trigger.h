//
// Created by 小新 on 2024/1/16.
//

#ifndef STM32_DEMO_INTERRUPT_TRIGGER_H
#define STM32_DEMO_INTERRUPT_TRIGGER_H

#include "stdio.h"
#include "gpio.h"
#include "tim.h"

void Interrupt(uint32_t RGB,uint32_t new_RGB);       // 中断切换颜色

#endif //STM32_DEMO_INTERRUPT_TRIGGER_H
