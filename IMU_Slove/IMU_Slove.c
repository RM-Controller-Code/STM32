//
// Created by 小新 on 2024/2/15.
//
//! 滤波部分和校准部分没有写
#include "IMU_Slove.h"

extern SPI_HandleTypeDef hspi1;
volatile uint8_t gyro_update_flag = 0;      // 更新陀螺仪数据标志
volatile uint8_t accel_update_flag = 0;     // 更新加速度计数据标志
volatile uint8_t imu_start_dma_flag = 0;    // 开始SPI的DMA传输标志
bmi088_real_data_t BMI088_Real_Data;

// IMU处理数据部分
static float INS_accel[3] = {0.0f, 0.0f, 0.0f};
static float INS_gyro[3] = {0.0f, 0.0f, 0.0f};
static float INS_mag[3] = {0.0f, 0.0f, 0.0f};
static float INS_quat[4] = {0.0f, 0.0f, 0.0f, 0.0f};
static const float timing_time = 0.001f;   //tast run time , unit s.任务运行的时间 单位 s
float INS_angle[3] = {0.0f, 0.0f, 0.0f};      //euler angle, unit rad.欧拉角 单位 rad

// 角速度
float gyro_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
float gyro_offset[3];           // 零飘数据

// 加速度计
float accel_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
float accel_offset[3];          // 零飘数据

// 磁力计
float mag_scale_factor[3][3] = {IST8310_BOARD_INSTALL_SPIN_MATRIX};
float mag_offset[3];            // 零飘数据



// 陀螺仪收发数据
uint8_t gyro_dma_rx_buf[SPI_DMA_GYRO_LENGHT];
uint8_t gyro_dma_tx_buf[SPI_DMA_GYRO_LENGHT] = {0x02|0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
// 加速度计收发数据
uint8_t accel_dma_rx_buf[SPI_DMA_ACCEL_LENGHT];
uint8_t accel_dma_tx_buf[SPI_DMA_ACCEL_LENGHT] = {0x12|0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void IMU_Init(){
    // 进行SPI1 DMA 初始化
    SPI1_DMA_init((uint32_t)gyro_dma_tx_buf, (uint32_t)gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);
    imu_start_dma_flag = 1;
}

void IMU_Slove(){
    // 等待SPI DMA传输
    while (1){

        // 陀螺仪数据
        if(gyro_update_flag & IMU_UPDATE_SHFITS)
        {
            gyro_update_flag &= ~IMU_UPDATE_SHFITS;
            BMI088_gyro_read_over(gyro_dma_rx_buf + BMI088_GYRO_RX_BUF_DATA_OFFSET, BMI088_Real_Data.gyro);
        }

        // 加速度计数据
        if(accel_update_flag & IMU_UPDATE_SHFITS)
        {
            accel_update_flag &= ~IMU_UPDATE_SHFITS;
            BMI088_accel_read_over(accel_dma_rx_buf + BMI088_ACCEL_RX_BUF_DATA_OFFSET, BMI088_Real_Data.accel, &BMI088_Real_Data.time);
        }

    }
}

// 进行SPI的DMA传输处理
void IMU_CMD_SPI_DMA(){

    // 开启陀螺仪的DMA传输
    if( (gyro_update_flag & IMU_DR_SHFITS)
        && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN)
        && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN)
        && !(accel_update_flag & IMU_SPI_SHFITS) )
        // 确保hspi1处于使能状态 && accel不处于可更新状态
    {
        // 更新状态
        gyro_update_flag &= ~IMU_DR_SHFITS;
        gyro_update_flag |= IMU_SPI_SHFITS;

        // SPI片选信号线,下拉电平选中陀螺仪
        HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);

        // 开启DMA传输
        SPI1_DMA_enable((uint32_t)gyro_dma_tx_buf, (uint32_t)gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);
        return;
    }

    // 开启加速度计的DMA传输
    if((accel_update_flag & IMU_DR_SHFITS)
        && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN)
        && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN)
        && !(gyro_update_flag & IMU_SPI_SHFITS))
        // 确保hspi1处于使能状态 && gyro不处于可更新状态
    {
        // 更新状态
        accel_update_flag &= ~IMU_DR_SHFITS;
        accel_update_flag |= IMU_SPI_SHFITS;

        // SPI片选信号线,下拉电平选中加速度计
        HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);

        // 开启DMA传输
        SPI1_DMA_enable((uint32_t)accel_dma_tx_buf, (uint32_t)accel_dma_rx_buf, SPI_DMA_ACCEL_LENGHT);
        return;
    }
    return;
}



/**
 * 位说明
 * BIT[0]: 进入中断后置1
 * BIT[1]: 成功开启SPI的DMA传输
 * BIT[2]: 完成SPI的DMA传输
 * BIT[3:7]: 保留
 */
// 中断接收部分
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    // 接收数据部分
    if(GPIO_Pin == INT1_ACCEL_Pin){
        // 加速计数据就绪
        accel_update_flag |= IMU_DR_SHFITS;
        // 进入SPI DMA传输处理
        if(imu_start_dma_flag) IMU_CMD_SPI_DMA();
    }
    else if(GPIO_Pin == INT1_GYRO_Pin){
        // 陀螺仪数据就绪
        gyro_update_flag |= IMU_DR_SHFITS;
        // 进入SPI DMA传输处理
        if(imu_start_dma_flag) IMU_CMD_SPI_DMA();
    }
}

void DMA2_Stream2_IRQHandler(){
    // DMA接收数据
    if(__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)) != RESET){
        // 清除接收标志
        __HAL_DMA_CLEAR_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));

        //陀螺仪读取完毕
        if(gyro_update_flag & IMU_SPI_SHFITS){
            // 更新状态(完成DMA传输)
            gyro_update_flag &= ~IMU_SPI_SHFITS;
            gyro_update_flag |= IMU_UPDATE_SHFITS;

            // SPI片选信号线,复位陀螺仪
            HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
        }

        //加速度计读取完毕
        if(accel_update_flag & IMU_SPI_SHFITS){
            // 更新状态(完成DMA传输)
            accel_update_flag &= ~IMU_SPI_SHFITS;
            accel_update_flag |= IMU_UPDATE_SHFITS;

            // SPI片选信号线,复位加速度计
            HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
        }

        IMU_CMD_SPI_DMA();
    }
}

/**
  * @brief          旋转陀螺仪,加速度计和磁力计,并计算零漂,因为设备有不同安装方式
  * @param[out]     gyro: 加上零漂和旋转
  * @param[out]     accel: 加上零漂和旋转
  * @param[out]     mag: 加上零漂和旋转
  * @param[in]      bmi088: 陀螺仪和加速度计数据
  * @retval         none
  */
static void imu_cali_slove(float gyro[3], float accel[3], bmi088_real_data_t *bmi088)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        gyro[i] = bmi088->gyro[0] * gyro_scale_factor[i][0] + bmi088->gyro[1] * gyro_scale_factor[i][1] + bmi088->gyro[2] * gyro_scale_factor[i][2] + gyro_offset[i];
        accel[i] = bmi088->accel[0] * accel_scale_factor[i][0] + bmi088->accel[1] * accel_scale_factor[i][1] + bmi088->accel[2] * accel_scale_factor[i][2] + accel_offset[i];
    }
}