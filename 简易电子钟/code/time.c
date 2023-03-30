#include "time.h"
#include <reg51.h>
#include "lcd12864.h"
#include "display.h"

sbit LED=P1^7;		//LED��	//���ӵ������ʾ

unsigned int cnt=0;			//��ʱ��0�еļ�����
unsigned int cnt_alarm=7;	//LED����˸����������
char flag=1;		//ˢ��ʱ����ʾ��־


MyTime my_time;		//��ͨʱ��
MyTime alarm_time;	//����

//��ʼ��ʱ��
void InitMyTime(int hour,int minute,int seconds)
{	
	InitTimer0();	//��ʼ����ʱ��0
	//��ʼ��ʱ��
	my_time.hour=hour;
	my_time.minute=minute;
	my_time.seconds=seconds;
	my_time.flag=0;
	//��ʼ��LCD��ʾ
	ClearScreen(0);
	Set_line(0);
	//ʱ����
	PrintTimeHour(my_time.hour);
	PrintTimeMinute(my_time.minute);
	PrintTimeSeconds(my_time.seconds);
	//��ʼ����ʾ
	PrintMode(0);
	PrintCursor(0);
	PrintColon();
}

//��ʼ������
void InitAlarmTime(int hour,int minute,int seconds,int flag)
{
	//��ʼ������
	alarm_time.hour=hour;
	alarm_time.minute=minute;
	alarm_time.seconds=seconds;
	alarm_time.flag=flag;
	//��ʼ����ʾ
	PrintClock(alarm_time.flag);
	LED=1;					//�ر�LED��
}

//�޸�ʱ�䣨�ж�ר�ã�
/*
*********************************************************************************************************
*	�� �� ��: ModifyTimeInterrupt
*	����˵��: 1. ��ʱ�ӵĲ��������޸ģ�ʵ��ʱ������Լӣ��Լ���λ
*			  2. �����������Ծ������ҵ����趨��ʱ�䣬����LED����˸3�ε����ѣ����ر����� 		
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ModifyTimeInterrupt()
{
	//��
	my_time.seconds++;
	if(flag)
	{
		PrintTimeSecondsInterrupt(my_time.seconds);		//��LCD�ϸ�����
	}
	else	//��Ч��ʱ
	{
		PrintClockNULLInterrupt(my_time.seconds);
	}
	//��->��
	if(my_time.seconds==60)
	{
		my_time.minute++;
		my_time.seconds=0;
		if(flag)
		{
			PrintTimeMinuteInterrupt(my_time.minute);	//��LCD�ϸ��·�
			PrintTimeSecondsInterrupt(my_time.seconds);	//��LCD�ϸ�����
		}
	}
	//��->ʱ
	if(my_time.minute==60)
	{
		my_time.hour++;
		my_time.minute=0;
		if(flag)
		{
			PrintTimeHourInterrupt(my_time.hour);		//��LCD�ϸ���ʱ
			PrintTimeMinuteInterrupt(my_time.minute);	//��LCD�ϸ��·�
		}
	}
	//ʱ
	if(my_time.hour==24)
	{
		my_time.hour=0;
		if(flag)
		{
			PrintTimeHourInterrupt(my_time.hour);		//��LCD�ϸ���ʱ
		}
	}
	//�ж������Ƿ񵽵�
	if(alarm_time.flag==1&&alarm_time.hour==my_time.hour&&alarm_time.minute==my_time.minute&&alarm_time.seconds==my_time.seconds)	//�����Ƿ񵽵�
	{	
		cnt_alarm--;	//��������
	}
	//LED��˸3�κ�ر�����	//LED��˸����=(i�ĳ�ʼֵ-1)/2
	if(cnt_alarm<7)
	{
		LED=!LED;	
		cnt_alarm--;
		if(cnt_alarm==0)
		{
			cnt_alarm=7;		//����������
			alarm_time.flag=0;	//�ر�����
			PrintClockInterrupt(alarm_time.flag);
		}
	}
}

//��ʼ����ʱ��0
void InitTimer0()
{
    TMOD=0x01;		//0000 0001	//T0�����ڷ�ʽ1��16λ��ʱ��
    TH0=(65536-50200)/256;	//50000us	256=2^8
    TL0=(65536-50200)%256;	//ȡ��8λ
    ET0=1;	//������ʱ��0���ж�
    EA=1;	//�������ж�
    TR0=1;	//������ʱ��0
}

/*
* ���޸�ʱ������ж���
* 50000:16  һ����1.00.81  ������3.04.61  �ķ���4.04.87
* 51000:16  һ����1.04.63  �ķ���4.09.81
* 50500:16  һ����1.03.95  ������2.04.69  �����5.06.88
* 50200:16  һ����1.03.13  ������2.03.45  �����5.04.59  ����һ���ӻ���3�룬��������׼ȷ��
*/

//��ʱ��0�ж�ִ�к���
void Timer0_isr()	interrupt 1
{
    //���г�ֵ����
    TH0=(65536-50200)/256;
    TL0=(65536-50200)%256;
    //�����ж�
	cnt++;
	//ֻ�н�����һ�������֮��Ż����һ�����ݴ���
	if(cnt==16)	//������һ���ʱ�䳤��	//����ֵΪ50000:20������ʵ���������
	{
		cnt=0;					//����������
		ModifyTimeInterrupt();	//�޸�ʱ��
	}
}