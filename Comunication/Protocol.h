//
// Created by 小新 on 2024/2/18.
//

#ifndef STM32_FREERTOS_PROTOCOL_H
#define STM32_FREERTOS_PROTOCOL_H

typedef float fp32;
typedef double fp64;

// 数据帧起始字节,固定值为 0xA5
#define HEADER_SOF 0xA5
// 数据帧头尾
#define END1_SOF 0x0D
#define END2_SOF 0x0A

// 协议数据部分
#define REF_PROTOCOL_FRAME_MAX_SIZE 128     // 最大数据长度
#define REF_PROTOCOL_HEADER_SIZE            sizeof(frame_header_struct_t)   // 帧头数据长度
#define REF_PROTOCOL_CMD_SIZE               2                               // CMD_ID命令长度
#define REF_PROTOCOL_CRC16_SIZE             2                               // CRC16数据长度
#define REF_HEADER_CRC_LEN                  (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE)    // 帧头+CRC16
#define REF_HEADER_CRC_CMDID_LEN            (REF_HEADER_CRC_LEN + sizeof(uint16_t))                 // 帧头+CRC16+CMD_ID
#define REF_HEADER_CMDID_LEN                (REF_PROTOCOL_HEADER_SIZE + sizeof(uint16_t))           // 帧头+CMD_ID


typedef enum
{
    CHASSIS_ODOM_CMD_ID = 0x0101,
    CHASSIS_CTRL_CMD_ID = 0x0102,
    RGB_ID = 0x0103,
    RC_ID = 0x0104,
    VISION_ID = 0x0105
} data_cmd_id;

// 数据帧尾结构体(供视觉进行解包)
typedef struct
{
    uint8_t end1;
    uint8_t end2;
} msg_end_info ;

typedef struct
{
    float vx;
    float vy;
    float vw;
}  chassis_odom_info_t;

//裁判系统帧头结构体
typedef  struct
{
    uint8_t SOF;                // 数据帧起始字节，固定值为 0xA5
    uint16_t data_length;       // 数据帧中 data 的长度
    uint8_t seq;                // 包序号
    uint8_t CRC8;               // 帧头 CRC8 校验
}__packed frame_header_struct_t;

//给视觉上发送的数据
typedef struct
{
    uint16_t id;                // ID 暂时没有作用
    uint16_t mode;              // 0x21启动自瞄
    fp32 pitch;                 // 俯仰角
    fp32 yaw;                   // 偏航角
    fp32 roll;                  // 翻滚角
    fp32 quaternion[4];         // 四元数
    fp32 shoot_speed;           // 弹速
} vision_t;

//机器人控制数据
typedef struct
{
    fp32 vx;
    fp32 vy;
    fp32 vw;
    fp32 yaw;                   // 偏航角
    fp32 pitch;                 // 俯仰角
    int8_t target_lock;         // 是否跟随 0x31(跟随) | 0x32(不跟随)
    int8_t fire_command;        // 火控 0:不开火 | 1:开火
    int8_t aim_id;
}  robot_ctrl_info_t;


typedef enum
{
    STEP_HEADER_SOF  = 0,
    STEP_LENGTH_LOW  = 1,
    STEP_LENGTH_HIGH = 2,
    STEP_FRAME_SEQ   = 3,
    STEP_HEADER_CRC8 = 4,
    STEP_DATA_CRC16  = 5,
} unpack_step_e;

//解包结构体
typedef struct
{
    frame_header_struct_t *p_header;
    uint16_t       data_len;                                        // 数据长度
    uint8_t        protocol_packet[REF_PROTOCOL_FRAME_MAX_SIZE];    // 所有的数据存放数组
    unpack_step_e  unpack_step;                                     // 解包状态
    uint16_t       index;                                           // 当前索引
} unpack_data_t;

#endif //STM32_FREERTOS_PROTOCOL_H
