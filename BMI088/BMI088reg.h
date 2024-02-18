//
// Created by С�� on 2024/2/8.
//

#ifndef STM32_DEMO_BMI088REG_H
#define STM32_DEMO_BMI088REG_H

/**************************************************/
#define BMI088_SPI_READ_CODE 0x80 // burst-readģʽ (��������ȡģʽ,����һ�ε�ַ,Ȼ��������ȡ)

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

/****************************  ���ٶȼ�  ********************************/

// ���ٶȼƳ�ʼ��: ��[0x7E]��[ACC_SOFTRESET]�Ĵ���д��[0xB6]��������
#define BMI088_ACC_SOFTRESET 0x7E
#define BMI088_ACC_SOFTRESET_VALUE 0xB6

// BMI088��Ҫ���õļĴ�����Ŀ
#define BMI088_WRITE_ACCEL_REG_NUM  6

// Who am I �豸ID��֤
// ���ٶȼƵ� ID �Ĵ�����ַΪ 0x00����׼ ID ֵΪ 0x1E
#define BMI088_ACC_CHIP_ID 0x00
#define BMI088_ACC_CHIP_ID_VALUE 0x1E

// BMI088_ACCEL  ���ٶȼ����ݵ�ַ
#define BMI088_ACCEL_XOUT_L 0x12
#define BMI088_ACCEL_XOUT_M 0x13
#define BMI088_ACCEL_YOUT_L 0x14
#define BMI088_ACCEL_YOUT_M 0x15
#define BMI088_ACCEL_ZOUT_L 0x16
#define BMI088_ACCEL_ZOUT_M 0x17

// BMI088_TEMP  �¶����ݵ�ַ
#define BMI088_TEMP_M 0x22
#define BMI088_TEMP_L 0x23

// BMI088_ACC_ERR_REG ���洫�����������
#define BMI088_ACC_ERR_REG 0x02
#define BMI088_ACCEL_CONGIF_ERROR (0x1 << 0x2)      // ���ٶȼ����ó��ִ���

// BMI088_ACC_STATUS ������״̬��־
#define BMI088_ACC_STATUS 0x03
#define BMI088_ACCEL_DRDY (0x1 << 0x7)              // ���ٶȼ����ݾ�����־

// BMI088_ACC_INT_STAT_1 �ж�״̬�Ĵ���
#define BMI088_ACC_INT_STAT_1 0x1D
#define BMI088_ACCEL_DRDY_INTERRUPT (0x1 << 0x7)    // �����ж�״̬����

// BMI088_ACC_CONF ���ٶȼ����üĴ���
#define BMI088_ACC_CONF 0x40
#define BMI088_ACC_CONF_MUST_Set 0x80               // ���λ�����Ĵ���
#define BMI088_ACC_OSR4 (0x0 << 0x4)                // acc_bwp����OSR4
#define BMI088_ACC_OSR2 (0x1 << 0x4)                // acc_bwp����OSR4
#define BMI088_ACC_NORMAL (0x2 << 0x4)              // acc_bwp����normal
#define BMI088_ACC_12_5_HZ 0x5                      // ODR: 12.5Hz
#define BMI088_ACC_25_HZ 0x6                        // ODR: 25Hz
#define BMI088_ACC_50_HZ 0x7                        // ODR: 50Hz
#define BMI088_ACC_100_HZ 0x8                       // ODR: 100Hz
#define BMI088_ACC_200_HZ 0x9                       // ODR: 200Hz
#define BMI088_ACC_400_HZ 0xA                       // ODR: 400Hz
#define BMI088_ACC_800_HZ 0xB                       // ODR: 800Hz
#define BMI088_ACC_1600_HZ 0xC                      // ODR: 1600Hz

// BMI088_ACC_RANGE ���ٶȼƷ�Χ���üĴ���
#define BMI088_ACC_RANGE 0x41
#define BMI088_ACC_RANGE_3G 0x0                     // Range: +/- 3g
#define BMI088_ACC_RANGE_6G 0x1                     // Range: +/- 6g
#define BMI088_ACC_RANGE_12G 0x2                    // Range: +/- 12g
#define BMI088_ACC_RANGE_24G 0x3                    // Range: +/- 24g

// BMI088_INT1_IO_CTRL ��������/�������INT1
#define BMI088_INT1_IO_CTRL 0x53
#define BMI088_ACC_INT1_IO_ENABLE (0x1 << 0x3)      // INT1�����������
#define BMI088_ACC_INT1_GPIO_PP (0x0 << 0x2)        // INT1����PP�������
#define BMI088_ACC_INT1_GPIO_OD (0x1 << 0x2)        // INT1����OD��©���
#define BMI088_ACC_INT1_GPIO_HIGH (0x1 << 0x1)      // INT1���Ÿߵ�ƽ
#define BMI088_ACC_INT1_GPIO_LOW (0x0 << 0x1)       // INT1���ŵ͵�ƽ

// BMI088_INT2_IO_CTRL ��������/�������INT2
#define BMI088_INT2_IO_CTRL 0x54
#define BMI088_ACC_INT2_IO_ENABLE (0x1 << 0x3)      // INT2�����������
#define BMI088_ACC_INT2_GPIO_PP (0x0 << 0x2)        // INT2����PP�������
#define BMI088_ACC_INT2_GPIO_OD (0x1 << 0x2)        // INT2����OD��©���
#define BMI088_ACC_INT2_GPIO_HIGH (0x1 << 0x1)      // INT2���Ÿߵ�ƽ
#define BMI088_ACC_INT2_GPIO_LOW (0x0 << 0x1)       // INT2���ŵ͵�ƽ

// BMI088_INT_MAP_DATA ���ݾ����ж�����
#define BMI088_INT_MAP_DATA 0x58
#define BMI088_ACC_INT2_DRDY_INTERRUPT (0x1 << 0x6) // INT2���ݾ����ж���������
#define BMI088_ACC_INT1_DRDY_INTERRUPT (0x1 << 0x2) // INT1���ݾ����ж���������

// BMI088_ACC_SELF_TEST ���ٶȼ��Լ�Ĵ���
#define BMI088_ACC_SELF_TEST 0x6D
#define BMI088_ACC_SELF_TEST_OFF 0x00               // �Լ�ر�
#define BMI088_ACC_SELF_TEST_POSITIVE_SIGNAL 0x0D   // ���Լ��ź�
#define BMI088_ACC_SELF_TEST_NEGATIVE_SIGNAL 0x09   // ���Լ��ź�

// BMI088_ACC_PWR_CONF ���ٶȼƵ�Դģʽѡ��
#define BMI088_ACC_PWR_CONF 0x7C
#define BMI088_ACC_PWR_SUSPEND_MODE 0x03            // ����ģʽ
#define BMI088_ACC_PWR_ACTIVE_MODE 0x00             // ����ģʽ

// BMI088_ACC_PWR_CTRL
#define BMI088_ACC_PWR_CTRL 0x7D
#define BMI088_ACC_ENABLE_ACC_OFF 0x00              // �رռ��ٶȼ�
#define BMI088_ACC_ENABLE_ACC_ON 0x04               // �򿪼��ٶȼ�


/****************************   ������   *******************************/

// �����ǳ�ʼ��: ��[0x14]��[GYRO_SOFTRESET]�Ĵ���д��[0xB6]��������
#define BMI088_GYRO_SOFTRESET 0x14
#define BMI088_GYRO_SOFTRESET_VALUE 0xB6

// BMI088��Ҫ���õļĴ�����Ŀ
#define BMI088_WRITE_GYRO_REG_NUM   6

// Who am I �豸ID��֤
// �����ǵ� ID �Ĵ�����ַΪ 0x00����׼ ID ֵΪ 0x0F
#define BMI088_GYRO_CHIP_ID 0x00
#define BMI088_GYRO_CHIP_ID_VALUE 0x0F

// BMI088_GYRO_RATE ���������ݵ�ַ
#define BMI088_GYRO_RATE_X_L 0x02
#define BMI088_GYRO_RATE_X_M 0x03
#define BMI088_GYRO_RATE_Y_L 0x04
#define BMI088_GYRO_RATE_Y_M 0x05
#define BMI088_GYRO_RATE_Z_L 0x06
#define BMI088_GYRO_RATE_Z_M 0x07

// GYRO_RANGE �����Ƿ�Χѡ��
#define BMI088_GYRO_RANGE 0x0F
#define BMI088_GYRO_2000 0x0   // ����:2000 | ������:16.384 LSB/��/s
#define BMI088_GYRO_1000 0x1   // ����:1000 | ������:32.768 LSB/��/s
#define BMI088_GYRO_500  0x2   // ����:500 | ������:65.536 LSB/��/s
#define BMI088_GYRO_250  0x3   // ����:250 | ������:131.072 LSB/��/s
#define BMI088_GYRO_125  0x4   // ����:125 | ������:262.144 LSB/��/s

// BMI088_GYRO_BANDWIDTH ���������ݹ���������(Filter Bandwidth)�������������(ODR)
#define BMI088_GYRO_BANDWIDTH 0x10
#define BMI088_GYRO_BANDWIDTH_MUST_Set 0x80     // ���λ�������ݵ�ͨ�˲���
#define BMI088_GYRO_2000_532_HZ 0x00            // ODR: 2000Hz |  Filter Bandwidth: 532Hz
#define BMI088_GYRO_2000_230_HZ 0x01            // ODR: 2000Hz |  Filter Bandwidth: 230Hz
#define BMI088_GYRO_1000_116_HZ 0x02            // ODR: 1000Hz |  Filter Bandwidth: 116Hz
#define BMI088_GYRO_400_47_HZ 0x03              // ODR: 400Hz |  Filter Bandwidth: 47Hz
#define BMI088_GYRO_200_23_HZ 0x04              // ODR: 200Hz |  Filter Bandwidth: 23Hz
#define BMI088_GYRO_100_12_HZ 0x05              // ODR: 100Hz |  Filter Bandwidth: 12Hz
#define BMI088_GYRO_200_64_HZ 0x06              // ODR: 200Hz |  Filter Bandwidth: 64Hz
#define BMI088_GYRO_100_32_HZ 0x07              // ODR: 100Hz |  Filter Bandwidth: 32Hz

// BMI088_GYRO_LPM1 �����ǵ�Դģʽѡ��
#define BMI088_GYRO_LPM1 0x11
#define BMI088_GYRO_NORMAL_MODE 0x00            // ����ģʽ
#define BMI088_GYRO_SUSPEND_MODE 0x80           // ����ģʽ
#define BMI088_GYRO_DEEP_SUSPEND_MODE 0x20      // �����ģʽ

// BMI088_GYRO_CTRL �������ж�ģʽѡ��
#define BMI088_GYRO_CTRL 0x15
#define BMI088_DRDY_OFF 0x00                    // δ�������ݾ����ж�
#define BMI088_DRDY_ON 0x80                     // �������������ϴ����������ж�

// BMI088_GYRO_INT3_INT4_IO_CONF �����������ж����ŵĵ������߼�����
#define BMI088_GYRO_INT3_INT4_IO_CONF 0x16
#define BMI088_GYRO_INT3_GPIO_OD (0x1 << 0x1)           // INT3����OD��©���
#define BMI088_GYRO_INT3_GPIO_PP (0x0 << 0x1)           // INT3����PP�������
#define BMI088_GYRO_INT3_GPIO_HIGH (0x1 << 0x0)         // INT3���Ÿߵ�ƽ
#define BMI088_GYRO_INT3_GPIO_LOW (0x0 << 0x0)          // INT3���ŵ͵�ƽ
#define BMI088_GYRO_INT4_GPIO_OD (0x1 << 0x3)           // INT4����PP�������
#define BMI088_GYRO_INT4_GPIO_PP (0x0 << 0x3)           // INT4����OD��©���
#define BMI088_GYRO_INT4_GPIO_HIGH (0x1 << 0x2)         // INT4���Ÿߵ�ƽ
#define BMI088_GYRO_INT4_GPIO_LOW (0x0 << 0x2)          // INT4���ŵ͵�ƽ

// BMI088_GYRO_INT3_INT4_IO_MAP ���������ݾ����ж�����
#define BMI088_GYRO_INT3_INT4_IO_MAP 0x18
#define BMI088_GYRO_DRDY_IO_OFF 0x00                    // �����ݾ�������
#define BMI088_GYRO_DRDY_IO_INT3 0x01                   // ���ݾ�������INT3
#define BMI088_GYRO_DRDY_IO_INT4 0x80                   // ���ݾ�������INT4
#define BMI088_GYRO_DRDY_IO_BOTH 0x81                   // ���ݾ�������INT3 | INT4

// BMI088_GYRO_SELF_TEST �������Լ�Ĵ���
#define BMI088_GYRO_SELF_TEST 0x3C
#define BMI088_GYRO_TRIG_BIST 0x01                      // ���������Լ�
#define BMI088_GYRO_RATE_OK   (0x1 << 0x3)              // rate_ok
#define BMI088_GYRO_BIST_FAIL (0x1 << 0x2)              // bist_fail
#define BMI088_GYRO_BIST_RDY  (0x1 << 0x1)              // bist_rdy

#endif //STM32_DEMO_BMI088REG_H
