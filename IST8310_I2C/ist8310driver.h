/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       IST8310.c/h
  * @brief      IST8310磁力计驱动函数，包括初始化函数，处理数据函数，通信读取函数
  *             本工程是将MPU6500 IIC_SLV0设置为自动读取IST8310数据，读取
  *             MPU_EXT_SENS_DATA_00保存了IST8310的Status，通过判断标志位，来更新
  *             数据。
  * @note       IST8310只支持IIC读取
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#ifndef IST8310DRIVER_H
#define IST8310DRIVER_H

#include "main.h"

#define IST8310_DATA_READY_BIT 2

#define IST8310_NO_ERROR 0x00   // IST8310启动无错误
#define IST8310_NO_SENSOR 0x40  // 设备未启动

typedef struct ist8310_real_data_t
{
  uint8_t status;   // 状态
  float mag[3];     // 磁场数据
} ist8310_real_data_t;

extern ist8310_real_data_t ist8310RealData;
extern uint8_t ist8310_init(void); // ist8310初始化
extern void ist8310_read_over(uint8_t *status_buf, ist8310_real_data_t *mpu6500_real_data); //ist8310读取处理函数
extern void ist8310_read_mag(float mag[3]); //ist8310读取磁场数据函数
#endif
