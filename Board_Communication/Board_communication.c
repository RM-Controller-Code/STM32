//
// Created by С�� on 2024/2/16.
//

#include "Board_communication.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

CAN_TxHeaderTypeDef CAN_Tx_message;     // CAN�ķ���ͷ
uint8_t CAN_TX_data[8];                 // ���ڷ�������
uint8_t CAN_RX_data[8];                 // ���ڽ�������
uint8_t CAN_RX_data_0[8];               // ���ڽ�������
uint8_t CAN_RX_data_1[8];               // ���ڽ�������
uint32_t send_mail;                     // ���䣬����ʧ�ܵ�id������������
uint32_t send_mail_2;                     // ���䣬����ʧ�ܵ�id������������
uint32_t send_mail_3;                     // ���䣬����ʧ�ܵ�id������������


/**
 *  uint32_t StdId;     �洢���ĵı�׼��ʶ��11λ��0-0x7FF.
 *  uint32_t ExtId;     �洢���ĵ���չ��ʶ��29λ��0-0x1FFFFFFF.
                        ExtId��StdId��������Ա����IDEλ���ã�ֻ��һ������Ч�ġ�
 *  uint8_t IDE;        �洢IDE��չ��־
                        ������ֵΪ��CAN_ID_STDʱ��ʾ�������Ǳ�׼֡��ʹ��StdId��Ա�洢����ID��
                        ������ֵΪ��CAN_ID_EXTʱ��ʾ����������չ֡��ʹ��ExtId��Ա�洢����ID��
 *  uint8_t RTR;        �洢RTRԶ��֡��־
                        ������ֵΪ��CAN_RTR_Dataʱ��ʾ������������֡��
                        ������ֵΪ��CAN_RTR_Remoteʱ��ʾ��������ң��֡��
                        ����ң��֡û�����ݶΣ����Ե�������ң��֡ʱ�������Data[8]��Ա����������Ч�ġ�
 *  uint8_t DLC;        �洢�������ݶεĳ��ȣ�0-8, ��������ң��֡ʱDLCֵΪ0��
 **/

// CAN��������
void CAN_TX(){
    CAN_Tx_message.StdId = 0x300;           // ��ʶ��
    CAN_Tx_message.IDE = CAN_ID_STD;        // ��׼֡
    CAN_Tx_message.RTR = CAN_RTR_DATA;      // ����֡
    CAN_Tx_message.DLC = 0x08;				// �������ݳ��ȣ�8���ֽ�
    CAN_Tx_message.TransmitGlobalTime = DISABLE;

    // �������� RoboMaster
    CAN_TX_data[0] = 'R';
    CAN_TX_data[1] = 'o';
    CAN_TX_data[2] = 'b';
    CAN_TX_data[3] = 'o';
    CAN_TX_data[4] = 'M';
    CAN_TX_data[5] = 'a';
    CAN_TX_data[6] = 's';
    CAN_TX_data[7] = 't';

    // ��������(���ͬʱ����3��)
    HAL_CAN_AddTxMessage(&hcan1,&CAN_Tx_message,CAN_TX_data,&send_mail);//��������
//    HAL_CAN_AddTxMessage(&hcan1,&CAN_Tx_message,CAN_TX_data,&send_mail_2);//��������
//    HAL_CAN_AddTxMessage(&hcan1,&CAN_Tx_message,CAN_TX_data,&send_mail_3);//��������

    HAL_Delay(10);
}

// CAN���жϻص������������յ����ݣ������˺�������
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, CAN_RX_data);
    // ���ݴ�����
    if(rx_header.ExtId == 0x300){
        htim5.Instance->CCR2=100;
    }

}

// CAN���жϻص������������յ����ݣ������˺�������
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, CAN_RX_data);
    // ���ݴ�����
//    if(rx_header.StdId == 0x300){
//        if(CAN_RX_data[4] == 'M'){
//            htim5.Instance->CCR2=100;
//        }
//    }
    htim5.Instance->CCR3=100;

}

