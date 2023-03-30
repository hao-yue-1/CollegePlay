//
// Created by yue on 2022/1/25.
//

#include "motor.h"
#include "tim.h"

/*
*********************************************************************************************************
*	函 数 名: MotorSetPWM
*	功能说明: 为左右电机赋值PWM
*	形    参: pwm_l：左电机的PWM
*	         pwm_r：右电机的PWM
*	返 回 值: 无
*	注    意: 该程序适配使用两个PWM通道控制一个电机的电机驱动模式，例如：A4950
 *	         左电机：CH3、CH4
 *	         右电机：CH1、CH2
*********************************************************************************************************
*/
void MotorSetPWM(int pwm_l,int pwm_r)
{
    if(pwm_l>=0)    //左电机正转
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,pwm_l);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);

    }
    else            //左电机反转
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,-pwm_l);
    }
    if(pwm_r>=0)    //右电机正转
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,pwm_r);
    }
    else            //右电机反转
    {
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,-pwm_r);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);
    }
}

