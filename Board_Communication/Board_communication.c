//
// Created by 小新 on 2024/2/16.
//

#include "Board_communication.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

CAN_TxHeaderTypeDef CAN_Tx_message;     // CAN的发送头
uint8_t CAN_TX_data[8];                 // 用于发送数据
uint8_t CAN_RX_data[8];                 // 用于接收数据
uint8_t CAN_RX_data_0[8];               // 用于接收数据
uint8_t CAN_RX_data_1[8];               // 用于接收数据
uint32_t send_mail;                     // 邮箱，发送失败的id号码存放在这里
uint32_t send_mail_2;                     // 邮箱，发送失败的id号码存放在这里
uint32_t send_mail_3;                     // 邮箱，发送失败的id号码存放在这里


/**
 *  uint32_t StdId;     存储报文的标准标识符11位，0-0x7FF.
 *  uint32_t ExtId;     存储报文的扩展标识符29位，0-0x1FFFFFFF.
                        ExtId与StdId这两个成员根据IDE位配置，只有一个是有效的。
 *  uint8_t IDE;        存储IDE扩展标志
                        当它的值为宏CAN_ID_STD时表示本报文是标准帧，使用StdId成员存储报文ID；
                        当它的值为宏CAN_ID_EXT时表示本报文是扩展帧，使用ExtId成员存储报文ID。
 *  uint8_t RTR;        存储RTR远程帧标志
                        当它的值为宏CAN_RTR_Data时表示本报文是数据帧；
                        当它的值为宏CAN_RTR_Remote时表示本报文是遥控帧，
                        由于遥控帧没有数据段，所以当报文是遥控帧时，下面的Data[8]成员的内容是无效的。
 *  uint8_t DLC;        存储报文数据段的长度，0-8, 当报文是遥控帧时DLC值为0。
 **/

// CAN发送数据
void CAN_TX(){
    CAN_Tx_message.StdId = 0x300;           // 标识符
    CAN_Tx_message.IDE = CAN_ID_STD;        // 标准帧
    CAN_Tx_message.RTR = CAN_RTR_DATA;      // 数据帧
    CAN_Tx_message.DLC = 0x08;				// 发送数据长度，8个字节
    CAN_Tx_message.TransmitGlobalTime = DISABLE;

    // 放置数据 RoboMaster
    CAN_TX_data[0] = 'R';
    CAN_TX_data[1] = 'o';
    CAN_TX_data[2] = 'b';
    CAN_TX_data[3] = 'o';
    CAN_TX_data[4] = 'M';
    CAN_TX_data[5] = 'a';
    CAN_TX_data[6] = 's';
    CAN_TX_data[7] = 't';

    // 发送数据(最多同时发送3个)
    HAL_CAN_AddTxMessage(&hcan1,&CAN_Tx_message,CAN_TX_data,&send_mail);//发送数据
//    HAL_CAN_AddTxMessage(&hcan1,&CAN_Tx_message,CAN_TX_data,&send_mail_2);//发送数据
//    HAL_CAN_AddTxMessage(&hcan1,&CAN_Tx_message,CAN_TX_data,&send_mail_3);//发送数据

    HAL_Delay(10);
}

// CAN的中断回调函数，当接收到数据，跳到此函数处理
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, CAN_RX_data);
    // 数据处理部分
    if(rx_header.ExtId == 0x300){
        htim5.Instance->CCR2=100;
    }

}

// CAN的中断回调函数，当接收到数据，跳到此函数处理
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, CAN_RX_data);
    // 数据处理部分
//    if(rx_header.StdId == 0x300){
//        if(CAN_RX_data[4] == 'M'){
//            htim5.Instance->CCR2=100;
//        }
//    }
    htim5.Instance->CCR3=100;

}

