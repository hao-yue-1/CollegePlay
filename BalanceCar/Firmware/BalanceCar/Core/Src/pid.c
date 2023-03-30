//
// Created by yue on 2022/1/25.
//

#include "pid.h"

/*
*********************************************************************************************************
*	函 数 名: Incremental_PI_L
*	功能说明: 增量式PI
*	形    参: Encoder：编码器测量值
             Target ：电机目标速度
*	返 回 值: 电机PWM
*********************************************************************************************************
*/
int Incremental_PI_L(int Encoder,int Target)
{
    float Kp=30,Ki=20;
    static int Bias,Pwm,Last_bias;

    Bias=Target-Encoder;               //计算偏差
    Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;  //增量式PI控制器
    Last_bias=Bias;	                   //保存上一次偏差

    return Pwm;                        //增量输出
}

/*
*********************************************************************************************************
*	函 数 名: Incremental_PI_R
*	功能说明: 增量式PI
*	形    参: Encoder：编码器测量值
             Target ：电机目标速度
*	返 回 值: 电机PWM
*********************************************************************************************************
*/
int Incremental_PI_R(int Encoder,int Target)
{
    float Kp=30,Ki=20;
    static int Bias,Pwm,Last_bias;

    Bias=Target-Encoder;               //计算偏差
    Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;  //增量式PI控制器
    Last_bias=Bias;	                   //保存上一次偏差

    return Pwm;                        //增量输出
}

/*
*********************************************************************************************************
*	函 数 名: Position_PID_Balance
*	功能说明: 位置式PD控制小车的平衡
*	形    参: pitch：陀螺仪俯仰角
             mid  ：平衡小车平衡角度
*	返 回 值: 电机PWM
*********************************************************************************************************
*/
int Position_PD_Balance(float pitch,short gyro,float mid)
{
    float Kp=-250*0.8,Kd=-1*0.8;        //PD参数,Kp=-250,Kd=-0.5
    int PWM;

//    PWM=Kp*(mid-pitch)+Kd*gyro;
    PWM=Kp*(pitch-mid)+Kd*gyro;    //这里PD计算，Kd参数相乘的是平衡角速度；pwm=kp*bias+kd*gyro，角速度gyro就是俯仰角pitch的微分
    return PWM;
}

/*
*********************************************************************************************************
*	函 数 名: Incremental_PI_Speed
*	功能说明: 位置式PI速度控制器，用于平衡小车的速度环
*	形    参: Encoder_L：左编码器测量值
             Encoder_R：右编码器测量值
*	返 回 值: 电机PWM
*********************************************************************************************************
*/
int Incremental_PI_Speed(int Encoder_L,int Encoder_R)
{
    static float PWM,Encoder_Least,Encoder;
    static float Encoder_Integral;
    float Kp=30,Ki=Kp/200;  //Kp=30 //Kp太大会出现振荡，太小会一直朝一个方向走

    Encoder_Least = (Encoder_L+Encoder_R)-0;             //获取最新速度偏差保存为上一次偏差用于滤波==测量速度（左右编码器之和）-目标速度（此处为零）
    Encoder *= 0.7;		                                 //一阶低通滤波器  //这里的一阶低通滤波形式上等于一阶滞后滤波：Encoder=Encoder*0.7+0.3*Encoder_Least*0.3
    Encoder += Encoder_Least*0.3;	                     //一阶低通滤波器
    Encoder_Integral += Encoder;                         //积分出位移
    if(Encoder_Integral>100)  	Encoder_Integral=100;    //积分限幅
    if(Encoder_Integral<-100) 	Encoder_Integral=-100;   //积分限幅
    PWM=Kp*Encoder+Ki*Encoder_Integral;                  //PI计算
    return PWM;
}