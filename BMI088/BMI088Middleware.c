#include "BMI088Middleware.h"
extern SPI_HandleTypeDef hspi1;

/**************************
 * 初始化引脚、通信接口
 **************************/
void BMI088_GPIO_init(void){}
void BMI088_com_init(void){}


/**************************
 * 延迟函数
 **************************/
void BMI088_delay_ms(uint16_t ms)
{
//    osDelay(ms);
    HAL_Delay(ms);
}

void BMI088_delay_us(uint16_t us)
{
//    delay_us(us);
    uint32_t time = us/1000;
    HAL_Delay(time);
}


/**************************
 * 复位函数
 * GPIO_PIN_RESET:低电平 GPIO_PIN_SET:高电平
 * 连接 SPI 片选信号线，低电平有效
 **************************/
void BMI088_ACCEL_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}

void BMI088_GYRO_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}


/**************************
 * 通信函数
 * tx_data: 发送数据指针 rx_data: 接受数据指针
 **************************/
uint8_t BMI088_read_write_byte(uint8_t tx_data)
{
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi1, &tx_data, &rx_data, 1, 1000);
    return rx_data;
}

void BMI088_read_single_reg(uint8_t reg, uint8_t *data){
    BMI088_read_write_byte(reg | BMI088_SPI_READ_CODE);
    *data = BMI088_read_write_byte(0x55);
}

void BMI088_write_single_reg(uint8_t reg, uint8_t data){
    BMI088_read_write_byte(reg);
    BMI088_read_write_byte(data);
}

void BMI088_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len){
    BMI088_read_write_byte(reg | BMI088_SPI_READ_CODE);
    while (len != 0)
    {
        *buf = BMI088_read_write_byte(0x55);
        buf++;
        len--;
    }
}

/**************************
 * 通信函数
 * 分为陀螺仪和加速度计
 **************************/

// 陀螺仪部分
void BMI088_gyro_read_single_reg(uint8_t reg, uint8_t *data){
    BMI088_GYRO_NS_L();
    BMI088_read_single_reg(reg,data);
    BMI088_GYRO_NS_H();
}

void BMI088_gyro_write_single_reg(uint8_t reg, uint8_t data){
    BMI088_GYRO_NS_L();
    BMI088_write_single_reg(reg,data);
    BMI088_GYRO_NS_H();
}

void BMI088_gyro_read_muli_reg(uint8_t reg, uint8_t *data, uint8_t len){
    BMI088_GYRO_NS_L();
    BMI088_read_muli_reg(reg,data,len);
    BMI088_GYRO_NS_H();
}

// 加速度计部分
void BMI088_accel_read_single_reg(uint8_t reg, uint8_t *data){
    //  加速度计的读的部分，其第二个字节是混乱的数据，所以需要用户读取两次才能得到正确的加速度计数据
    BMI088_ACCEL_NS_L();
    BMI088_read_single_reg(reg,data);
    *data = BMI088_read_write_byte(0x55);
    BMI088_ACCEL_NS_H();
}

void BMI088_accel_write_single_reg(uint8_t reg, uint8_t data){
    BMI088_ACCEL_NS_L();
    BMI088_write_single_reg(reg,data);
    BMI088_ACCEL_NS_H();
}

void BMI088_accel_read_muli_reg(uint8_t reg, uint8_t *data, uint8_t len){
    BMI088_ACCEL_NS_L();
    BMI088_read_write_byte(reg | BMI088_SPI_READ_CODE);
    BMI088_read_muli_reg(reg, data, len);
    BMI088_ACCEL_NS_H();
}