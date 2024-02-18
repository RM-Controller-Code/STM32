//
// Created by Ð¡ÐÂ on 2024/1/19.
//

#ifndef STM32_DEMO_DIRECT_MEMORY_ACCESS_H
#define STM32_DEMO_DIRECT_MEMORY_ACCESS_H

#include "stdio.h"
#include "stdarg.h"
#include "usart.h"

void RC_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
void RC_restart(uint16_t dma_buf_num);                                  //ÖØÆô

#endif //STM32_DEMO_DIRECT_MEMORY_ACCESS_H
