//
// Created by 小新 on 2024/2/4.
//
#include "Motor_CAN.h"
#include <tim.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
CAN_TxHeaderTypeDef Tx_message; //can的发送头
uint8_t CAN_send_data[8];//用于发送数据
uint8_t rx_data[8];//用于接收数据
uint32_t send_mail_box;//邮箱，发送失败的id号码存放在这里
dji_motor_measure_t motor1_data;//将收集到的电机数据存放到这里
pid_type_def PID;              // PID数据
track_control_t motor;

void CAN_filter_init(){
    //! FIFO0的优先级高于FIFO1
    CAN_FilterTypeDef can_filter_set;
    can_filter_set.FilterActivation = ENABLE;               // 启动过滤器
    can_filter_set.FilterMode = CAN_FILTERMODE_IDMASK;      // 掩码模式
    can_filter_set.FilterScale = CAN_FILTERSCALE_32BIT;     // 32位宽
    can_filter_set.FilterIdHigh = 0x0000;                   // ID最高位
    can_filter_set.FilterIdLow = 0x0000;                    // ID最低位
    can_filter_set.FilterMaskIdHigh = 0x0000;               // 掩码高位不检测
    can_filter_set.FilterMaskIdLow = 0x0000;                // 0表示接收来自发送端任意ID的数据
    can_filter_set.FilterBank = 0;                          // 将要初始化的过滤器组
    can_filter_set.SlaveStartFilterBank  = 0;               // 从模式下的过滤器组
    can_filter_set.FilterFIFOAssignment = CAN_RX_FIFO0;     // 接收到的报文放到FIFO0中

    // CAN1滤波器
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_set);
    HAL_CAN_Start(&hcan1);
    // 启动CAN接收中断-FIFO0接收新消息
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

    // CAN的FIFO1过滤器
    can_filter_set.FilterActivation = ENABLE;               // 启动过滤器
    can_filter_set.FilterMode = CAN_FILTERMODE_IDMASK;      // 掩码模式
    can_filter_set.FilterScale = CAN_FILTERSCALE_32BIT;     // 32位宽
    can_filter_set.FilterIdHigh = 0x0000;                   // ID最高位
    can_filter_set.FilterIdLow = 0x0000;                    // ID最低位
    can_filter_set.FilterMaskIdHigh = 0x0000;               // 掩码高位不检测
    can_filter_set.FilterMaskIdLow = 0x0000;                // 0表示接收来自发送端任意ID的数据
    can_filter_set.FilterBank = 7;                          // 将要初始化的过滤器组
    can_filter_set.FilterFIFOAssignment = CAN_RX_FIFO1;     // 接收到的报文放到FIFO0中
    // CAN1滤波器
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_set);
    HAL_CAN_Start(&hcan1);
    // 启动CAN接收中断-FIFO1接收新消息
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);

    // CAN2滤波器
    can_filter_set.SlaveStartFilterBank = 14;
    can_filter_set.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_set);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void CAN_cmd_motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4){
//    Tx_message.StdId=0x200;        // 控制电机1-4
    Tx_message.StdId=0x1FF;        // 控制电机5-8
    Tx_message.IDE=CAN_ID_STD;
    Tx_message.RTR=CAN_RTR_DATA;
    Tx_message.DLC=0x08;				//发送数据长度，8个字节
    //电机1号
    CAN_send_data[0]=motor1>>8;         //高八位
    CAN_send_data[1]=motor1;		    //低八位
    //电机2号
    CAN_send_data[2]=motor2>>8;
    CAN_send_data[3]=motor2;
    //电机3号
    CAN_send_data[4]=motor3>>8;
    CAN_send_data[5]=motor3;
    //电机4号
    CAN_send_data[6]=motor4>>8;
    CAN_send_data[7]=motor4;
    // 发送数据
    HAL_CAN_AddTxMessage(&hcan1,&Tx_message,CAN_send_data,&send_mail_box);//发送数据
    HAL_Delay(2);
}

void CAN_Slove(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]){

    if(hcan == &hcan1)//来自CAN1口的信息
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
                get_motor_measure(&motor1_data, rx_data);//获取数据
//                get_motor_round_cnt(&motor1_data[i]);//获取补充圈数数据
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
    // 过0点问题
    uint16_t set_ecd = 5000;
    motor.set_position = pid_calc(&PID,motor1_data.ecd,set_ecd);
    CAN_cmd_motor(motor.set_position,0,0,0);
}

/* 用来将获取的数据封装进电机结构体中*/
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
//接收中断回调函数，用于接收CAN通道发来的信息
//! 在中断函数打开的情况下,必须将其标志设置为完成,否则无法发送数据
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
//    hcan1.Instance->RF0R |= CAN_RF0R_RFOM0;   // 这个标志位表示释放 FIFO 0 的输出邮箱，使其可以接收新的 CAN 消息
    // 数据处理
    htim5.Instance->CCR1=100;
    CAN_Slove(hcan,rx_header,rx_data);
}
#endif //OK