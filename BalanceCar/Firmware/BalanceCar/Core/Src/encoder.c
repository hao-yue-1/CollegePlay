//
// Created by yue on 2022/1/25.
//

#include "encoder.h"

/*
*********************************************************************************************************
*	�� �� ��: ReadEncoder
*	����˵��: ��ȡ��������ֵ
*	��    ��: TIMX����ȡ�Ķ�ʱ��
*	�� �� ֵ: ��������ֵ
*	ע    ��: TIM3������    TIM2���ҵ��
*********************************************************************************************************
*/
int ReadEncoder(uint8_t TIMX)
{
    int Encoder_TIM;

    switch(TIMX)
    {
        case 2 :Encoder_TIM=(short)TIM2->CNT;	TIM2->CNT=0;	break;
        case 3 :Encoder_TIM=-(short)TIM3->CNT;	TIM3->CNT=0;	break;  //�������������Է��ã���������һ��Ҫȡ��
        default:Encoder_TIM=0;
    }
    return Encoder_TIM;
}