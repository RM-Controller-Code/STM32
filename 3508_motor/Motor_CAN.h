//
// Created by С�� on 2024/2/4.
//

#ifndef STM32_DEMO_MOTOR_CAN_H
#define STM32_DEMO_MOTOR_CAN_H

#include "main.h"
#include "PID.h"

#define M3508_ECD 8192  // 3508���һȦ����ֵ

typedef struct
{
//    dji_motor_measure_t* motor_measure;
    int32_t set_speed;//�趨�ٶ�
    int32_t set_ecd;//�趨������ֵ
    int32_t set_current;//�趨����
}track_motor_t;

typedef struct
{
    track_motor_t motor_3508[2];
    float set_position;
    float get_position;
    float max_position;
    float min_position;
}track_control_t;

// ������ݽṹ��
typedef struct
{
    uint16_t ecd;               // ������ֵ��һ��ָ���λ��
    int16_t speed_rpm;          // �����ת�ٶ�
    int16_t given_current;      // �����Ҫ����
    uint8_t temperate;          // ����¶�
    int16_t last_ecd;           // �ϴ�����λ��

    int32_t total_ecd;          // �����ת���ܱ�������ֵ
    int32_t round_cnt;          // �����תȦ��
    uint16_t offset_ecd;        // У׼ֵ��һ���ǵ�ǰλ��
} dji_motor_measure_t;


extern void CAN_filter_init();
void CAN_cmd_motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
void CAN_Slove(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]);
extern void Send();
void get_motor_measure(dji_motor_measure_t *motorMeasure, const uint8_t *rxData);
#endif //STM32_DEMO_MOTOR_CAN_H
