#ifndef _DISPLAY_H
#define _DISPLAY_H

void delay(unsigned int x);	//�򵥵���ʱ����

void PrintCursor(int cursor);		//��LCD���ƶ����
void PrintMode(int mode);			//��LCD����ʾģʽ

void PrintTimeHour(int hour);					//��LCD������ʾʱ�䣨ʱ��
void PrintTimeMinute(int minute);				//��LCD������ʾʱ�䣨�֣�
void PrintTimeSeconds(int seconds); 			//��LCD������ʾʱ�䣨�룩
void PrintTimeHourReverse(int hour);			//��LCD������ʾʱ�䣨ʱ��������
void PrintTimeMinuteReverse(int minute);		//��LCD������ʾʱ�䣨�֣�������
void PrintTimeSecondsReverse(int seconds); 		//��LCD������ʾʱ�䣨�룩������
void PrintTimeHourInterrupt(int hour);			//��LCD������ʾʱ�䣨ʱ�����ж�ר�ã�
void PrintTimeMinuteInterrupt(int minute);		//��LCD������ʾʱ�䣨�֣����ж�ר�ã�
void PrintTimeSecondsInterrupt(int seconds); 	//��LCD������ʾʱ�䣨�룩���ж�ר�ã�

void PrintSetOK(int mode);				//��LCD����ʾ���óɹ�
void PrintColon();						//��LCD����ʾð�ţ�:��
void PrintClock(int flag);				//��LCD����ʾ����ͼ��
void PrintClockInterrupt(int flag);		//��LCD����ʾ����ͼ�꣨�ж�ר�ã�
void PrintReturn(int mode);				//��LCD����ʾ�˳�����

void PrintClockNULLInterrupt(int minute);	//һ����Ч����ʱ

#endif