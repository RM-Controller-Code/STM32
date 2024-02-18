//
// Created by 小新 on 2024/2/4.
//

#ifndef STM32_DEMO_MOTOR_CAN_H
#define STM32_DEMO_MOTOR_CAN_H

#include "main.h"
#include "PID.h"

#define M3508_ECD 8192  // 3508电机一圈的数值

typedef struct
{
//    dji_motor_measure_t* motor_measure;
    int32_t set_speed;//设定速读
    int32_t set_ecd;//设定编码器值
    int32_t set_current;//设定电流
}track_motor_t;

typedef struct
{
    track_motor_t motor_3508[2];
    float set_position;
    float get_position;
    float max_position;
    float min_position;
}track_control_t;

// 电机数据结构体
typedef struct
{
    uint16_t ecd;               // 编码器值，一般指电机位置
    int16_t speed_rpm;          // 电机旋转速度
    int16_t given_current;      // 电机需要电流
    uint8_t temperate;          // 电机温度
    int16_t last_ecd;           // 上次运行位置

    int32_t total_ecd;          // 电机旋转的总编码器数值
    int32_t round_cnt;          // 电机旋转圈数
    uint16_t offset_ecd;        // 校准值，一般是当前位置
} dji_motor_measure_t;


extern void CAN_filter_init();
void CAN_cmd_motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
void CAN_Slove(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]);
extern void Send();
void get_motor_measure(dji_motor_measure_t *motorMeasure, const uint8_t *rxData);
#endif //STM32_DEMO_MOTOR_CAN_H
