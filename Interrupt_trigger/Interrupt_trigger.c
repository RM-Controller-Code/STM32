#include "Interrupt_trigger.h"

static GPIO_PinState Read_PIN;

void Interrupt(uint32_t RGB,uint32_t new_RGB){
    double R, G, B;
    double nR, nG, nB;
    // ��ȡaRGB����
    R = (RGB & 0xF00) >> 8;
    G = (RGB & 0x0F0) >> 4;
    B = (RGB & 0x00F) >> 0;
    nR = (new_RGB & 0xF00) >> 8;
    nG = (new_RGB & 0x0F0) >> 4;
    nB = (new_RGB & 0x00F) >> 0;
    // ��ʼ��
    htim5.Instance->CCR1 = 0;
    htim5.Instance->CCR2 = 0;
    htim5.Instance->CCR3 = 0;

    // ��ȡ��ƽ
    Read_PIN = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
    //�����ı���
    if(Read_PIN == GPIO_PIN_SET){
        if(R == 1) htim5.Instance->CCR3 = 300;
        if(G == 1) htim5.Instance->CCR2 = 300;
        if(B == 1) htim5.Instance->CCR1 = 300;
    }
    //�½��ı���
    else if(Read_PIN == GPIO_PIN_RESET){
        if(nR == 1) htim5.Instance->CCR3 = 300;
        if(nG == 1) htim5.Instance->CCR2 = 300;
        if(nB == 1) htim5.Instance->CCR1 = 300;
    }

}
