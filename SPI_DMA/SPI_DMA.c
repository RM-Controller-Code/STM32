//
// Created by 小新 on 2024/2/15.
//

#include "SPI_DMA.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

void SPI1_DMA_init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num){
    /************************接收部分************************/
    // 使能 DMA SPI 接收|发送
    SET_BIT(hspi1.Instance->CR2, SPI_CR2_TXDMAEN);
    SET_BIT(hspi1.Instance->CR2, SPI_CR2_RXDMAEN);

    // 使能接受中断
    __HAL_SPI_ENABLE(&hspi1);

    // 失效DMA接收
    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    while(hdma_spi1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }

    // 清除接收标志
    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_rx, DMA_LISR_TCIF2);

    // 存储 STM32F4 外设的地址 如使用串口 1，那么该寄存器必须写入 0x40011004(其实就是&USART1_DR)
    hdma_spi1_rx.Instance->PAR = (uint32_t) & (SPI1->DR);

    // 内存缓冲区1
    hdma_spi1_rx.Instance->M0AR = (uint32_t)(rx_buf);

    // 设置数据长度
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, num);

    // 使能空闲中断
    __HAL_DMA_ENABLE_IT(&hdma_spi1_rx, DMA_IT_TC);

    /************************发送部分************************/
    // 失效DMA接收
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    while(hdma_spi1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }

    // 清除发送标志
    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_LISR_TCIF3);

    // 设置外设地址
    hdma_spi1_tx.Instance->PAR = (uint32_t) & (SPI1->DR);

    //内存缓冲区1
    hdma_spi1_tx.Instance->M0AR = (uint32_t)(tx_buf);

    // 设置数据长度
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, num);
}


void SPI1_DMA_enable(uint32_t tx_buf, uint32_t rx_buf, uint16_t ndtr){
    // 失能DMA
    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    while(hdma_spi1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }
    while(hdma_spi1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }

    // 清除所有接收标志
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi1.hdmarx));
    // 清除所有发送标志
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi1.hdmatx));

    // 设置内存缓冲区
    hdma_spi1_rx.Instance->M0AR = rx_buf;
    hdma_spi1_tx.Instance->M0AR = tx_buf;

    // 设置数据长度
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, ndtr);
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, ndtr);

    // 使能DMA
    __HAL_DMA_ENABLE(&hdma_spi1_rx);
    __HAL_DMA_ENABLE(&hdma_spi1_tx);
}