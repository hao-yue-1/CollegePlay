//
// Created by yue on 2022/1/31.
//

#include "control.h"
#include "encoder.h"
#include "pid.h"
#include "motor.h"
#include "retarget.h"
#include "DataScope_DP.h"
#include "sys.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "oled.h"

/*
*********************************************************************************************************
*	函 数 名: MotorSpeedCtrl
*	功能说明: 使用增量式PI控制电机速度
*	形    参: speed_l：左电机的PWM
*	          speed_r：右电机的PWM
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorSpeedCtrl(int speed_l,int speed_r)
{
    int encoder_l,encoder_r;
    encoder_l= ReadEncoder(3);  //读取左编码器的值
    encoder_r= ReadEncoder(2);  //读取右编码器的值

    int pwm_l,pwm_r;
    pwm_l= Incremental_PI_L(encoder_l,speed_l);   //左电机PID控速
    pwm_r= Incremental_PI_R(encoder_r,speed_r);   //右电机PID控速

    pwm_l= LimitPWM(pwm_l); //左电机PWM限幅
    pwm_r= LimitPWM(pwm_r); //右电机PWM限幅

    //使用波形显示上位机调试PID
    unsigned char Send_Count;
    DataScope_Get_Channel_Data(speed_l,1);
    DataScope_Get_Channel_Data(encoder_l,2);
    DataScope_Get_Channel_Data(speed_r,3);
    DataScope_Get_Channel_Data(encoder_r,4);
    Send_Count = DataScope_Data_Generate(4);
    for (int i = 0; i < Send_Count; i++)
    {
        while((USART2->SR&0X40)==0);
        USART2->DR = DataScope_OutPut_Buffer[i];
    }

    MotorSetPWM(pwm_l,pwm_r);   //电机赋值PWM
}

/*
*********************************************************************************************************
*	函 数 名: LimitPWM
*	功能说明: PWM限幅
*	形    参: 限幅前的PWM
*	返 回 值: 限幅后的PWM
*********************************************************************************************************
*/
int LimitPWM(int pwm)
{
    if(pwm>7200)
    {
        return 7100;
    }
    if(pwm<-7200)
    {
        return -7100;
    }
    return pwm;
}

/*
*********************************************************************************************************
*	函 数 名: BalancePIDCtrl
*	功能说明: 平衡小车的控制，实现原地自平衡
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BalancePIDCtrl(float mid)
{
    float pitch;
    short gyro;
    int PWM_PD;
    while(mpu_dmp_get_pitch(&pitch,&gyro));          //更新姿态
    PWM_PD= Position_PD_Balance(pitch,gyro,mid);     //平衡环PD控制

    int encoder_l,encoder_r;
    int PWM_PI;
    encoder_l= ReadEncoder(3);  //读取左编码器的值
    encoder_r= ReadEncoder(2);  //读取右编码器的值
    PWM_PI=Incremental_PI_Speed(encoder_l,encoder_r);

    int PWM;
    PWM=PWM_PD+PWM_PI;  //计算最终的PWM
    PWM= LimitPWM(PWM);

    MotorSetPWM(PWM,PWM);   //左右电机赋值PWM
//    printf("%f\r\n",pitch);
}
