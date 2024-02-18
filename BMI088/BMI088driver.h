#ifndef BMI088DRIVER_H
#define BMI088DRIVER_H

#include "BMI088Middleware.h"
#include "BMI088reg.h"
#include "stdio.h"
#include "tim.h"

typedef struct BMI088_REAL_DATA
{
    uint8_t status;
    float accel[3];     // 加速度计
    float gyro[3];      // 陀螺仪
    float temp;         // 温度
    float time;
} bmi088_real_data_t;

// BMI088错误码
enum
{
    BMI088_NO_ERROR                     = 0x00, // 无错误
    // 加速度计
    BMI088_ACC_PWR_CTRL_ERROR           = 0x01, // 加速度计开关
    BMI088_ACC_PWR_CONF_ERROR           = 0x02, // 电源模式
    BMI088_ACC_CONF_ERROR               = 0x03, // 配置寄存器
    BMI088_ACC_SELF_TEST_ERROR          = 0x04, // 加速度计自检开关
    BMI088_ACC_RANGE_ERROR              = 0x05, // 加速度计范围设置寄存器
    BMI088_INT1_IO_CTRL_ERROR           = 0x06, // 配置输入/输出引脚INT1
    BMI088_INT_MAP_DATA_ERROR           = 0x07, // 数据就绪中断引脚
    // 陀螺仪
    BMI088_GYRO_RANGE_ERROR             = 0x08, // 陀螺仪范围设置寄存器
    BMI088_GYRO_BANDWIDTH_ERROR         = 0x09, // 陀螺仪带宽
    BMI088_GYRO_LPM1_ERROR              = 0x0A, // 电源模式
    BMI088_GYRO_CTRL_ERROR              = 0x0B, // 中断模式
    BMI088_GYRO_INT3_INT4_IO_CONF_ERROR = 0x0C, // 中断引脚电气和逻辑属性
    BMI088_GYRO_INT3_INT4_IO_MAP_ERROR  = 0x0D, // 数据就绪中断引脚
    // 自检
    BMI088_SELF_TEST_ACCEL_ERROR        = 0x80, // 加速度计自检
    BMI088_SELF_TEST_GYRO_ERROR         = 0x40, // 陀螺仪自检
    BMI088_NO_SENSOR                    = 0xFF, // 无传感器
};

#define BMI088_LONG_DELAY_TIME 80           // 长延迟时间
#define BMI088_COM_WAIT_SENSOR_TIME 150     // 重置设备时间

// 加速度计自检寄存器初始化列表(参数与正常启动不一样)
static const uint8_t Write_BMI088_ACCEL_self_test[6][3] =
        {
                {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
                {BMI088_ACC_CONF, BMI088_ACC_NORMAL | BMI088_ACC_1600_HZ | BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
                {BMI088_ACC_RANGE, BMI088_ACC_RANGE_24G, BMI088_ACC_RANGE_ERROR},
                {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
                {BMI088_ACC_SELF_TEST, BMI088_ACC_SELF_TEST_POSITIVE_SIGNAL, BMI088_ACC_PWR_CONF_ERROR},
                {BMI088_ACC_SELF_TEST, BMI088_ACC_SELF_TEST_NEGATIVE_SIGNAL, BMI088_ACC_PWR_CONF_ERROR}
        };

// 加速度计寄存器初始化列表
static uint8_t Write_BMI088_accel[BMI088_WRITE_ACCEL_REG_NUM][3] =
        {
            {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
            {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
            {BMI088_ACC_CONF,  BMI088_ACC_NORMAL| BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
            {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, BMI088_ACC_RANGE_ERROR},
            {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW, BMI088_INT1_IO_CTRL_ERROR},
            {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}
        };

// 陀螺仪寄存器初始化列表
static uint8_t Write_BMI088_gyro[BMI088_WRITE_GYRO_REG_NUM][3] =
        {
                {BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},
                {BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set, BMI088_GYRO_BANDWIDTH_ERROR},
                {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},
                {BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},
                {BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW, BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},
                {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}
        };


// 初始化函数
uint8_t BMI088_init();              // BMI088的初始化
uint8_t BMI088_gyro_init();         // 陀螺仪初始化
uint8_t BMI088_accel_init();        // 加速度计初始化
uint8_t BMI088_accel_self_test();   // 陀螺仪自检程序
uint8_t BMI088_gyro_self_test();    // 加速度计自检程序

// 数据处理函数
void BMI088_read(float gyro[3], float accel[3], float *temperate);          // BMI088数据的读取
void BMI088_gyro_read_over(uint8_t *rx_buf, float gyro[3]);                 // 陀螺仪数据的处理
void BMI088_accel_read_over(uint8_t *rx_buf,float accel[3],float *time);    // 加速度数据的处理
void BMI088_temperature_read_over(uint8_t *rx_buf, float *temperate);       // 温度数据的处理
#endif
