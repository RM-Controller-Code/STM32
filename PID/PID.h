//
// Created by С�� on 2024/2/14.
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
  * @brief     PID �ṹ��
  */
typedef struct
{
    /* p��i��d���� */
    float p;
    float i;
    float d;

    /* Ŀ��ֵ������ֵ�����ֵ */
    float set;
    float get;
    float err[2];

    /* p��i��d������������� */
    float pout;
    float iout;
    float dout;

    /* pid��ʽ������������ */
    float out;

    /* pid����������  */
    uint32_t max_output;

    /* pid����������޷� */
    uint32_t integral_limit;

} pid_type_def;

/**
  * @brief     PID ��ʼ������
  * @param[in] pid: PID �ṹ��
  * @param[in] max_out: ������
  * @param[in] intergral_limit: �����޷�
  * @param[in] kp/ki/kd: ���� PID ����
  */
extern void pid_init(pid_type_def *pid, uint32_t max_out, uint32_t intergral_limit,float kp, float ki, float kd);

/**
  * @brief     PID ���㺯����ʹ��λ��ʽ PID ����
  * @param[in] pid: PID �ṹ��
  * @param[in] get: ��������
  * @param[in] set: Ŀ������
  * @retval    PID �������
  */
extern float pid_calc(pid_type_def *pid, float get, float set);

/**
  * @brief     PID �ջ�������ʹ��λ��ʽ PID ����
  * @param[in] pid: PID �ṹ��
  * @param[in] get: ��������
  * @param[in] set: Ŀ������
  * @param[in] max_value: ���ֵ
  * @param[in] min_value: ��Сֵ
  * @retval    PID �������
  */
extern float pid_loop_calc(pid_type_def *pid,float get,float set,float max_value,float min_value);

/**
  * @brief     PID ������λ����
  * @param[in] pid: PID �ṹ��
  * @param[in] kp/ki/kd: ���� PID ����
  */
extern void pid_reset(pid_type_def *pid, float kp, float ki, float kd);

#endif //STM32_DEMO_PID_H
