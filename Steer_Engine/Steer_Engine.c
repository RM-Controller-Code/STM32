#include "Steer_Engine.h"


void Steer_init(){
    // �����λ
    if(Init == 0){
        htim1.Instance->CCR1 = 530;
        HAL_Delay(2000);
        Init = 1;
    }
}

// ����ƶ�
void Steer_move(uint16_t angle){
    htim1.Instance->CCR1 = angle;
}