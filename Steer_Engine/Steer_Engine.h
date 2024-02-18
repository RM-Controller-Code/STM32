//
// Created by 小新 on 2024/1/13.
//

#ifndef STM32_DEMO_STEER_ENGINE_H
#define STM32_DEMO_STEER_ENGINE_H

#include "stdio.h"
#include <tim.h>

static uint8_t Init = 0;
void Steer_init();  // 舵机初始化
void Steer_move(uint16_t angle);

#endif //STM32_DEMO_STEER_ENGINE_H
