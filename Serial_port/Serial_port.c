//
// Created by 小新 on 2024/1/21.
//

#include "Serial_port.h"

void Serial(char data[],uint32_t RGB){
    // 颜色读取
    uint8_t R, G, B;
    R = (RGB & 0xF00) >> 8;
    G = (RGB & 0x0F0) >> 4;
    B = (RGB & 0x00F) >> 0;

    // 串口通信部分
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
        // 接到信号停止1s 再重新发送信号,避免卡死
        HAL_Delay(1000);
        HAL_UART_Transmit(&huart1,data, sizeof(data),100);
    } else{
        htim5.Instance->CCR1=0;
        htim5.Instance->CCR2=0;
        htim5.Instance->CCR3=0;
        // 未接到信号则一直发送信号
        HAL_UART_Transmit(&huart1,data, sizeof(data),100);
    }
}