//
// Created by С�� on 2024/2/14.
//

#include "PID.h"

/**
  * @brief     PID �޷�����
  * @param[in] a: �޷�����
  * @param[in] ABS_MAX: ����޷�
  */
static void abs_limit(float *a, float ABS_MAX)
{
    // ��a�޷���+/-ABS_MAX֮��
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}

/**
  * @brief     PID ��ʼ������
  * @param[in] pid: PID �ṹ��
  * @param[in] max_out: ������
  * @param[in] intergral_limit: �����޷�
  * @param[in] kp/ki/kd: ���� PID ����
  */
void pid_init(pid_type_def *pid, uint32_t max_out, uint32_t intergral_limit,float kp, float ki, float kd)
{
    // pid����������޷�
    pid->integral_limit = intergral_limit;
    // pid����������
    pid->max_output     = max_out;

    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}

/**
  * @brief     PID ���㺯����ʹ��λ��ʽ PID ����
  * @param[in] pid: PID �ṹ��
  * @param[in] get: ��������
  * @param[in] set: Ŀ������
  * @retval    PID �������
  */
float pid_calc(pid_type_def *pid, float get, float set)
{
    // ����Ŀ��ֵ������ֵ,�������ֵ
    pid->get = get;
    pid->set = set;
    pid->err[NOW] = set - get;

    // p��i��d�������������
    pid->pout = pid->p * pid->err[NOW];
    pid->iout += pid->i * pid->err[NOW];
    pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]);

    abs_limit(&(pid->iout), pid->integral_limit);    // pid��i�޷�
    pid->out = pid->pout + pid->iout + pid->dout;    // pid���ֵ
    abs_limit(&(pid->out), pid->max_output);         // pid������ֵ�޷�

    // �����µ����ֵ
    pid->err[LAST]  = pid->err[NOW];

    return pid->out;
}

/**
  * @brief     PID �ջ�������ʹ��λ��ʽ PID ����
  * @param[in] pid: PID �ṹ��
  * @param[in] get: ��������
  * @param[in] set: Ŀ������
  * @param[in] max_value: ���ֵ
  * @param[in] min_value: ��Сֵ
  * @retval    PID �������
  */
float pid_loop_calc(pid_type_def *pid,float get,float set,float max_value,float min_value){
    // yaw��ջ�,�����180�ȵ�����
    float mid;  // ��ֵ
    float gap;  // ���ֵ
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
  * @brief     PID ������λ����
  * @param[in] pid: PID �ṹ��
  * @param[in] kp/ki/kd: ���� PID ����
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