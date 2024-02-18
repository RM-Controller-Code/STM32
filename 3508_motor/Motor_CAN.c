//
// Created by С�� on 2024/2/4.
//
#include "Motor_CAN.h"
#include <tim.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
CAN_TxHeaderTypeDef Tx_message; //can�ķ���ͷ
uint8_t CAN_send_data[8];//���ڷ�������
uint8_t rx_data[8];//���ڽ�������
uint32_t send_mail_box;//���䣬����ʧ�ܵ�id������������
dji_motor_measure_t motor1_data;//���ռ����ĵ�����ݴ�ŵ�����
pid_type_def PID;              // PID����
track_control_t motor;

void CAN_filter_init(){
    //! FIFO0�����ȼ�����FIFO1
    CAN_FilterTypeDef can_filter_set;
    can_filter_set.FilterActivation = ENABLE;               // ����������
    can_filter_set.FilterMode = CAN_FILTERMODE_IDMASK;      // ����ģʽ
    can_filter_set.FilterScale = CAN_FILTERSCALE_32BIT;     // 32λ��
    can_filter_set.FilterIdHigh = 0x0000;                   // ID���λ
    can_filter_set.FilterIdLow = 0x0000;                    // ID���λ
    can_filter_set.FilterMaskIdHigh = 0x0000;               // �����λ�����
    can_filter_set.FilterMaskIdLow = 0x0000;                // 0��ʾ�������Է��Ͷ�����ID������
    can_filter_set.FilterBank = 0;                          // ��Ҫ��ʼ���Ĺ�������
    can_filter_set.SlaveStartFilterBank  = 0;               // ��ģʽ�µĹ�������
    can_filter_set.FilterFIFOAssignment = CAN_RX_FIFO0;     // ���յ��ı��ķŵ�FIFO0��

    // CAN1�˲���
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_set);
    HAL_CAN_Start(&hcan1);
    // ����CAN�����ж�-FIFO0��������Ϣ
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

    // CAN��FIFO1������
    can_filter_set.FilterActivation = ENABLE;               // ����������
    can_filter_set.FilterMode = CAN_FILTERMODE_IDMASK;      // ����ģʽ
    can_filter_set.FilterScale = CAN_FILTERSCALE_32BIT;     // 32λ��
    can_filter_set.FilterIdHigh = 0x0000;                   // ID���λ
    can_filter_set.FilterIdLow = 0x0000;                    // ID���λ
    can_filter_set.FilterMaskIdHigh = 0x0000;               // �����λ�����
    can_filter_set.FilterMaskIdLow = 0x0000;                // 0��ʾ�������Է��Ͷ�����ID������
    can_filter_set.FilterBank = 7;                          // ��Ҫ��ʼ���Ĺ�������
    can_filter_set.FilterFIFOAssignment = CAN_RX_FIFO1;     // ���յ��ı��ķŵ�FIFO0��
    // CAN1�˲���
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_set);
    HAL_CAN_Start(&hcan1);
    // ����CAN�����ж�-FIFO1��������Ϣ
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);

    // CAN2�˲���
    can_filter_set.SlaveStartFilterBank = 14;
    can_filter_set.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_set);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void CAN_cmd_motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4){
//    Tx_message.StdId=0x200;        // ���Ƶ��1-4
    Tx_message.StdId=0x1FF;        // ���Ƶ��5-8
    Tx_message.IDE=CAN_ID_STD;
    Tx_message.RTR=CAN_RTR_DATA;
    Tx_message.DLC=0x08;				//�������ݳ��ȣ�8���ֽ�
    //���1��
    CAN_send_data[0]=motor1>>8;         //�߰�λ
    CAN_send_data[1]=motor1;		    //�Ͱ�λ
    //���2��
    CAN_send_data[2]=motor2>>8;
    CAN_send_data[3]=motor2;
    //���3��
    CAN_send_data[4]=motor3>>8;
    CAN_send_data[5]=motor3;
    //���4��
    CAN_send_data[6]=motor4>>8;
    CAN_send_data[7]=motor4;
    // ��������
    HAL_CAN_AddTxMessage(&hcan1,&Tx_message,CAN_send_data,&send_mail_box);//��������
    HAL_Delay(2);
}

void CAN_Slove(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]){

    if(hcan == &hcan1)//����CAN1�ڵ���Ϣ
    {
        switch (rx_header.StdId)
        {
            case 0x201:
            case 0x202:
            case 0x203:
            case 0x204:
            case 0x205:
            {
                static uint8_t i = 0;
                i = rx_header.StdId - 0x201;
                get_motor_measure(&motor1_data, rx_data);//��ȡ����
//                get_motor_round_cnt(&motor1_data[i]);//��ȡ����Ȧ������
                break;
            }
            case 0x206:
            case 0x207:
            case 0x208:
            default: break;
        }
    }
}


void Send(){
    static uint8_t init = 0;
    if(init == 0){
        init = 1;
        pid_init(&PID, 2000, 100, 4.f,0,10.f);
    }
    // ��0������
    uint16_t set_ecd = 5000;
    motor.set_position = pid_calc(&PID,motor1_data.ecd,set_ecd);
    CAN_cmd_motor(motor.set_position,0,0,0);
}

/* ��������ȡ�����ݷ�װ������ṹ����*/
void get_motor_measure(dji_motor_measure_t *motorMeasure, const uint8_t *rxData)
{
    motorMeasure->last_ecd = (int16_t) motorMeasure->ecd;
    motorMeasure->ecd = (uint16_t) (rxData[0] << 8 | rxData[1]);
    motorMeasure->speed_rpm = (int16_t) (rxData[2] << 8 | rxData[3]);
    motorMeasure->given_current = (int16_t) (rxData[4] << 8 | rxData[5]);
    motorMeasure->temperate = (uint16_t) rxData[6];
}

//#define OK
#ifdef OK
//�����жϻص����������ڽ���CANͨ����������Ϣ
//! ���жϺ����򿪵������,���뽫���־����Ϊ���,�����޷���������
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
//    hcan1.Instance->RF0R |= CAN_RF0R_RFOM0;   // �����־λ��ʾ�ͷ� FIFO 0 ��������䣬ʹ����Խ����µ� CAN ��Ϣ
    // ���ݴ���
    htim5.Instance->CCR1=100;
    CAN_Slove(hcan,rx_header,rx_data);
}
#endif //OK