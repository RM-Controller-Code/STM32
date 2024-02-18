//
// Created by С�� on 2024/2/15.
//

#ifndef STM32_DEMO_IMU_SLOVE_H
#define STM32_DEMO_IMU_SLOVE_H

#include "main.h"
#include "SPI_DMA.h"
#include "BMI088driver.h"
#include "tim.h"


// DMA���ݳ���
#define SPI_DMA_GYRO_LENGHT       8
#define SPI_DMA_ACCEL_LENGHT      9

// ����ƫ��λ
#define BMI088_GYRO_RX_BUF_DATA_OFFSET  1
#define BMI088_ACCEL_RX_BUF_DATA_OFFSET 2

// DMA����״̬
#define IMU_DR_SHFITS       (0x1 << 0x0)    // ���ݾ���
#define IMU_SPI_SHFITS      (0x1 << 0x1)    // �ɹ�����SPI��DMA����
#define IMU_UPDATE_SHFITS   (0x1 << 0x2)    // ���SPI��DMA����
#define IMU_NOTIFY_SHFITS   (0x1 << 0x3)    // ����λ

// BMI088��װλ����ת
#define BMI088_BOARD_INSTALL_SPIN_MATRIX    \
    {0.0f, 1.0f, 0.0f},                     \
    {-1.0f, 0.0f, 0.0f},                    \
    {0.0f, 0.0f, 1.0f}                      \

// IST8310��װλ����ת
#define IST8310_BOARD_INSTALL_SPIN_MATRIX   \
    {1.0f, 0.0f, 0.0f},                     \
    {0.0f, 1.0f, 0.0f},                     \
    {0.0f, 0.0f, 1.0f}                      \

// ŷ���ǽṹ��
typedef struct {
    float pitch[1];
    float yaw[1];
    float roll[1];
}Euler_angle;

extern void IMU_Init();
extern void IMU_CMD_SPI_DMA();
extern void IMU_Slove();

#endif //STM32_DEMO_IMU_SLOVE_H
