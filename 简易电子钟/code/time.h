#ifndef _TIME_H
#define _TIME_H

//ʱ�ӽṹ��
typedef struct
{
	int hour;		//ʱ
	int minute;		//��
	int seconds;	//��
	char flag;		//���ӱ�־	1.����������    2.û����������
}MyTime;

extern MyTime my_time;		//��ͨʱ��
extern MyTime alarm_time;	//����

extern unsigned int cnt;	//��ʱ��0�еļ�����
extern char flag;			//ˢ��ʱ����ʾ��־	1.��ʾˢ��	2.��ˢ����ʾ

void InitTimer0();												//��ʼ����ʱ��0
void InitMyTime(int hour,int minute,int seconds);				//��ʼ��ʱ��
void InitAlarmTime(int hour,int minute,int seconds,int flag);	//��ʼ������

void ModifyTimeInterrupt();	//�޸�ʱ�䣨�ж�ר�ã�	

#endif