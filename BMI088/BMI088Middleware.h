#ifndef BMI088MIDDLEWARE_H
#define BMI088MIDDLEWARE_H

#include "main.h"
#include "stdio.h"
#include "BMI088reg.h"
// 引脚、通信接口初始化
extern void BMI088_GPIO_init(void);
extern void BMI088_com_init(void);

// 延迟函数
extern void BMI088_delay_ms(uint16_t ms);
extern void BMI088_delay_us(uint16_t us);

//复位函数
extern void BMI088_ACCEL_NS_L(void);
extern void BMI088_ACCEL_NS_H(void);
extern void BMI088_GYRO_NS_L(void);
extern void BMI088_GYRO_NS_H(void);

// BMI088通信函数
extern uint8_t BMI088_read_write_byte(uint8_t tx_data);
extern void BMI088_read_single_reg(uint8_t reg, uint8_t *data);
extern void BMI088_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len);   // 多字节读取
extern void BMI088_write_single_reg(uint8_t reg, uint8_t data);

// 陀螺仪部分
void BMI088_gyro_read_single_reg(uint8_t reg, uint8_t *data);
void BMI088_gyro_write_single_reg(uint8_t reg, uint8_t data);
void BMI088_gyro_read_muli_reg(uint8_t reg, uint8_t *data, uint8_t len);
// 加速度计部分
void BMI088_accel_read_single_reg(uint8_t reg, uint8_t *data);
void BMI088_accel_write_single_reg(uint8_t reg, uint8_t data);
void BMI088_accel_read_muli_reg(uint8_t reg, uint8_t *data, uint8_t len);
#endif //BMI088MIDDLEWARE_H