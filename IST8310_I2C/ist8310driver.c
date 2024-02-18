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

#include "ist8310driver.h"
#include "ist8310driver_middleware.h"

#define MAG_SEN 0.3f                // 转换成 uT(单位)
#define IST8310_WHO_AM_I 0x00       // ist8310 who am I 寄存器地址
#define IST8310_WHO_AM_I_VALUE 0x10 // 设备 ID
#define IST8310_WRITE_REG_NUM 4     // IST8310需要设置的寄存器数目

ist8310_real_data_t ist8310RealData;
// ist8310配置信息以及错误码
static const uint8_t ist8310_write_reg_data_error[IST8310_WRITE_REG_NUM][3] =
    {
        {0x0B, 0x08, 0x01},
        {0x41, 0x09, 0x02},
        {0x42, 0xC0, 0x03},
        {0x0A, 0x0B, 0x04}};

// ist8310初始化
#include "tim.h"
uint8_t ist8310_init(void)
{
    static const uint8_t wait_time = 1;
    static const uint8_t sleepTime = 50;
    uint8_t res = 0;
    uint8_t writeNum = 0;

    // 引脚、IC2通信接口初始化
    ist8310_GPIO_init();
    ist8310_com_init();

    // 重启设备
    ist8310_RST_L();
    ist8310_delay_ms(sleepTime);
    ist8310_RST_H();
    ist8310_delay_ms(sleepTime);

    // 通过读取 WHO_AM_I寄存器验证设备ID
    res = ist8310_IIC_read_single_reg(IST8310_WHO_AM_I);
    if (res != IST8310_WHO_AM_I_VALUE)
    {
        // 设备未启动
        return IST8310_NO_SENSOR;
    }
    ist8310_delay_ms(wait_time);

    // 配置IST8310四个寄存器
    // 0x0B:中断寄存器
    // 0x41:采样次数寄存器
    // 0x42:需要配置成 0xC0
    // 0x0A:配置成 200Hz 输出频率
    for (writeNum = 0; writeNum < IST8310_WRITE_REG_NUM; writeNum++)
    {
        // 配置ist8310寄存器
        ist8310_IIC_write_single_reg(ist8310_write_reg_data_error[writeNum][0], ist8310_write_reg_data_error[writeNum][1]);
        ist8310_delay_ms(wait_time);

        // 检验ist8310寄存器
        res = ist8310_IIC_read_single_reg(ist8310_write_reg_data_error[writeNum][0]);
        ist8310_delay_ms(wait_time);
        if (res != ist8310_write_reg_data_error[writeNum][1])
        {
            return ist8310_write_reg_data_error[writeNum][2];
        }
    }

    return IST8310_NO_ERROR;
}

// 读取处理函数
// 通过判断 STAT1(0x02) 寄存器值判断有没有新的数据产生
// 这里进行磁场
void ist8310_read_over(uint8_t *status_buf, ist8310_real_data_t *ist8310_real_data)
{

    if (status_buf[0] & 0x01)
    {
        int16_t temp_ist8310_data = 0;
        ist8310_real_data->status |= 1 << IST8310_DATA_READY_BIT;

        temp_ist8310_data = (int16_t)((status_buf[2] << 8) | status_buf[1]);
        ist8310_real_data->mag[0] = MAG_SEN * temp_ist8310_data;
        temp_ist8310_data = (int16_t)((status_buf[4] << 8) | status_buf[3]);
        ist8310_real_data->mag[1] = MAG_SEN * temp_ist8310_data;
        temp_ist8310_data = (int16_t)((status_buf[6] << 8) | status_buf[5]);
        ist8310_real_data->mag[2] = MAG_SEN * temp_ist8310_data;
    }
    else
    {
        ist8310_real_data->status &= ~(1 << IST8310_DATA_READY_BIT);
    }
}

/********************
 * 通用的读取磁场数据函数
 * 主要通过I2C 读取多个字节函数，读取 DATAXL 寄存器，并将两个八位数据合并成一个 16 位整型数据，再乘以 0.3 灵敏度变成单位是 ut 的磁场强度数据
 ********************/
void ist8310_read_mag(float mag[3])
{
    uint8_t buf[6];
    int16_t temp_ist8310_data = 0;
    // DATAXL输出数据寄存器 0x03-0x08 读取6位数据
    // 0x03 X轴磁场数据低八位 0x04 X轴磁场数据高八位
    // 0x05 Y轴磁场数据低八位 0x06 Y轴磁场数据高八位
    // 0x07 Z轴磁场数据低八位 0x08 Z轴磁场数据高八位
    ist8310_IIC_read_muli_reg(0x03, buf, 6);

    temp_ist8310_data = (int16_t)((buf[1] << 8) | buf[0]);
    mag[0] = MAG_SEN * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buf[3] << 8) | buf[2]);
    mag[1] = MAG_SEN * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buf[5] << 8) | buf[4]);
    mag[2] = MAG_SEN * temp_ist8310_data;
}

// PG3中断退出(不能和imu读取同时使用,放在一起或者单独打开)
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//    if(GPIO_Pin == IST8310_DRDY_Pin){
//        ist8310_read_mag(ist8310RealData.mag);
//    }
//}