//
// Created by Ð¡ÐÂ on 2024/2/15.
//

#ifndef STM32_DEMO_SPI_DMA_H
#define STM32_DEMO_SPI_DMA_H

#include "main.h"

extern void SPI1_DMA_init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num);
extern void SPI1_DMA_enable(uint32_t tx_buf, uint32_t rx_buf, uint16_t ndtr);


#endif //STM32_DEMO_SPI_DMA_H
