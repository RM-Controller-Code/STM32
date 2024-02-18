#include "BMI088driver.h"

float BMI088_ACCEL_SEN = BMI088_ACCEL_3G_SEN;
float BMI088_GYRO_SEN = BMI088_GYRO_2000_SEN;

uint8_t BMI088_init(){
    uint8_t error = BMI088_NO_ERROR;
    // GPIO and SPI Init
    BMI088_GPIO_init();
    BMI088_com_init();

    // 加速度计自检
    if(BMI088_accel_self_test() != BMI088_NO_ERROR){
        error |= BMI088_SELF_TEST_ACCEL_ERROR;
    } else{
        error |= BMI088_accel_init();
    }

    // 陀螺仪自检
    if(BMI088_gyro_self_test() != BMI088_NO_ERROR){
        error |= BMI088_SELF_TEST_GYRO_ERROR;
    } else{
        error |= BMI088_gyro_init();
    }

//    if(error == BMI088_NO_ERROR){
//        htim5.Instance->CCR1 = 0;
//        htim5.Instance->CCR2 = 100;
//        htim5.Instance->CCR3 = 0;
//    }

    return error;
}

uint8_t BMI088_accel_init() {
    uint8_t write_reg_num;
    uint8_t res = 0;

    // Reset the accel sensor
    BMI088_accel_write_single_reg(BMI088_ACC_SOFTRESET,BMI088_ACC_SOFTRESET_VALUE);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // open accel
    BMI088_accel_write_single_reg(BMI088_ACC_PWR_CTRL,BMI088_ACC_ENABLE_ACC_ON);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // Check commiunication is normal after reset
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID,&res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // Check the "who am I"
    if (res != BMI088_ACC_CHIP_ID_VALUE){
        return BMI088_NO_SENSOR;
    }

    // Set accel sensor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++){
        // 初始化寄存器
        BMI088_accel_write_single_reg(Write_BMI088_accel[write_reg_num][0],Write_BMI088_accel[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 读取寄存器状态
        BMI088_accel_read_single_reg(Write_BMI088_accel[write_reg_num][0],&res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 判读寄存器状态
        if (res != Write_BMI088_accel[write_reg_num][1])
        {
            return Write_BMI088_accel[write_reg_num][2];
        }
    }

    return BMI088_NO_ERROR;
}

uint8_t BMI088_accel_self_test() {
    uint8_t res = 0;
    uint8_t write_reg_num;
    uint8_t buf[6] = {0, 0, 0, 0, 0, 0};
    int16_t self_test_accel[2][3];          // 自检加速度计数据

    // Reset the accel sensor
    BMI088_accel_write_single_reg(BMI088_ACC_SOFTRESET,BMI088_ACC_SOFTRESET_VALUE);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // open accel
    BMI088_accel_write_single_reg(BMI088_ACC_PWR_CTRL,BMI088_ACC_ENABLE_ACC_ON);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // Check commiunication is normal after reset
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID,&res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // Check the "who am I"
    if (res != BMI088_ACC_CHIP_ID_VALUE){
        return BMI088_NO_SENSOR;
    }

    // 设置自检模式下的寄存器
    for (write_reg_num = 0; write_reg_num < 4; write_reg_num++){
        // 写入寄存器
        BMI088_accel_write_single_reg(Write_BMI088_ACCEL_self_test[write_reg_num][0],Write_BMI088_ACCEL_self_test[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 读取寄存器状态
        BMI088_accel_read_single_reg(Write_BMI088_ACCEL_self_test[write_reg_num][0], &res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 判读寄存器状态
        if (res != Write_BMI088_ACCEL_self_test[write_reg_num][1])
        {
            return Write_BMI088_ACCEL_self_test[write_reg_num][2];
        }
        // accel conf and accel range  . the two register set need wait for > 50ms
        BMI088_delay_ms(BMI088_LONG_DELAY_TIME);
    }

    // 启动自检模式(正、负自检)
    for (write_reg_num = 0; write_reg_num < 2; write_reg_num++){
        // 启动自检
        BMI088_accel_write_single_reg(Write_BMI088_ACCEL_self_test[write_reg_num + 4][0],Write_BMI088_ACCEL_self_test[write_reg_num + 4][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 读取寄存器状态
        BMI088_accel_read_single_reg(Write_BMI088_ACCEL_self_test[write_reg_num + 4][0], &res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 判读寄存器状态
        if (res != Write_BMI088_ACCEL_self_test[write_reg_num + 4][1])
        {
            return Write_BMI088_ACCEL_self_test[write_reg_num + 4][2];
        }
        BMI088_delay_ms(BMI088_LONG_DELAY_TIME);

        // 读取加速度计数据
        BMI088_accel_read_muli_reg(BMI088_ACCEL_XOUT_L,buf,6);
        self_test_accel[write_reg_num][0] = (int16_t)((buf[1]) << 8) | buf[0];
        self_test_accel[write_reg_num][1] = (int16_t)((buf[3]) << 8) | buf[2];
        self_test_accel[write_reg_num][2] = (int16_t)((buf[5]) << 8) | buf[4];
    }
    // 关闭自检
    BMI088_accel_write_single_reg(BMI088_ACC_SELF_TEST,BMI088_ACC_SELF_TEST_OFF);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // 判读自检是否关闭
    BMI088_accel_read_single_reg(BMI088_ACC_SELF_TEST, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    if (res != BMI088_ACC_SELF_TEST_OFF)
    {
        return BMI088_ACC_SELF_TEST_ERROR;
    }

    // 判断自检是否正常
    if ((self_test_accel[0][0] - self_test_accel[1][0] < 1365) || (self_test_accel[0][1] - self_test_accel[1][1] < 1365) || (self_test_accel[0][2] - self_test_accel[1][2] < 680))
    {
        return BMI088_SELF_TEST_ACCEL_ERROR;
    }

    return BMI088_NO_ERROR;
}

uint8_t BMI088_gyro_init(){
    uint8_t write_reg_num;
    uint8_t res = 0;

    // Reset the gyro sensor
    BMI088_gyro_write_single_reg(BMI088_GYRO_SOFTRESET,BMI088_GYRO_SOFTRESET_VALUE);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // Check commiunication is normal after reset
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID,&res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // Check the "who am I"
    if(res != BMI088_GYRO_CHIP_ID_VALUE) {
        return BMI088_NO_SENSOR;
    }

    // Set gyro sensor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++){
        // 初始化寄存器
        BMI088_gyro_write_single_reg(Write_BMI088_gyro[write_reg_num][0],Write_BMI088_gyro[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 读取寄存器状态
        BMI088_gyro_read_single_reg(Write_BMI088_gyro[write_reg_num][0],&res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        // 判读寄存器状态
        if (res != Write_BMI088_gyro[write_reg_num][1]){
            return Write_BMI088_gyro[write_reg_num][2];
        }
    }

    return BMI088_NO_ERROR;
}

uint8_t BMI088_gyro_self_test() {
    uint8_t res = 0;
    uint8_t retry = 0;

    // Reset the gyro sensor
    BMI088_gyro_write_single_reg(BMI088_GYRO_SOFTRESET,BMI088_GYRO_SOFTRESET_VALUE);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // Check commiunication is normal after reset
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // 启动内置自检
    BMI088_gyro_write_single_reg(BMI088_GYRO_SELF_TEST,BMI088_GYRO_TRIG_BIST);
    BMI088_delay_us(BMI088_LONG_DELAY_TIME);

    // 循环自检
    do
    {
        BMI088_gyro_read_single_reg(BMI088_GYRO_SELF_TEST, &res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
        retry++;
    } while (!(res & BMI088_GYRO_BIST_RDY) && retry < 10);

    // retry达到10次,超时自检失败
    if (retry == 10){
        return BMI088_SELF_TEST_GYRO_ERROR;
    }

    // 判断自检是否成功(res为第[2]位为0表示自检成功)
    if (res & BMI088_GYRO_BIST_FAIL)
    {
        return BMI088_SELF_TEST_GYRO_ERROR;
    }

    return BMI088_NO_ERROR;
}

void BMI088_read(float gyro[3], float accel[3], float *temperate) {
    uint8_t buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t BMI088_raw_temp;

    //加速度计数据读取
    BMI088_accel_read_muli_reg(BMI088_ACCEL_XOUT_L,buf,6);
    BMI088_raw_temp = (int16_t) ((buf[1]) << 8) | buf[0];
    accel[0] = BMI088_raw_temp * BMI088_ACCEL_SEN;
    BMI088_raw_temp = (int16_t) ((buf[3]) << 8) | buf[2];
    accel[1] = BMI088_raw_temp * BMI088_ACCEL_SEN;
    BMI088_raw_temp = (int16_t) ((buf[5]) << 8) | buf[4];
    accel[2] = BMI088_raw_temp * BMI088_ACCEL_SEN;

    //陀螺仪数据读取
    BMI088_gyro_read_muli_reg(BMI088_GYRO_CHIP_ID, buf, 8);
    if (buf[0] == BMI088_GYRO_CHIP_ID_VALUE){
        BMI088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2];
        gyro[0] = BMI088_raw_temp * BMI088_GYRO_SEN;
        BMI088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
        gyro[1] = BMI088_raw_temp * BMI088_GYRO_SEN;
        BMI088_raw_temp = (int16_t)((buf[7]) << 8) | buf[6];
        gyro[2] = BMI088_raw_temp * BMI088_GYRO_SEN;
    }

    //温度数据读取
    BMI088_accel_read_muli_reg(BMI088_TEMP_M,buf,2);
    BMI088_raw_temp = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
    if (BMI088_raw_temp > 1023)
    {
        BMI088_raw_temp -= 2048;
    }
    *temperate = BMI088_raw_temp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;

}

void BMI088_gyro_read_over(uint8_t *rx_buf, float gyro[3]){
    int16_t BMI088_raw_temp;
    BMI088_raw_temp = (int16_t)((rx_buf[1]) << 8) | rx_buf[0];
    gyro[0] = BMI088_raw_temp * BMI088_GYRO_SEN;
    BMI088_raw_temp = (int16_t)((rx_buf[3]) << 8) | rx_buf[2];
    gyro[1] = BMI088_raw_temp * BMI088_GYRO_SEN;
    BMI088_raw_temp = (int16_t)((rx_buf[5]) << 8) | rx_buf[4];
    gyro[2] = BMI088_raw_temp * BMI088_GYRO_SEN;
}

void BMI088_accel_read_over(uint8_t *rx_buf,float accel[3],float *time){
    int16_t BMI088_raw_temp;
    uint32_t sensor_time;
    BMI088_raw_temp = (int16_t)((rx_buf[1]) << 8) | rx_buf[0];
    accel[0] = BMI088_raw_temp * BMI088_ACCEL_SEN;
    BMI088_raw_temp = (int16_t)((rx_buf[3]) << 8) | rx_buf[2];
    accel[1] = BMI088_raw_temp * BMI088_ACCEL_SEN;
    BMI088_raw_temp = (int16_t)((rx_buf[5]) << 8) | rx_buf[4];
    accel[2] = BMI088_raw_temp * BMI088_ACCEL_SEN;
    sensor_time = (uint32_t)((rx_buf[8] << 16) | (rx_buf[7] << 8) | rx_buf[6]);
    *time = sensor_time * 39.0625f;
}

void BMI088_temperature_read_over(uint8_t *rx_buf, float *temperate){
    int16_t BMI088_raw_temp;
    BMI088_raw_temp = (int16_t)((rx_buf[0] << 3) | (rx_buf[1] >> 5));

    if (BMI088_raw_temp > 1023)
    {
        BMI088_raw_temp -= 2048;
    }
    *temperate = BMI088_raw_temp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}