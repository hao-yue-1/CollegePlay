//
// Created by yue on 2022/1/25.
//

#include "pid.h"

/*
*********************************************************************************************************
*	�� �� ��: Incremental_PI_L
*	����˵��: ����ʽPI
*	��    ��: Encoder������������ֵ
             Target �����Ŀ���ٶ�
*	�� �� ֵ: ���PWM
*********************************************************************************************************
*/
int Incremental_PI_L(int Encoder,int Target)
{
    float Kp=30,Ki=20;
    static int Bias,Pwm,Last_bias;

    Bias=Target-Encoder;               //����ƫ��
    Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;  //����ʽPI������
    Last_bias=Bias;	                   //������һ��ƫ��

    return Pwm;                        //�������
}

/*
*********************************************************************************************************
*	�� �� ��: Incremental_PI_R
*	����˵��: ����ʽPI
*	��    ��: Encoder������������ֵ
             Target �����Ŀ���ٶ�
*	�� �� ֵ: ���PWM
*********************************************************************************************************
*/
int Incremental_PI_R(int Encoder,int Target)
{
    float Kp=30,Ki=20;
    static int Bias,Pwm,Last_bias;

    Bias=Target-Encoder;               //����ƫ��
    Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;  //����ʽPI������
    Last_bias=Bias;	                   //������һ��ƫ��

    return Pwm;                        //�������
}

/*
*********************************************************************************************************
*	�� �� ��: Position_PID_Balance
*	����˵��: λ��ʽPD����С����ƽ��
*	��    ��: pitch�������Ǹ�����
             mid  ��ƽ��С��ƽ��Ƕ�
*	�� �� ֵ: ���PWM
*********************************************************************************************************
*/
int Position_PD_Balance(float pitch,short gyro,float mid)
{
    float Kp=-250*0.8,Kd=-1*0.8;        //PD����,Kp=-250,Kd=-0.5
    int PWM;

//    PWM=Kp*(mid-pitch)+Kd*gyro;
    PWM=Kp*(pitch-mid)+Kd*gyro;    //����PD���㣬Kd������˵���ƽ����ٶȣ�pwm=kp*bias+kd*gyro�����ٶ�gyro���Ǹ�����pitch��΢��
    return PWM;
}

/*
*********************************************************************************************************
*	�� �� ��: Incremental_PI_Speed
*	����˵��: λ��ʽPI�ٶȿ�����������ƽ��С�����ٶȻ�
*	��    ��: Encoder_L�������������ֵ
             Encoder_R���ұ���������ֵ
*	�� �� ֵ: ���PWM
*********************************************************************************************************
*/
int Incremental_PI_Speed(int Encoder_L,int Encoder_R)
{
    static float PWM,Encoder_Least,Encoder;
    static float Encoder_Integral;
    float Kp=30,Ki=Kp/200;  //Kp=30 //Kp̫�������񵴣�̫С��һֱ��һ��������

    Encoder_Least = (Encoder_L+Encoder_R)-0;             //��ȡ�����ٶ�ƫ���Ϊ��һ��ƫ�������˲�==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩
    Encoder *= 0.7;		                                 //һ�׵�ͨ�˲���  //�����һ�׵�ͨ�˲���ʽ�ϵ���һ���ͺ��˲���Encoder=Encoder*0.7+0.3*Encoder_Least*0.3
    Encoder += Encoder_Least*0.3;	                     //һ�׵�ͨ�˲���
    Encoder_Integral += Encoder;                         //���ֳ�λ��
    if(Encoder_Integral>100)  	Encoder_Integral=100;    //�����޷�
    if(Encoder_Integral<-100) 	Encoder_Integral=-100;   //�����޷�
    PWM=Kp*Encoder+Ki*Encoder_Integral;                  //PI����
    return PWM;
}