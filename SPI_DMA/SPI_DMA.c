//
// Created by С�� on 2024/2/15.
//

#include "SPI_DMA.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

void SPI1_DMA_init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num){
    /************************���ղ���************************/
    // ʹ�� DMA SPI ����|����
    SET_BIT(hspi1.Instance->CR2, SPI_CR2_TXDMAEN);
    SET_BIT(hspi1.Instance->CR2, SPI_CR2_RXDMAEN);

    // ʹ�ܽ����ж�
    __HAL_SPI_ENABLE(&hspi1);

    // ʧЧDMA����
    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    while(hdma_spi1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }

    // ������ձ�־
    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_rx, DMA_LISR_TCIF2);

    // �洢 STM32F4 ����ĵ�ַ ��ʹ�ô��� 1����ô�üĴ�������д�� 0x40011004(��ʵ����&USART1_DR)
    hdma_spi1_rx.Instance->PAR = (uint32_t) & (SPI1->DR);

    // �ڴ滺����1
    hdma_spi1_rx.Instance->M0AR = (uint32_t)(rx_buf);

    // �������ݳ���
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, num);

    // ʹ�ܿ����ж�
    __HAL_DMA_ENABLE_IT(&hdma_spi1_rx, DMA_IT_TC);

    /************************���Ͳ���************************/
    // ʧЧDMA����
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    while(hdma_spi1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }

    // ������ͱ�־
    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_LISR_TCIF3);

    // ���������ַ
    hdma_spi1_tx.Instance->PAR = (uint32_t) & (SPI1->DR);

    //�ڴ滺����1
    hdma_spi1_tx.Instance->M0AR = (uint32_t)(tx_buf);

    // �������ݳ���
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, num);
}


void SPI1_DMA_enable(uint32_t tx_buf, uint32_t rx_buf, uint16_t ndtr){
    // ʧ��DMA
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

    // ������н��ձ�־
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi1.hdmarx));
    // ������з��ͱ�־
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi1.hdmatx));

    // �����ڴ滺����
    hdma_spi1_rx.Instance->M0AR = rx_buf;
    hdma_spi1_tx.Instance->M0AR = tx_buf;

    // �������ݳ���
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, ndtr);
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, ndtr);

    // ʹ��DMA
    __HAL_DMA_ENABLE(&hdma_spi1_rx);
    __HAL_DMA_ENABLE(&hdma_spi1_tx);
}