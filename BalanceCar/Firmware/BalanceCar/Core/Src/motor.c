//
// Created by yue on 2022/1/25.
//

#include "motor.h"
#include "tim.h"

/*
*********************************************************************************************************
*	�� �� ��: MotorSetPWM
*	����˵��: Ϊ���ҵ����ֵPWM
*	��    ��: pwm_l��������PWM
*	         pwm_r���ҵ����PWM
*	�� �� ֵ: ��
*	ע    ��: �ó�������ʹ������PWMͨ������һ������ĵ������ģʽ�����磺A4950
 *	         ������CH3��CH4
 *	         �ҵ����CH1��CH2
*********************************************************************************************************
*/
void MotorSetPWM(int pwm_l,int pwm_r)
{
    if(pwm_l>=0)    //������ת
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,pwm_l);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);

    }
    else            //������ת
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,-pwm_l);
    }
    if(pwm_r>=0)    //�ҵ����ת
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,pwm_r);
    }
    else            //�ҵ����ת
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,-pwm_r);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);
    }
}

