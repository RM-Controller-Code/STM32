//
// Created by С�� on 2024/1/21.
//

#include "Serial_port.h"

void Serial(char data[],uint32_t RGB){
    // ��ɫ��ȡ
    uint8_t R, G, B;
    R = (RGB & 0xF00) >> 8;
    G = (RGB & 0x0F0) >> 4;
    B = (RGB & 0x00F) >> 0;

    // ����ͨ�Ų���
    char temp[10];
    HAL_StatusTypeDef HAL;
    HAL = HAL_UART_Receive(&huart1,temp, sizeof(data),100);
    if(HAL == HAL_OK){
        if(B == 1) htim5.Instance->CCR1=100;
        else htim5.Instance->CCR1=0;
        if(G == 1) htim5.Instance->CCR2=100;
        else htim5.Instance->CCR2=0;
        if(R == 1) htim5.Instance->CCR3=100;
        else htim5.Instance->CCR3=0;
        // �ӵ��ź�ֹͣ1s �����·����ź�,���⿨��
        HAL_Delay(1000);
        HAL_UART_Transmit(&huart1,data, sizeof(data),100);
    } else{
        htim5.Instance->CCR1=0;
        htim5.Instance->CCR2=0;
        htim5.Instance->CCR3=0;
        // δ�ӵ��ź���һֱ�����ź�
        HAL_UART_Transmit(&huart1,data, sizeof(data),100);
    }
}