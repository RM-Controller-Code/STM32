//
// Created by 小新 on 2024/1/13.
//

#ifndef STM32_DEMO_BUZZER_H
#define STM32_DEMO_BUZZER_H

#include "stdio.h"
#include <tim.h>

#define  proport          84000 	//Tclk/(arr+1)=84000000/(1000)

//根据Tout= ((arr+1)*(psc+1))/Tclk推出psc值就是本句define定义的值，Tout为音调频率131Hz的倒数，Tclk=84MHz
#define  L1       ((proport/262)-1)//低调　do 的频率
#define  L2       ((proport/296)-1)//低调　re 的频率
#define  L3       ((proport/330)-1)//低调　mi 的频率
#define  L4       ((proport/349)-1)//低调　fa 的频率
#define  L5       ((proport/392)-1)//低调　sol 的频率
#define  L6       ((proport/440)-1)//低调　la 的频率
#define  L7       ((proport/494)-1)//低调　si 的频率

#define  M1       ((proport/523)-1)//中调　do 的频率
#define  M2       ((proport/587)-1)//中调　re 的频率
#define  M3       ((proport/659)-1)//中调　mi 的频率
#define  M4       ((proport/699)-1)//中调　fa 的频率
#define  M5       ((proport/784)-1)//中调　sol的频率
#define  M6       ((proport/880)-1)//中调　la 的频率
#define  M7       ((proport/988)-1)//中调　si 的频率

#define  H1       ((proport/1048)-1)//高调　do 的频率
#define  H2       ((proport/1176)-1)//高调　re 的频率
#define  H3       ((proport/1320)-1)//高调　mi 的频率
#define  H4       ((proport/1480)-1)//高调　fa 的频率
#define  H5       ((proport/1640)-1)//高调　sol的频率
#define  H6       ((proport/1760)-1)//高调　la 的频率
#define  H7       ((proport/1976)-1)//高调　si 的频率

#define  Z0       0//

#define MAX_BUZZER_PWM 20000    // 最大PWM值
#define MAX_PSC 1000            // 最大PSC值

// 无横线为1拍 单横线为1/2拍 双横线为1/4拍 数字后面有点加半拍 数字后有横线加一拍
// 1/4拍为50 1/2拍100 1拍为200
extern uint8_t OK;

void Buzzer_on(uint16_t psc,uint16_t pwm);
void Buzzer_off();

// 乐谱
void Solitary_brave();
void wind_rise();       // 起风了
void Canon();           // 卡农

#endif //STM32_DEMO_BUZZER_H
