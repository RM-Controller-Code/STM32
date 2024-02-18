//
// Created by 小新 on 2024/1/3.
//

#ifndef STM32_DEMO_BREATHE_LIGHT_H
#define STM32_DEMO_BREATHE_LIGHT_H
#include "stdio.h"
#include <tim.h>

static uint32_t aRGB = 0xFF000000;
#define RGB_FLOW_COLOR_CHANGE_TIME 500  // 设置切换时间
#define PWM_MAX 500                   // PWM设置最大值(0~1000)

static void aRGB_led_show(uint32_t RGB);
void breathe_light(int Mode,uint32_t Select_Colour);

#endif //STM32_DEMO_BREATHE_LIGHT_H
