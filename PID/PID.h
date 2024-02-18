//
// Created by 小新 on 2024/2/14.
//

#ifndef STM32_DEMO_PID_H
#define STM32_DEMO_PID_H

#include "stdint.h"
#include <math.h>

enum
{
    LAST  = 0,
    NOW   = 1,
};

/**
  * @brief     PID 结构体
  */
typedef struct
{
    /* p、i、d参数 */
    float p;
    float i;
    float d;

    /* 目标值、反馈值、误差值 */
    float set;
    float get;
    float err[2];

    /* p、i、d各项计算出的输出 */
    float pout;
    float iout;
    float dout;

    /* pid公式计算出的总输出 */
    float out;

    /* pid最大输出限制  */
    uint32_t max_output;

    /* pid积分输出项限幅 */
    uint32_t integral_limit;

} pid_type_def;

/**
  * @brief     PID 初始化函数
  * @param[in] pid: PID 结构体
  * @param[in] max_out: 最大输出
  * @param[in] intergral_limit: 积分限幅
  * @param[in] kp/ki/kd: 具体 PID 参数
  */
extern void pid_init(pid_type_def *pid, uint32_t max_out, uint32_t intergral_limit,float kp, float ki, float kd);

/**
  * @brief     PID 计算函数，使用位置式 PID 计算
  * @param[in] pid: PID 结构体
  * @param[in] get: 反馈数据
  * @param[in] set: 目标数据
  * @retval    PID 计算输出
  */
extern float pid_calc(pid_type_def *pid, float get, float set);

/**
  * @brief     PID 闭环函数，使用位置式 PID 计算
  * @param[in] pid: PID 结构体
  * @param[in] get: 反馈数据
  * @param[in] set: 目标数据
  * @param[in] max_value: 最大值
  * @param[in] min_value: 最小值
  * @retval    PID 计算输出
  */
extern float pid_loop_calc(pid_type_def *pid,float get,float set,float max_value,float min_value);

/**
  * @brief     PID 参数复位函数
  * @param[in] pid: PID 结构体
  * @param[in] kp/ki/kd: 具体 PID 参数
  */
extern void pid_reset(pid_type_def *pid, float kp, float ki, float kd);

#endif //STM32_DEMO_PID_H
