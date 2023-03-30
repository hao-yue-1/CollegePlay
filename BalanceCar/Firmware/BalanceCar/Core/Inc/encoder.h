//
// Created by yue on 2022/1/25.
//

#ifndef BALANCECAR_ENCODER_H
#define BALANCECAR_ENCODER_H

#include "sys.h"

int ReadEncoder(u8 TIMX);

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

#endif //BALANCECAR_ENCODER_H
