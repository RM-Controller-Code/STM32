#include "Steer_Engine.h"


void Steer_init(){
    // 舵机复位
    if(Init == 0){
        htim1.Instance->CCR1 = 530;
        HAL_Delay(2000);
        Init = 1;
    }
}

// 舵机移动
void Steer_move(uint16_t angle){
    htim1.Instance->CCR1 = angle;
}