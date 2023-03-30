//
// Created by yue on 2022/1/25.
//

#include "encoder.h"

/*
*********************************************************************************************************
*	函 数 名: ReadEncoder
*	功能说明: 读取编码器的值
*	形    参: TIMX：读取的定时器
*	返 回 值: 编码器的值
*	注    意: TIM3：左电机    TIM2：右电机
*********************************************************************************************************
*/
int ReadEncoder(uint8_t TIMX)
{
    int Encoder_TIM;

    switch(TIMX)
    {
        case 2 :Encoder_TIM=(short)TIM2->CNT;	TIM2->CNT=0;	break;
        case 3 :Encoder_TIM=-(short)TIM3->CNT;	TIM3->CNT=0;	break;  //由于两个电机相对放置，所以其中一个要取反
        default:Encoder_TIM=0;
    }
    return Encoder_TIM;
}