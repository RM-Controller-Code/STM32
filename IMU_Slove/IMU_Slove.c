//
// Created by С�� on 2024/2/15.
//
//! �˲����ֺ�У׼����û��д
#include "IMU_Slove.h"

extern SPI_HandleTypeDef hspi1;
volatile uint8_t gyro_update_flag = 0;      // �������������ݱ�־
volatile uint8_t accel_update_flag = 0;     // ���¼��ٶȼ����ݱ�־
volatile uint8_t imu_start_dma_flag = 0;    // ��ʼSPI��DMA�����־
bmi088_real_data_t BMI088_Real_Data;

// IMU�������ݲ���
static float INS_accel[3] = {0.0f, 0.0f, 0.0f};
static float INS_gyro[3] = {0.0f, 0.0f, 0.0f};
static float INS_mag[3] = {0.0f, 0.0f, 0.0f};
static float INS_quat[4] = {0.0f, 0.0f, 0.0f, 0.0f};
static const float timing_time = 0.001f;   //tast run time , unit s.�������е�ʱ�� ��λ s
float INS_angle[3] = {0.0f, 0.0f, 0.0f};      //euler angle, unit rad.ŷ���� ��λ rad

// ���ٶ�
float gyro_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
float gyro_offset[3];           // ��Ʈ����

// ���ٶȼ�
float accel_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
float accel_offset[3];          // ��Ʈ����

// ������
float mag_scale_factor[3][3] = {IST8310_BOARD_INSTALL_SPIN_MATRIX};
float mag_offset[3];            // ��Ʈ����



// �������շ�����
uint8_t gyro_dma_rx_buf[SPI_DMA_GYRO_LENGHT];
uint8_t gyro_dma_tx_buf[SPI_DMA_GYRO_LENGHT] = {0x02|0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
// ���ٶȼ��շ�����
uint8_t accel_dma_rx_buf[SPI_DMA_ACCEL_LENGHT];
uint8_t accel_dma_tx_buf[SPI_DMA_ACCEL_LENGHT] = {0x12|0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void IMU_Init(){
    // ����SPI1 DMA ��ʼ��
    SPI1_DMA_init((uint32_t)gyro_dma_tx_buf, (uint32_t)gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);
    imu_start_dma_flag = 1;
}

void IMU_Slove(){
    // �ȴ�SPI DMA����
    while (1){

        // ����������
        if(gyro_update_flag & IMU_UPDATE_SHFITS)
        {
            gyro_update_flag &= ~IMU_UPDATE_SHFITS;
            BMI088_gyro_read_over(gyro_dma_rx_buf + BMI088_GYRO_RX_BUF_DATA_OFFSET, BMI088_Real_Data.gyro);
        }

        // ���ٶȼ�����
        if(accel_update_flag & IMU_UPDATE_SHFITS)
        {
            accel_update_flag &= ~IMU_UPDATE_SHFITS;
            BMI088_accel_read_over(accel_dma_rx_buf + BMI088_ACCEL_RX_BUF_DATA_OFFSET, BMI088_Real_Data.accel, &BMI088_Real_Data.time);
        }

    }
}

// ����SPI��DMA���䴦��
void IMU_CMD_SPI_DMA(){

    // ���������ǵ�DMA����
    if( (gyro_update_flag & IMU_DR_SHFITS)
        && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN)
        && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN)
        && !(accel_update_flag & IMU_SPI_SHFITS) )
        // ȷ��hspi1����ʹ��״̬ && accel�����ڿɸ���״̬
    {
        // ����״̬
        gyro_update_flag &= ~IMU_DR_SHFITS;
        gyro_update_flag |= IMU_SPI_SHFITS;

        // SPIƬѡ�ź���,������ƽѡ��������
        HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);

        // ����DMA����
        SPI1_DMA_enable((uint32_t)gyro_dma_tx_buf, (uint32_t)gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);
        return;
    }

    // �������ٶȼƵ�DMA����
    if((accel_update_flag & IMU_DR_SHFITS)
        && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN)
        && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN)
        && !(gyro_update_flag & IMU_SPI_SHFITS))
        // ȷ��hspi1����ʹ��״̬ && gyro�����ڿɸ���״̬
    {
        // ����״̬
        accel_update_flag &= ~IMU_DR_SHFITS;
        accel_update_flag |= IMU_SPI_SHFITS;

        // SPIƬѡ�ź���,������ƽѡ�м��ٶȼ�
        HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);

        // ����DMA����
        SPI1_DMA_enable((uint32_t)accel_dma_tx_buf, (uint32_t)accel_dma_rx_buf, SPI_DMA_ACCEL_LENGHT);
        return;
    }
    return;
}



/**
 * λ˵��
 * BIT[0]: �����жϺ���1
 * BIT[1]: �ɹ�����SPI��DMA����
 * BIT[2]: ���SPI��DMA����
 * BIT[3:7]: ����
 */
// �жϽ��ղ���
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    // �������ݲ���
    if(GPIO_Pin == INT1_ACCEL_Pin){
        // ���ټ����ݾ���
        accel_update_flag |= IMU_DR_SHFITS;
        // ����SPI DMA���䴦��
        if(imu_start_dma_flag) IMU_CMD_SPI_DMA();
    }
    else if(GPIO_Pin == INT1_GYRO_Pin){
        // ���������ݾ���
        gyro_update_flag |= IMU_DR_SHFITS;
        // ����SPI DMA���䴦��
        if(imu_start_dma_flag) IMU_CMD_SPI_DMA();
    }
}

void DMA2_Stream2_IRQHandler(){
    // DMA��������
    if(__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)) != RESET){
        // ������ձ�־
        __HAL_DMA_CLEAR_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));

        //�����Ƕ�ȡ���
        if(gyro_update_flag & IMU_SPI_SHFITS){
            // ����״̬(���DMA����)
            gyro_update_flag &= ~IMU_SPI_SHFITS;
            gyro_update_flag |= IMU_UPDATE_SHFITS;

            // SPIƬѡ�ź���,��λ������
            HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
        }

        //���ٶȼƶ�ȡ���
        if(accel_update_flag & IMU_SPI_SHFITS){
            // ����״̬(���DMA����)
            accel_update_flag &= ~IMU_SPI_SHFITS;
            accel_update_flag |= IMU_UPDATE_SHFITS;

            // SPIƬѡ�ź���,��λ���ٶȼ�
            HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
        }

        IMU_CMD_SPI_DMA();
    }
}

/**
  * @brief          ��ת������,���ٶȼƺʹ�����,��������Ư,��Ϊ�豸�в�ͬ��װ��ʽ
  * @param[out]     gyro: ������Ư����ת
  * @param[out]     accel: ������Ư����ת
  * @param[out]     mag: ������Ư����ת
  * @param[in]      bmi088: �����Ǻͼ��ٶȼ�����
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