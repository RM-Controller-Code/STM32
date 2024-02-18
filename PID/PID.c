//
// Created by 小新 on 2024/2/14.
//

#include "PID.h"

/**
  * @brief     PID 限幅函数
  * @param[in] a: 限幅对象
  * @param[in] ABS_MAX: 最大限幅
  */
static void abs_limit(float *a, float ABS_MAX)
{
    // 将a限幅在+/-ABS_MAX之间
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}

/**
  * @brief     PID 初始化函数
  * @param[in] pid: PID 结构体
  * @param[in] max_out: 最大输出
  * @param[in] intergral_limit: 积分限幅
  * @param[in] kp/ki/kd: 具体 PID 参数
  */
void pid_init(pid_type_def *pid, uint32_t max_out, uint32_t intergral_limit,float kp, float ki, float kd)
{
    // pid积分输出项限幅
    pid->integral_limit = intergral_limit;
    // pid最大输出限制
    pid->max_output     = max_out;

    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}

/**
  * @brief     PID 计算函数，使用位置式 PID 计算
  * @param[in] pid: PID 结构体
  * @param[in] get: 反馈数据
  * @param[in] set: 目标数据
  * @retval    PID 计算输出
  */
float pid_calc(pid_type_def *pid, float get, float set)
{
    // 设置目标值、反馈值,计算误差值
    pid->get = get;
    pid->set = set;
    pid->err[NOW] = set - get;

    // p、i、d各项计算出的输出
    pid->pout = pid->p * pid->err[NOW];
    pid->iout += pid->i * pid->err[NOW];
    pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]);

    abs_limit(&(pid->iout), pid->integral_limit);    // pid的i限幅
    pid->out = pid->pout + pid->iout + pid->dout;    // pid输出值
    abs_limit(&(pid->out), pid->max_output);         // pid最大输出值限幅

    // 设置新的误差值
    pid->err[LAST]  = pid->err[NOW];

    return pid->out;
}

/**
  * @brief     PID 闭环函数，使用位置式 PID 计算
  * @param[in] pid: PID 结构体
  * @param[in] get: 反馈数据
  * @param[in] set: 目标数据
  * @param[in] max_value: 最大值
  * @param[in] min_value: 最小值
  * @retval    PID 计算输出
  */
float pid_loop_calc(pid_type_def *pid,float get,float set,float max_value,float min_value){
    // yaw轴闭环,处理过180度的问题
    float mid;  // 中值
    float gap;  // 误差值
    mid = (max_value - min_value)/2;
    gap = set - get;
    if(gap >= mid){
        gap -= (max_value - min_value);
        return pid_calc(pid,-gap,0);
    }
    else if(gap <= -mid){
        gap += (max_value - min_value);
        return pid_calc(pid,-gap,0);
    }
    else{
        return pid_calc(pid,get,set);
    }
}



/**
  * @brief     PID 参数复位函数
  * @param[in] pid: PID 结构体
  * @param[in] kp/ki/kd: 具体 PID 参数
  */
void pid_reset(pid_type_def *pid, float kp, float ki, float kd)
{
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;

    pid->pout = 0;
    pid->iout = 0;
    pid->dout = 0;
    pid->out  = 0;
}