//
// Created by 小新 on 2024/2/8.
//

#ifndef STM32_DEMO_BMI088REG_H
#define STM32_DEMO_BMI088REG_H

/**************************************************/
#define BMI088_SPI_READ_CODE 0x80 // burst-read模式 (即连续读取模式,发送一次地址,然后连续读取)

#define BMI088_TEMP_FACTOR 0.125f
#define BMI088_TEMP_OFFSET 23.0f

#define BMI088_ACCEL_3G_SEN     0.0008974358974f
#define BMI088_ACCEL_6G_SEN     0.00179443359375f
#define BMI088_ACCEL_12G_SEN    0.0035888671875f
#define BMI088_ACCEL_24G_SEN    0.007177734375f

#define BMI088_GYRO_2000_SEN    0.00106526443603169529841533860381f
#define BMI088_GYRO_1000_SEN    0.00053263221801584764920766930190693f
#define BMI088_GYRO_500_SEN     0.00026631610900792382460383465095346f
#define BMI088_GYRO_250_SEN     0.00013315805450396191230191732547673f
#define BMI088_GYRO_125_SEN     0.000066579027251980956150958662738366f

/****************************  加速度计  ********************************/

// 加速度计初始化: 向[0x7E]的[ACC_SOFTRESET]寄存器写入[0xB6]进行重置
#define BMI088_ACC_SOFTRESET 0x7E
#define BMI088_ACC_SOFTRESET_VALUE 0xB6

// BMI088需要设置的寄存器数目
#define BMI088_WRITE_ACCEL_REG_NUM  6

// Who am I 设备ID验证
// 加速度计的 ID 寄存器地址为 0x00，标准 ID 值为 0x1E
#define BMI088_ACC_CHIP_ID 0x00
#define BMI088_ACC_CHIP_ID_VALUE 0x1E

// BMI088_ACCEL  加速度计数据地址
#define BMI088_ACCEL_XOUT_L 0x12
#define BMI088_ACCEL_XOUT_M 0x13
#define BMI088_ACCEL_YOUT_L 0x14
#define BMI088_ACCEL_YOUT_M 0x15
#define BMI088_ACCEL_ZOUT_L 0x16
#define BMI088_ACCEL_ZOUT_M 0x17

// BMI088_TEMP  温度数据地址
#define BMI088_TEMP_M 0x22
#define BMI088_TEMP_L 0x23

// BMI088_ACC_ERR_REG 报告传感器错误情况
#define BMI088_ACC_ERR_REG 0x02
#define BMI088_ACCEL_CONGIF_ERROR (0x1 << 0x2)      // 加速度计配置出现错误

// BMI088_ACC_STATUS 传感器状态标志
#define BMI088_ACC_STATUS 0x03
#define BMI088_ACCEL_DRDY (0x1 << 0x7)              // 加速度计数据就绪标志

// BMI088_ACC_INT_STAT_1 中断状态寄存器
#define BMI088_ACC_INT_STAT_1 0x1D
#define BMI088_ACCEL_DRDY_INTERRUPT (0x1 << 0x7)    // 数据中断状态就绪

// BMI088_ACC_CONF 加速度计配置寄存器
#define BMI088_ACC_CONF 0x40
#define BMI088_ACC_CONF_MUST_Set 0x80               // 最高位启动寄存器
#define BMI088_ACC_OSR4 (0x0 << 0x4)                // acc_bwp设置OSR4
#define BMI088_ACC_OSR2 (0x1 << 0x4)                // acc_bwp设置OSR4
#define BMI088_ACC_NORMAL (0x2 << 0x4)              // acc_bwp设置normal
#define BMI088_ACC_12_5_HZ 0x5                      // ODR: 12.5Hz
#define BMI088_ACC_25_HZ 0x6                        // ODR: 25Hz
#define BMI088_ACC_50_HZ 0x7                        // ODR: 50Hz
#define BMI088_ACC_100_HZ 0x8                       // ODR: 100Hz
#define BMI088_ACC_200_HZ 0x9                       // ODR: 200Hz
#define BMI088_ACC_400_HZ 0xA                       // ODR: 400Hz
#define BMI088_ACC_800_HZ 0xB                       // ODR: 800Hz
#define BMI088_ACC_1600_HZ 0xC                      // ODR: 1600Hz

// BMI088_ACC_RANGE 加速度计范围设置寄存器
#define BMI088_ACC_RANGE 0x41
#define BMI088_ACC_RANGE_3G 0x0                     // Range: +/- 3g
#define BMI088_ACC_RANGE_6G 0x1                     // Range: +/- 6g
#define BMI088_ACC_RANGE_12G 0x2                    // Range: +/- 12g
#define BMI088_ACC_RANGE_24G 0x3                    // Range: +/- 24g

// BMI088_INT1_IO_CTRL 配置输入/输出引脚INT1
#define BMI088_INT1_IO_CTRL 0x53
#define BMI088_ACC_INT1_IO_ENABLE (0x1 << 0x3)      // INT1输出引脚启动
#define BMI088_ACC_INT1_GPIO_PP (0x0 << 0x2)        // INT1引脚PP推挽输出
#define BMI088_ACC_INT1_GPIO_OD (0x1 << 0x2)        // INT1引脚OD开漏输出
#define BMI088_ACC_INT1_GPIO_HIGH (0x1 << 0x1)      // INT1引脚高电平
#define BMI088_ACC_INT1_GPIO_LOW (0x0 << 0x1)       // INT1引脚低电平

// BMI088_INT2_IO_CTRL 配置输入/输出引脚INT2
#define BMI088_INT2_IO_CTRL 0x54
#define BMI088_ACC_INT2_IO_ENABLE (0x1 << 0x3)      // INT2输出引脚启动
#define BMI088_ACC_INT2_GPIO_PP (0x0 << 0x2)        // INT2引脚PP推挽输出
#define BMI088_ACC_INT2_GPIO_OD (0x1 << 0x2)        // INT2引脚OD开漏输出
#define BMI088_ACC_INT2_GPIO_HIGH (0x1 << 0x1)      // INT2引脚高电平
#define BMI088_ACC_INT2_GPIO_LOW (0x0 << 0x1)       // INT2引脚低电平

// BMI088_INT_MAP_DATA 数据就绪中断引脚
#define BMI088_INT_MAP_DATA 0x58
#define BMI088_ACC_INT2_DRDY_INTERRUPT (0x1 << 0x6) // INT2数据就绪中断引脚启动
#define BMI088_ACC_INT1_DRDY_INTERRUPT (0x1 << 0x2) // INT1数据就绪中断引脚启动

// BMI088_ACC_SELF_TEST 加速度计自检寄存器
#define BMI088_ACC_SELF_TEST 0x6D
#define BMI088_ACC_SELF_TEST_OFF 0x00               // 自检关闭
#define BMI088_ACC_SELF_TEST_POSITIVE_SIGNAL 0x0D   // 正自检信号
#define BMI088_ACC_SELF_TEST_NEGATIVE_SIGNAL 0x09   // 负自检信号

// BMI088_ACC_PWR_CONF 加速度计电源模式选择
#define BMI088_ACC_PWR_CONF 0x7C
#define BMI088_ACC_PWR_SUSPEND_MODE 0x03            // 挂起模式
#define BMI088_ACC_PWR_ACTIVE_MODE 0x00             // 正常模式

// BMI088_ACC_PWR_CTRL
#define BMI088_ACC_PWR_CTRL 0x7D
#define BMI088_ACC_ENABLE_ACC_OFF 0x00              // 关闭加速度计
#define BMI088_ACC_ENABLE_ACC_ON 0x04               // 打开加速度计


/****************************   陀螺仪   *******************************/

// 陀螺仪初始化: 向[0x14]的[GYRO_SOFTRESET]寄存器写入[0xB6]进行重置
#define BMI088_GYRO_SOFTRESET 0x14
#define BMI088_GYRO_SOFTRESET_VALUE 0xB6

// BMI088需要设置的寄存器数目
#define BMI088_WRITE_GYRO_REG_NUM   6

// Who am I 设备ID验证
// 陀螺仪的 ID 寄存器地址为 0x00，标准 ID 值为 0x0F
#define BMI088_GYRO_CHIP_ID 0x00
#define BMI088_GYRO_CHIP_ID_VALUE 0x0F

// BMI088_GYRO_RATE 陀螺仪数据地址
#define BMI088_GYRO_RATE_X_L 0x02
#define BMI088_GYRO_RATE_X_M 0x03
#define BMI088_GYRO_RATE_Y_L 0x04
#define BMI088_GYRO_RATE_Y_M 0x05
#define BMI088_GYRO_RATE_Z_L 0x06
#define BMI088_GYRO_RATE_Z_M 0x07

// GYRO_RANGE 陀螺仪范围选择
#define BMI088_GYRO_RANGE 0x0F
#define BMI088_GYRO_2000 0x0   // 量程:2000 | 灵敏度:16.384 LSB/°/s
#define BMI088_GYRO_1000 0x1   // 量程:1000 | 灵敏度:32.768 LSB/°/s
#define BMI088_GYRO_500  0x2   // 量程:500 | 灵敏度:65.536 LSB/°/s
#define BMI088_GYRO_250  0x3   // 量程:250 | 灵敏度:131.072 LSB/°/s
#define BMI088_GYRO_125  0x4   // 量程:125 | 灵敏度:262.144 LSB/°/s

// BMI088_GYRO_BANDWIDTH 陀螺仪数据过滤器带宽(Filter Bandwidth)和输出数据速率(ODR)
#define BMI088_GYRO_BANDWIDTH 0x10
#define BMI088_GYRO_BANDWIDTH_MUST_Set 0x80     // 最高位启动数据低通滤波器
#define BMI088_GYRO_2000_532_HZ 0x00            // ODR: 2000Hz |  Filter Bandwidth: 532Hz
#define BMI088_GYRO_2000_230_HZ 0x01            // ODR: 2000Hz |  Filter Bandwidth: 230Hz
#define BMI088_GYRO_1000_116_HZ 0x02            // ODR: 1000Hz |  Filter Bandwidth: 116Hz
#define BMI088_GYRO_400_47_HZ 0x03              // ODR: 400Hz |  Filter Bandwidth: 47Hz
#define BMI088_GYRO_200_23_HZ 0x04              // ODR: 200Hz |  Filter Bandwidth: 23Hz
#define BMI088_GYRO_100_12_HZ 0x05              // ODR: 100Hz |  Filter Bandwidth: 12Hz
#define BMI088_GYRO_200_64_HZ 0x06              // ODR: 200Hz |  Filter Bandwidth: 64Hz
#define BMI088_GYRO_100_32_HZ 0x07              // ODR: 100Hz |  Filter Bandwidth: 32Hz

// BMI088_GYRO_LPM1 陀螺仪电源模式选择
#define BMI088_GYRO_LPM1 0x11
#define BMI088_GYRO_NORMAL_MODE 0x00            // 正常模式
#define BMI088_GYRO_SUSPEND_MODE 0x80           // 挂起模式
#define BMI088_GYRO_DEEP_SUSPEND_MODE 0x20      // 深挂起模式

// BMI088_GYRO_CTRL 陀螺仪中断模式选择
#define BMI088_GYRO_CTRL 0x15
#define BMI088_DRDY_OFF 0x00                    // 未触发数据就绪中断
#define BMI088_DRDY_ON 0x80                     // 允许在新数据上触发新数据中断

// BMI088_GYRO_INT3_INT4_IO_CONF 陀螺仪设置中断引脚的电气和逻辑属性
#define BMI088_GYRO_INT3_INT4_IO_CONF 0x16
#define BMI088_GYRO_INT3_GPIO_OD (0x1 << 0x1)           // INT3引脚OD开漏输出
#define BMI088_GYRO_INT3_GPIO_PP (0x0 << 0x1)           // INT3引脚PP推挽输出
#define BMI088_GYRO_INT3_GPIO_HIGH (0x1 << 0x0)         // INT3引脚高电平
#define BMI088_GYRO_INT3_GPIO_LOW (0x0 << 0x0)          // INT3引脚低电平
#define BMI088_GYRO_INT4_GPIO_OD (0x1 << 0x3)           // INT4引脚PP推挽输出
#define BMI088_GYRO_INT4_GPIO_PP (0x0 << 0x3)           // INT4引脚OD开漏输出
#define BMI088_GYRO_INT4_GPIO_HIGH (0x1 << 0x2)         // INT4引脚高电平
#define BMI088_GYRO_INT4_GPIO_LOW (0x0 << 0x2)          // INT4引脚低电平

// BMI088_GYRO_INT3_INT4_IO_MAP 陀螺仪数据就绪中断引脚
#define BMI088_GYRO_INT3_INT4_IO_MAP 0x18
#define BMI088_GYRO_DRDY_IO_OFF 0x00                    // 无数据就绪引脚
#define BMI088_GYRO_DRDY_IO_INT3 0x01                   // 数据就绪引脚INT3
#define BMI088_GYRO_DRDY_IO_INT4 0x80                   // 数据就绪引脚INT4
#define BMI088_GYRO_DRDY_IO_BOTH 0x81                   // 数据就绪引脚INT3 | INT4

// BMI088_GYRO_SELF_TEST 陀螺仪自检寄存器
#define BMI088_GYRO_SELF_TEST 0x3C
#define BMI088_GYRO_TRIG_BIST 0x01                      // 启动内置自检
#define BMI088_GYRO_RATE_OK   (0x1 << 0x3)              // rate_ok
#define BMI088_GYRO_BIST_FAIL (0x1 << 0x2)              // bist_fail
#define BMI088_GYRO_BIST_RDY  (0x1 << 0x1)              // bist_rdy

#endif //STM32_DEMO_BMI088REG_H
