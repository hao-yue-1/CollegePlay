//
// Created by yue on 2022/1/25.
//

#ifndef BALANCECAR_ENCODER_H
#define BALANCECAR_ENCODER_H

#include "sys.h"

int ReadEncoder(u8 TIMX);

#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�

#endif //BALANCECAR_ENCODER_H
