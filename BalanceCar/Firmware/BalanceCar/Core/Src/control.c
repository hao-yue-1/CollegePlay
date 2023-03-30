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
*	�� �� ��: MotorSpeedCtrl
*	����˵��: ʹ������ʽPI���Ƶ���ٶ�
*	��    ��: speed_l��������PWM
*	          speed_r���ҵ����PWM
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MotorSpeedCtrl(int speed_l,int speed_r)
{
    int encoder_l,encoder_r;
    encoder_l= ReadEncoder(3);  //��ȡ���������ֵ
    encoder_r= ReadEncoder(2);  //��ȡ�ұ�������ֵ

    int pwm_l,pwm_r;
    pwm_l= Incremental_PI_L(encoder_l,speed_l);   //����PID����
    pwm_r= Incremental_PI_R(encoder_r,speed_r);   //�ҵ��PID����

    pwm_l= LimitPWM(pwm_l); //����PWM�޷�
    pwm_r= LimitPWM(pwm_r); //�ҵ��PWM�޷�

    //ʹ�ò�����ʾ��λ������PID
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

    MotorSetPWM(pwm_l,pwm_r);   //�����ֵPWM
}

/*
*********************************************************************************************************
*	�� �� ��: LimitPWM
*	����˵��: PWM�޷�
*	��    ��: �޷�ǰ��PWM
*	�� �� ֵ: �޷����PWM
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
*	�� �� ��: BalancePIDCtrl
*	����˵��: ƽ��С���Ŀ��ƣ�ʵ��ԭ����ƽ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BalancePIDCtrl(float mid)
{
    float pitch;
    short gyro;
    int PWM_PD;
    while(mpu_dmp_get_pitch(&pitch,&gyro));          //������̬
    PWM_PD= Position_PD_Balance(pitch,gyro,mid);     //ƽ�⻷PD����

    int encoder_l,encoder_r;
    int PWM_PI;
    encoder_l= ReadEncoder(3);  //��ȡ���������ֵ
    encoder_r= ReadEncoder(2);  //��ȡ�ұ�������ֵ
    PWM_PI=Incremental_PI_Speed(encoder_l,encoder_r);

    int PWM;
    PWM=PWM_PD+PWM_PI;  //�������յ�PWM
    PWM= LimitPWM(PWM);

    MotorSetPWM(PWM,PWM);   //���ҵ����ֵPWM
//    printf("%f\r\n",pitch);
}
